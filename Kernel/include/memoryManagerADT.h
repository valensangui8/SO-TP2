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

void init_memory_manager_ADT(uint64_t size, void *start_address);
void *alloc_memory_ADT(uint64_t size);
void free_memory_ADT(void *ptr);
void get_memory_info_ADT(char *type, uint64_t *free, uint64_t *allocated);

#endif