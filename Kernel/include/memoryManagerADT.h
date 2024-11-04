#ifndef _MEMORY_MANAGER_ADT_H
#define _MEMORY_MANAGER_ADT_H

#include "videoDriver.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <utils.h>

#define BLOCK_SIZE 4096
#define HEAP_START 0x100000
#define MM_SIZE 0x1000000
#define TOTAL_BLOCKS (MM_SIZE / BLOCK_SIZE)

#define FREE 0
#define USED 1

typedef struct MemoryManagerCDT *MemoryManagerADT;

MemoryManagerADT init_memory_manager(uint64_t size, void *start_address);
void *alloc_memory(uint64_t size);
void free_memory(void *ptr);
void mem_info_ADT(int * current, int * free, int * total);

#endif