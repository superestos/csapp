/*
 * mm-naive.c - The least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by allocating a
 * new page as needed.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

#define HEADER_SIZE 8

/* always use 16-byte alignment */
#define ALIGNMENT 16

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

/* rounds up to the nearest multiple of mem_pagesize() */
#define PAGE_ALIGN(size) (((size) + (mem_pagesize()-1)) & ~(mem_pagesize()-1))

void *current_avail = NULL;
int current_avail_size = 0;

struct memblock {
  size_t size;
  struct memblock *prev;
  struct memblock *next;
};

struct memblock *head;

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
  head = NULL;
  return 0;
}

size_t real_size(size_t size) {
  return size > ALIGNMENT? size - ALIGNMENT: 0;
}

size_t padding_size(size_t size) {
  return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

size_t paging_size(size_t size) {
  size_t metadata_size = 32;

  return (size + metadata_size + mem_pagesize() - 1) & ~(mem_pagesize() - 1);
}

size_t memblock_length(struct memblock *block) {
  return block->size & ~(ALIGNMENT - 1);
}

size_t memblock_alloc(struct memblock *block) {
  return block->size & 1;
}

void set_header(struct memblock *block, size_t block_size, size_t alloc) {
  size_t size = block_size & ~(ALIGNMENT - 1);

  block->size = (alloc & 1) | size;
  struct memblock *end = (struct memblock *)(((void *)block) + size - HEADER_SIZE);
  end->size = block->size;
}

struct memblock *create_memblock(void *mem, size_t block_size) {
  struct memblock *block = (struct memblock *)mem;

  size_t size = real_size(block_size);
  if (size == 0) {
    return NULL;
  }

  set_header(block, block_size, 0);
  block->next = NULL;
  block->prev = NULL;

  return block;
}

struct memblock *trim_memblock(struct memblock *block, size_t block_size) {
  struct memblock *remain_block = create_memblock(((void *)block) + block_size, memblock_length(block) - block_size);

  if (remain_block != NULL) {
    set_header(block, block_size, 0);
  }

  return remain_block;
}

void add_memblock(struct memblock *block) {
  if (block == NULL)
    return;

  block->next = head;
  block->prev = NULL;
  if (head != NULL) {
    head->prev = block;
  }

  head = block;
}

void delete_memblock(struct memblock *block) {  
  if (block->prev != NULL)
    block->prev->next = block->next;
  if (block->next != NULL)
    block->next->prev = block->prev;

  if (head == block) {
    head = block->next;

    if (head != NULL)
      head->prev = NULL;
  }
}

struct memblock *search(size_t block_size) {
  struct memblock *result = NULL;
  size_t min;

  struct memblock *p = head;
  for (; p != NULL; p = p->next) {
    if (memblock_length(p) > block_size) {
      if (result == NULL || min > memblock_length(p)) {
        result = p;
        min = memblock_length(p);
      }
    }
    else if (memblock_length(p) == block_size) {
      return p;
    }
  }

  return result;
}

/* 
 * mm_malloc - Allocate a block by using bytes from current_avail,
 *     grabbing a new page if necessary.
 */
void *mm_malloc(size_t size)
{
  size = padding_size(size);

  size_t block_size = size + ALIGNMENT;

  struct memblock *p = search(block_size);  

  if (p == NULL) {
    size_t page_size = paging_size(block_size + ALIGNMENT);
    void *mem = mem_map(page_size);
    if (mem == NULL)
      return NULL;

    set_header((struct memblock *)mem, page_size, 1);

    p = create_memblock(mem + HEADER_SIZE, page_size - ALIGNMENT);
  }
  else {
    delete_memblock(p);
  }

  add_memblock(trim_memblock(p, block_size));

  set_header(p, p->size, 1);
  return ((void *)p + HEADER_SIZE);
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
  struct memblock *block = (struct memblock *)(ptr - HEADER_SIZE);

  struct memblock *succ = (struct memblock *)(((void *)block) + memblock_length(block));
  if (!memblock_alloc(succ)) {
    delete_memblock(succ);
    set_header(block, block->size + succ->size, 0);
  }
  
  
  struct memblock *prev = (struct memblock *)(((void *)block) - HEADER_SIZE);
  if (!memblock_alloc(prev)) {
    prev = (struct memblock *)(((void *)block) - memblock_length(prev));
    set_header(prev, prev->size + block->size, 0);
    return;
  }
  
  set_header(block, block->size, 0);
  add_memblock(block);
}
