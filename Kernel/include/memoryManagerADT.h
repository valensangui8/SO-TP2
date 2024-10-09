#ifndef MEMORY_MANAGER_ADT_H
#define MEMORY_MANAGER_ADT_H

#include <stddef.h>
#include <stdint.h>
#include <utils.h>

#define SIZE_MEM 0x10000


#define BLOCK_SIZE 0x400
#define TOTAL_BLOCKS 1024
#define FREE 0
#define USED 1

typedef struct MemoryManagerCDT *MemoryManagerADT;

MemoryManagerADT init_memory_manager(uint64_t size, void *start_address);
void *alloc_memory(uint64_t size);
void free_memory(void *ptr);

#endif