//  #ifdef BUDDY
#include <buddy.h>

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


// Función para obtener el manejador de memoria
static MemoryManagerADT get_memory_manager() {
    return (MemoryManagerADT) MM_ADDRESS;
}

// Función para inicializar el sistema de memoria Buddy
void init_buddy_system(uint64_t size, void *start_address) {
    MemoryManagerADT memory = get_memory_manager();
    memory->start_address = start_address;
    memory->size = size;
    memory->max_levels = log2(size);

    if(memory->max_levels > MAX_LEVELS){
        drawWithColor("Error: Size requested is too large", 0xFF0000);
    }else if(memory->max_levels < MIN_LEVELS){
        drawWithColor("Error: Size requested is too small", 0xFF0000);
    }

    for (int i = 0; i < memory->max_levels; i++) {
        memory->free_list[i] = NULL;
    }

    int initial_level = memory->max_levels - 1;
    memory->free_list[initial_level] = create_free_block(start_address, initial_level, NULL, NULL);
}

static BuddyBlock *create_free_block(void *start_address, int level, BuddyBlock *next, BuddyBlock *prev) {
    BuddyBlock *block = (BuddyBlock *)start_address;
    block->level = level;
    block->is_free = 1;
    block->next = next;
    block->prev = prev;
    return block;
}

static BuddyBlock *split_block(BuddyBlock *block) {
    MemoryManagerADT memory = get_memory_manager();
    int level = block->level - 1;

    // Update the level of the original block
    block->level = level;

    // Calculate the address of the buddy
    BuddyBlock *buddy = (BuddyBlock *)((uint64_t)block + (1 << level));

    // Initialize the buddy block
    buddy->level = level;
    buddy->is_free = 1;
    buddy->next = memory->free_list[level];
    buddy->prev = NULL;

    // Update the free list
    if (memory->free_list[level] != NULL) {
        memory->free_list[level]->prev = buddy;
    }
    memory->free_list[level] = buddy;

    // The original block is used for allocation
    return block;
}


static BuddyBlock *find_block_buddy(int level){
    MemoryManagerADT memory = get_memory_manager();

    // Check if the level is within valid bounds
    if(level > memory->max_levels){
        return NULL;
    }

    // Search for a free block from the requested level up to the maximum level
    for(int i = level; i <= memory->max_levels; i++) {
        if(memory->free_list[i] != NULL) {
            // Found a free block at level i
            BuddyBlock *block = memory->free_list[i];

            // Remove the block from the free list
            memory->free_list[i] = block->next;
            if(block->next != NULL){
                block->next->prev = NULL;
            }
            block->next = block->prev = NULL;

            // Split the block down to the desired level
            while(i > level) {
                block = split_block(block);
                i--;
            }

            // Mark the block as allocated
            block->is_free = 0;
            return block;
        }
    }

    // No available block found
    return NULL;
}


void *alloc_buddy_memory(uint64_t size) {
    
    int level = log2(size + sizeof(BuddyBlock));

    if(level < MIN_LEVELS){
        level = MIN_LEVELS;
    } else if(level > MAX_LEVELS){
        drawWithColor("Error: Size requested is too large", 0xFF0000);
        return NULL;
    }
    
    BuddyBlock *block = find_block_buddy(level);
    if(block == NULL){
        drawWithColor("Error: No available blocks", 0xFF0000);
        return NULL;
    }

    // Return a pointer to the memory after the BuddyBlock struct
    return (void*)((uint64_t)block + sizeof(BuddyBlock));
}


static void merge_block(BuddyBlock *block) {
    MemoryManagerADT memory = get_memory_manager();

    int level = block->level;

    // Calculate the address difference to find the buddy
    uint64_t relative_address = (uint64_t)block - (uint64_t)memory->start_address;
    uint64_t buddy_number = relative_address / (1 << level);
    uint64_t buddy_relative_address = 0;

    if (buddy_number % 2 == 0) {
        // If even, buddy is the next block
        buddy_relative_address = relative_address + (1 << level);
    } else {
        // If odd, buddy is the previous block
        buddy_relative_address = relative_address - (1 << level);
    }

    BuddyBlock *buddy = (BuddyBlock *)((uint64_t)memory->start_address + buddy_relative_address);

    // Check if the buddy exists within the memory bounds
    if ((uint64_t)buddy < (uint64_t)memory->start_address || 
        (uint64_t)buddy >= (uint64_t)memory->start_address + memory->size) {
        // Buddy is out of bounds, cannot merge
        add_block_to_free_list(block);
        return;
    }

    // Check if the buddy is free and at the same level
    if (buddy->is_free && buddy->level == level) {
        // Remove buddy from free list
        remove_block_from_free_list(buddy);

        // Determine the new block's address (lower of the two)
        BuddyBlock *merged_block = (block < buddy) ? block : buddy;
        merged_block->level = level + 1;

        // Recursively attempt to merge at the next level
        merge_block(merged_block);
    } else {
        // Cannot merge, add the block to the free list
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
    } else {
        // Block is the head of the free list
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

    // Get the block address
    BuddyBlock *block = (BuddyBlock *)((uint64_t)ptr - sizeof(BuddyBlock));

    // Mark the block as free
    block->is_free = 1;

    // Merge the block with its buddy if possible
    merge_block(block);
}


// #endif