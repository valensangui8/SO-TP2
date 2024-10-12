#ifndef _MEMORY_MANAGER_ADT_H
#define _MEMORY_MANAGER_ADT_H

#include "videoDriver.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <utils.h>

#define BLOCK_SIZE 0x400
#define HEAP_START 0x100000
#define MM_SIZE 0x100000
#define TOTAL_BLOCKS (MM_SIZE / BLOCK_SIZE)

#define FREE 0
#define USED 1

typedef struct MemoryManagerCDT *MemoryManagerADT;

MemoryManagerADT init_memory_manager(uint64_t size, void *start_address);
void *alloc_memory(uint64_t size);
void free_memory(void *ptr);

#endif