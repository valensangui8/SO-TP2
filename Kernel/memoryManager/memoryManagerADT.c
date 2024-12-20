// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef BUDDY // Compila solo si BUDDY_SYSTEM no está definido

#include "memoryManagerADT.h"

typedef struct MemoryInfo {
	void *free_blocks[TOTAL_BLOCKS];
	int current;
} MemoryInfo;

struct MemoryManagerCDT {
	uint64_t size;
	void *start_address;
	MemoryInfo info;
};

static int check_already_free(void *ptr);
static int free_blocks_remaining(MemoryInfo *memory_info);
static MemoryManagerADT get_memory_manager();

void init_memory_info(MemoryInfo *memory_info, void *start) {
	memory_info->current = 0;
	for (int i = 0; i < TOTAL_BLOCKS - 1; i++) {
		memory_info->free_blocks[i] = start + (BLOCK_SIZE * i);
	}
}

void init_memory_manager_ADT(uint64_t size, void *start_address) {
	if ((uintptr_t) start_address < HEAP_START) {
		return;
	}
	MemoryManagerADT new_memory = (MemoryManagerADT) MM_ADDRESS;
	new_memory->start_address = start_address;
	new_memory->size = size;

	init_memory_info(&(new_memory->info), start_address);
}

void *alloc_memory_ADT(uint64_t size) {
	MemoryManagerADT memory = get_memory_manager();
	int free_remaining = free_blocks_remaining(&(memory->info));
	int needed_blocks;

	if (free_remaining == 0 || (needed_blocks = (size + BLOCK_SIZE - 1) / BLOCK_SIZE) > free_remaining) {
		return NULL;
	}

	void *ret_ptr = memory->info.free_blocks[memory->info.current];
	memory->info.current += needed_blocks;
	return ret_ptr;
}

void free_memory_ADT(void *ptr) {
	MemoryManagerADT memory = get_memory_manager();
	if (ptr > memory->size + memory->start_address || ptr < memory->start_address || memory->info.current == 0) {
		// error no es válido el puntero
		draw_with_color("Error: Pointer is not valid", 0xFF0000);
		return;
	}
	if (check_already_free(ptr) == 1 || free_blocks_remaining(&(memory->info)) == 0) {
		draw_with_color("Error: Double free not allowed", 0xFF0000);
		return;
	}
	memory->info.free_blocks[--(memory->info.current)] = ptr;
}

static int check_already_free(void *ptr) {
	MemoryManagerADT memory = get_memory_manager();
	for (int i = memory->info.current; i < free_blocks_remaining(&(memory->info)); i++) {
		if (memory->info.free_blocks[i] == ptr) {
			return 1;
		}
	}
	return 0;
}

static int free_blocks_remaining(MemoryInfo *memory_info) {
	return TOTAL_BLOCKS - memory_info->current;
}

static MemoryManagerADT get_memory_manager() {
	return (MemoryManagerADT) MM_ADDRESS;
}

void get_memory_info_ADT(char *type, uint64_t *free, uint64_t *allocated, uint64_t *total) {
	MemoryManagerADT memory = get_memory_manager();
	my_strcpy(type, "Block Memory");

	*free = free_blocks_remaining(&(memory->info)) * BLOCK_SIZE;

	*allocated = memory->size - *free;
	*total = memory->size;
}

#endif
