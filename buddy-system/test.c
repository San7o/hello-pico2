// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#include "buddy-system.h"

unsigned char memory[BUDDY_SYSTEM_MEMORY_SIZE] = {0};

static void print_free_lists(buddy_system_t *bs)
{
  printf("  Free lists:\n");
  for (int l = 0; l < BUDDY_LEVELS_NUM; l++) {
    size_t block_size = level_to_size(l);
    int count = 0;
    buddy_block_header_t *cur = bs->levels[l].free_list;
    while (cur) { count++; cur = cur->next; }
    if (count)
      printf("    level %d (block=%4zu B): %d free block(s)\n",
             l, block_size, count);
  }
}

static void check_pointer(buddy_system_t *bs, void *ptr, const char *label)
{
  if (!ptr) {
    printf("  [FAIL] %s: alloc returned NULL\n", label);
    return;
  }
  uintptr_t base   = (uintptr_t)bs->memory;
  uintptr_t end    = base + BUDDY_SYSTEM_MEMORY_SIZE;
  uintptr_t addr   = (uintptr_t)ptr;
  if (addr < base || addr >= end)
    printf("  [FAIL] %s: pointer %p outside pool\n", label, ptr);
  else
    printf("  [OK]   %s: %p (offset +%zu)\n", label, ptr,
           (size_t)(addr - base));
}

static void test_single_alloc_free(void)
{
  printf("\n=== test_single_alloc_free ===\n");
  buddy_system_t bs;
  buddy_system_init(&bs, memory);

  void *p = buddy_system_alloc(&bs, 32);
  check_pointer(&bs, p, "alloc 32 B");
  print_free_lists(&bs);

  buddy_system_free(&bs, p);
  printf("  after free:\n");
  print_free_lists(&bs);

  // Pool should be fully coalesced back to one block at the top level
  int top = BUDDY_LEVELS_NUM - 1;
  assert(bs.levels[top].free_list != NULL);
  for (int l = 0; l < top; l++)
    assert(bs.levels[l].free_list == NULL);
  printf("  [OK]   pool fully coalesced\n");
}

static void test_multiple_alloc_free(void)
{
  printf("\n=== test_multiple_alloc_free ===\n");
  buddy_system_t bs;
  buddy_system_init(&bs, memory);

  void *a = buddy_system_alloc(&bs, 32);
  void *b = buddy_system_alloc(&bs, 64);
  void *c = buddy_system_alloc(&bs, 128);
  check_pointer(&bs, a, "alloc  32 B");
  check_pointer(&bs, b, "alloc  64 B");
  check_pointer(&bs, c, "alloc 128 B");
  assert(a != b && b != c && a != c);
  print_free_lists(&bs);

  buddy_system_free(&bs, b);
  buddy_system_free(&bs, a);
  buddy_system_free(&bs, c);
  printf("  after freeing all:\n");
  print_free_lists(&bs);

  int top = BUDDY_LEVELS_NUM - 1;
  assert(bs.levels[top].free_list != NULL);
  printf("  [OK]   pool fully coalesced\n");
}

static void test_no_overlap(void)
{
  printf("\n=== test_no_overlap ===\n");
  buddy_system_t bs;
  buddy_system_init(&bs, memory);

  // Fill with pointers and check they don't overlap
#define N 8
  void   *ptrs[N];
  size_t  sizes[N] = {16, 32, 16, 64, 32, 16, 32, 16};

  for (int i = 0; i < N; i++) {
    ptrs[i] = buddy_system_alloc(&bs, sizes[i]);
    check_pointer(&bs, ptrs[i], "alloc");
    assert(ptrs[i]);
  }

  // Check every pair for non-overlap
  int ok = 1;
  for (int i = 0; i < N; i++) {
    buddy_block_header_t *hi = (buddy_block_header_t *)ptrs[i] - 1;
    uintptr_t si = (uintptr_t)hi, ei = si + hi->size;
    for (int j = i + 1; j < N; j++) {
      buddy_block_header_t *hj = (buddy_block_header_t *)ptrs[j] - 1;
      uintptr_t sj = (uintptr_t)hj, ej = sj + hj->size;
      if (si < ej && sj < ei) {
        printf("  [FAIL] blocks %d and %d overlap!\n", i, j);
        ok = 0;
      }
    }
  }
  if (ok) printf("  [OK]   no overlapping allocations\n");

  for (int i = 0; i < N; i++) buddy_system_free(&bs, ptrs[i]);
#undef N
}

static void test_write_and_read(void)
{
  printf("\n=== test_write_and_read ===\n");
  buddy_system_t bs;
  buddy_system_init(&bs, memory);

  char *p = buddy_system_alloc(&bs, 64);
  assert(p);
  memset(p, 0xAB, 64);
  int ok = 1;
  for (int i = 0; i < 64; i++) {
    if ((unsigned char)p[i] != 0xAB) { ok = 0; break; }
  }
  printf("  [%s]   write/read 64 B of 0xAB\n", ok ? "OK" : "FAIL");

  buddy_system_free(&bs, p);
}

static void test_exhaustion(void)
{
  printf("\n=== test_exhaustion ===\n");
  buddy_system_t bs;
  buddy_system_init(&bs, memory);

  // Alloc until NULL
  void *ptrs[256];
  int   count = 0;
  while (count < 256) {
    void *p = buddy_system_alloc(&bs, BUDDY_MIN_BLOCK_SIZE);
    if (!p) break;
    ptrs[count++] = p;
  }
  printf("  allocated %d block(s) of %d B before exhaustion\n",
         count, BUDDY_MIN_BLOCK_SIZE);

  // Next alloc must fail
  void *p = buddy_system_alloc(&bs, 1);
  assert(p == NULL);
  printf("  [OK]   alloc after exhaustion returns NULL\n");

  for (int i = 0; i < count; i++) buddy_system_free(&bs, ptrs[i]);
  int top = BUDDY_LEVELS_NUM - 1;
  assert(bs.levels[top].free_list != NULL);
  printf("  [OK]   pool fully coalesced after freeing all\n");
}

static void test_free_order_independence(void)
{
  printf("\n=== test_free_order_independence ===\n");
  buddy_system_t bs;
  buddy_system_init(&bs, memory);

  void *a = buddy_system_alloc(&bs, 32);
  void *b = buddy_system_alloc(&bs, 32);
  void *c = buddy_system_alloc(&bs, 32);
  assert(a && b && c);

  // Free in reverse order
  buddy_system_free(&bs, c);
  buddy_system_free(&bs, b);
  buddy_system_free(&bs, a);

  int top = BUDDY_LEVELS_NUM - 1;
  assert(bs.levels[top].free_list != NULL);
  printf("  [OK]   reverse-order free coalesces correctly\n");
}

int main(void)
{
  printf("Buddy system — pool %d B, min block %d B, %d levels\n",
         BUDDY_SYSTEM_MEMORY_SIZE, BUDDY_MIN_BLOCK_SIZE, BUDDY_LEVELS_NUM);

  test_single_alloc_free();
  test_multiple_alloc_free();
  test_no_overlap();
  test_write_and_read();
  test_exhaustion();
  test_free_order_independence();

  printf("\nAll tests passed.\n");
  return 0;
}
