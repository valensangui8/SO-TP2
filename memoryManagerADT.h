#ifndef MEMORY_MANAGER_ADT_H
#define MEMORY_MANAGER_ADT_H

#include <stddef.h>
#include <stdint.h>

#define MEM_ADDRESS 0x50000 // del manual pure64 aca esta free
// #define MEM_ADDRESS 0x100000	// recomienda usar pure64

#define BLOCK_SIZE 10
#define GET_BLOCKS(mem) (mem / BLOCK_SIZE)
#define MEM_BLOCK(blocks) (blocks * BLOCK_SIZE)
#define FREE 0

typedef struct MemoryManagerCDT *MemoryManagerADT;

MemoryManagerADT init_memory_manager(uint64_t size, void *start_address);
void *alloc_memory(MemoryManagerADT memory, uint64_t size);
void free_memory(MemoryManagerADT memory, void *ptr, uint64_t size);

#endif