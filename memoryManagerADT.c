#include "memoryManagerADT.h"

typedef struct MemoryInfo {
	uint64_t free_blocks;
	uint64_t used_blocks;
	uint64_t total_blocks;
	uint64_t free_memory;
	uint64_t used_memory;
} MemoryInfo;

typedef struct MemoryManagerCDT {
	uint64_t size;
	void *start_address;
	int index;
	MemoryInfo info;
};

void init_memory_info(MemoryInfo *memory_info, uint64_t size) {
	uint64_t blocks = GET_BLOCKS(size);
	memory_info->free_blocks = blocks;
	memory_info->total_blocks = blocks;
	memory_info->used_blocks = 0;
	memory_info->free_memory = size;
	memory_info->used_memory = 0;
}

MemoryManagerADT init_memory_manager(uint64_t size, void *start_address) {
	MemoryManagerADT new_memory = (MemoryManagerADT) MEM_ADDRESS; // Memory manager is assigned to the free memory
	new_memory->start_address = start_address;
	new_memory->size = size;
	new_memory->index = 0;
	init_memory_block_info(&(new_memory->info), size);
	return new_memory;
}
// ver tema de como saber que no esta ocupado ya
void *alloc_memory(MemoryManagerADT memory, uint64_t size) {
	uint64_t blocks = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;

	if (memory->index + blocks <= memory->info.total_blocks && memory->info.free_blocks >= blocks) {
		// uso char para usar bytes
		void *allocated_address = (uint8_t *) memory->start_address + (memory->index * (blocks));
		memory->info.free_blocks -= blocks;
		memory->info.used_blocks += blocks;
		memory->info.free_memory -= MEM_BLOCK(blocks);
		memory->info.used_memory += MEM_BLOCK(blocks);
		memory->index += blocks;
		return allocated_address;
	}
	return NULL;
}
// check tema de doble free como saber si ya se libero o no
void free_memory(MemoryManagerADT memory, void *ptr, uint64_t size) {
	if (ptr < memory->start_address || ptr >= (uint64_t *) memory->start_address + (memory->size)) {
		// error
		return;
	}

	uint64_t offset = (uint64_t *) ptr - (uint64_t *) memory->start_address;
	uint64_t block_index = GET_BLOCKS(offset);
	uint64_t blocks_to_free = GET_BLOCKS(size);

	memory->index = block_index;
	memory->info.free_blocks += blocks_to_free;
	memory->info.used_blocks -= blocks_to_free;
	memory->info.free_memory += size;
	memory->info.used_memory -= size;
}