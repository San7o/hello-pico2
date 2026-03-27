// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "buddy-system.h"

int size_to_level(size_t size)
{
  int level = 0;
  size_t block = BUDDY_MIN_BLOCK_SIZE;
  while (block < size && level < BUDDY_LEVELS_NUM - 1)
  {
    block <<= 1;
    level++;
  }
  return level;
}

size_t level_to_size(int level)
{
  return (size_t)BUDDY_MIN_BLOCK_SIZE << level;
}

static buddy_block_header_t *get_buddy(buddy_system_t *bs,
                                       buddy_block_header_t *block)
{
  uintptr_t offset = (uintptr_t)block - (uintptr_t)bs->memory;
  uintptr_t buddy_offset = offset ^ block->size;
  return (buddy_block_header_t *)(bs->memory + buddy_offset);
}

void buddy_system_init(buddy_system_t *bs, unsigned char *memory)
{
  if (!bs) return;
  memset(bs, 0, sizeof(*bs));
  bs->memory = memory;

  // Insert the whole pool as one free block at the top level
  int top = BUDDY_LEVELS_NUM - 1;
  buddy_block_header_t *block = (buddy_block_header_t *)memory;
  block->size = level_to_size(top);
  block->next = NULL;
  bs->levels[top].free_list = block;
}

void *buddy_system_alloc(buddy_system_t *bs, size_t size)
{
  if (!bs || size == 0) return NULL;

  // Account for the header living inside the block
  size += sizeof(buddy_block_header_t);
  if (size < BUDDY_MIN_BLOCK_SIZE) size = BUDDY_MIN_BLOCK_SIZE;

  int level = size_to_level(size);

  // Find a level that has a free block (split down if needed)
  int found = -1;
  for (int l = level; l < BUDDY_LEVELS_NUM; l++) {
    if (bs->levels[l].free_list) { found = l; break; }
  }
  if (found == -1) return NULL;  // out of memory

  // Split blocks down until we reach the requested level
  while (found > level)
  {
    buddy_block_header_t *block = bs->levels[found].free_list;
    bs->levels[found].free_list = block->next;
    found--;

    // The two halves sit at [block] and [block + new_size]
    size_t new_size = level_to_size(found);
    buddy_block_header_t *buddy = (buddy_block_header_t *)
      ((unsigned char *)block + new_size);

    block->size = new_size;
    buddy->size = new_size;

    // Push both halves onto the lower level; we'll pop one next iteration
    buddy->next = bs->levels[found].free_list;
    block->next = buddy;
    bs->levels[found].free_list = block;
  }

  // Pop the head of the target level
  buddy_block_header_t *block = bs->levels[level].free_list;
  bs->levels[level].free_list = block->next;
  block->next = NULL;

  // Return a pointer past the header
  return (void *)(block + 1);
}

void buddy_system_free(buddy_system_t *bs, void *ptr)
{
  if (!bs || !ptr) return;

  buddy_block_header_t *block = (buddy_block_header_t *)ptr - 1;
  int level = size_to_level(block->size);

  // Walk upward, merging with free buddies
  while (level < BUDDY_LEVELS_NUM - 1)
  {
    buddy_block_header_t *buddy = get_buddy(bs, block);

    // Check if the buddy is on this level's free list
    buddy_block_header_t **cur = &bs->levels[level].free_list;
    while (*cur && *cur != buddy) cur = &(*cur)->next;

    if (!*cur) break;  // buddy is allocated — stop here

    // Remove buddy from free list
    *cur = buddy->next;

    // Merge: the lower address is the merged block
    if (buddy < block) block = buddy;
    level++;
    block->size = level_to_size(level);
  }

  // Push the (possibly merged) block onto the appropriate free list
  block->next = bs->levels[level].free_list;
  bs->levels[level].free_list = block;
}
