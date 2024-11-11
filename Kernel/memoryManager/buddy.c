// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifdef BUDDY
#include <buddy.h>

typedef struct BuddyBlock {
	struct BuddyBlock *prev;
	struct BuddyBlock *next;
	int level;
	char is_free;
} BuddyBlock;

static void remove_block_from_free_list(BuddyBlock *block);
static void add_block_to_free_list(BuddyBlock *block);
static BuddyBlock *split_block(BuddyBlock *block);
static BuddyBlock *create_free_block(void *start_address, int level, BuddyBlock *next, BuddyBlock *prev);

struct MemoryManagerCDT {
	uint64_t size;
	void *start_address;
	uint64_t max_levels;
	BuddyBlock *free_list[MAX_LEVELS];
};

static MemoryManagerADT get_memory_manager() {
	return (MemoryManagerADT) MM_ADDRESS;
}

void init_buddy_system(uint64_t size, void *start_address) {
	MemoryManagerADT memory = get_memory_manager();
	memory->start_address = start_address;
	memory->size = size;
	memory->max_levels = log2(size);

	if (memory->max_levels > MAX_LEVELS) {
		draw_with_color("Error: Size requested is too large", 0xFF0000);
	}
	else if (memory->max_levels < MIN_LEVELS) {
		draw_with_color("Error: Size requested is too small", 0xFF0000);
	}

	for (int i = 0; i < memory->max_levels; i++) {
		memory->free_list[i] = NULL;
	}

	int initial_level = memory->max_levels - 1;
	memory->free_list[initial_level] = create_free_block(start_address, initial_level, NULL, NULL);
}

static BuddyBlock *create_free_block(void *start_address, int level, BuddyBlock *next, BuddyBlock *prev) {
	BuddyBlock *block = (BuddyBlock *) start_address;
	block->level = level;
	block->is_free = 1;
	block->next = next;
	block->prev = prev;
	return block;
}

static BuddyBlock *split_block(BuddyBlock *block) {
	MemoryManagerADT memory = get_memory_manager();
	int level = block->level - 1;

	block->level = level;

	BuddyBlock *buddy = (BuddyBlock *) ((uint64_t) block + (1 << level));

	buddy->level = level;
	buddy->is_free = 1;
	buddy->next = memory->free_list[level];
	buddy->prev = NULL;

	if (memory->free_list[level] != NULL) {
		memory->free_list[level]->prev = buddy;
	}
	memory->free_list[level] = buddy;

	return block;
}

static BuddyBlock *find_block_buddy(int level) {
	MemoryManagerADT memory = get_memory_manager();

	if (level > memory->max_levels) {
		return NULL;
	}

	for (int i = level; i <= memory->max_levels; i++) {
		if (memory->free_list[i] != NULL) {
			BuddyBlock *block = memory->free_list[i];

			memory->free_list[i] = block->next;
			if (block->next != NULL) {
				block->next->prev = NULL;
			}
			block->next = block->prev = NULL;

			while (i > level) {
				block = split_block(block);
				i--;
			}

			block->is_free = 0;
			return block;
		}
	}

	return NULL;
}

void *alloc_buddy_memory(uint64_t size) {
	int level = log2(size + sizeof(BuddyBlock));

	if (level < MIN_LEVELS) {
		level = MIN_LEVELS;
	}
	else if (level > MAX_LEVELS) {
		draw_with_color("Error: Size requested is too large", 0xFF0000);
		return NULL;
	}

	BuddyBlock *block = find_block_buddy(level);
	if (block == NULL) {
		draw_with_color("Error: No available blocks", 0xFF0000);
		return NULL;
	}

	return (void *) ((uint64_t) block + sizeof(BuddyBlock));
}

static void merge_block(BuddyBlock *block) {
	MemoryManagerADT memory = get_memory_manager();

	int level = block->level;

	uint64_t relative_address = (uint64_t) block - (uint64_t) memory->start_address;
	uint64_t buddy_number = relative_address / (1 << level);
	uint64_t buddy_relative_address = 0;

	if (buddy_number % 2 == 0) {
		buddy_relative_address = relative_address + (1 << level);
	}
	else {
		buddy_relative_address = relative_address - (1 << level);
	}

	BuddyBlock *buddy = (BuddyBlock *) ((uint64_t) memory->start_address + buddy_relative_address);

	if ((uint64_t) buddy < (uint64_t) memory->start_address ||
		(uint64_t) buddy >= (uint64_t) memory->start_address + memory->size) {
		add_block_to_free_list(block);
		return;
	}

	if (buddy->is_free && buddy->level == level) {
		remove_block_from_free_list(buddy);

		BuddyBlock *merged_block = (block < buddy) ? block : buddy;
		merged_block->level = level + 1;

		merge_block(merged_block);
	}
	else {
		add_block_to_free_list(block);
	}
}

static void add_block_to_free_list(BuddyBlock *block) {
	MemoryManagerADT memory = get_memory_manager();
	int level = block->level;

	block->is_free = 1;
	block->next = memory->free_list[level];
	block->prev = NULL;

	if (memory->free_list[level] != NULL) {
		memory->free_list[level]->prev = block;
	}
	memory->free_list[level] = block;
}

static void remove_block_from_free_list(BuddyBlock *block) {
	MemoryManagerADT memory = get_memory_manager();
	int level = block->level;

	if (block->prev != NULL) {
		block->prev->next = block->next;
	}
	else {
		memory->free_list[level] = block->next;
	}

	if (block->next != NULL) {
		block->next->prev = block->prev;
	}

	block->next = block->prev = NULL;
}

void free_buddy_memory(void *ptr) {
	if (ptr == NULL) {
		return; // Nothing to free
	}

	BuddyBlock *block = (BuddyBlock *) ((uint64_t) ptr - sizeof(BuddyBlock));

	if (block->is_free) {
		draw_with_color("Error: Double free", 0xFF0000);
		return;
	}

	block->is_free = 1;

	merge_block(block);
}

void get_memory_info_buddy(char *type, uint64_t *free, uint64_t *allocated, uint64_t *total) {
	MemoryManagerADT memory = get_memory_manager();
	my_strcpy(type, "Buddy memory");

	*free = 0;
	*allocated = 0;

	for (int i = 0; i < memory->max_levels; i++) {
		BuddyBlock *block = memory->free_list[i];
		uint64_t level_free = 0;
		while (block != NULL) {
			level_free += (1 << i);
			block = block->next;
		}
		*free += level_free;
	}

	BuddyBlock *block = (BuddyBlock *) memory->start_address;
	while ((uint64_t) block < (uint64_t) memory->start_address + memory->size) {
		if (block->is_free) {
			*free += (1 << block->level);
		}
		block = (BuddyBlock *) ((uint64_t) block + (1 << block->level));
	}

	*allocated = memory->size - *free;
	*total = memory->size;
}

#endif