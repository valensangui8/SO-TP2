#ifndef _BUDDY_H
#define _BUDDY_H

#include <stddef.h>
#include <stdint.h>
#include <utils.h>

// #define MIN_LEVELS 5
// #define MAX_LEVELS 27 
#define MIN_LEVELS 1
#define MAX_LEVELS 20

// #define BLOCK_SIZE 512
// #define MM_SIZE (1 << 20)
// #define TOTAL_BLOCKS (MM_SIZE / BLOCK_SIZE)
#define BASE 2

typedef struct MemoryManagerCDT * MemoryManagerADT;

void init_buddy_system(uint64_t size, void *start_address);
void *alloc_buddy_memory(uint64_t size);
void free_buddy_memory(void *ptr);
void get_memory_info_buddy(char *type, uint64_t *free, uint64_t *allocated, uint64_t *total);

#endif
