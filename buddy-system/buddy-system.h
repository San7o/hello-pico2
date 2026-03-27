// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#define BUDDY_SYSTEM_MEMORY_SIZE  (4 * 1024)
#define BUDDY_MIN_BLOCK_SIZE      16 
#define BUDDY_LEVELS_NUM          9  // 16 << 8 = 4096

typedef struct buddy_block_header buddy_block_header_t;

struct buddy_block_header {
  size_t                size;
  buddy_block_header_t *next;
};

typedef struct buddy_level {
  buddy_block_header_t *free_list;
} buddy_level_t;

typedef struct buddy_system {
  buddy_level_t  levels[BUDDY_LEVELS_NUM];
  unsigned char *memory;
} buddy_system_t;

void  buddy_system_init(buddy_system_t *bs, unsigned char *memory);
void *buddy_system_alloc(buddy_system_t *bs, size_t size);
void  buddy_system_free(buddy_system_t *bs, void *ptr);

// Utils

size_t level_to_size(int level);
int    size_to_level(size_t size);
