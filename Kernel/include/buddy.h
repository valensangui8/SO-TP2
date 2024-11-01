#ifndef _BUDDY_H
#define _BUDDY_H

#include <stddef.h>
#include <stdint.h>

#define MAX_LEVELS 12  // Número de niveles de memoria (por ejemplo, de 4096 bytes a 1 MB)

typedef struct BuddyBlock {
    struct BuddyBlock *next;
    int level;
} BuddyBlock;

void init_buddy_system(uint64_t size, void *start_address);
void *alloc_buddy_memory(uint64_t size);
void free_buddy_memory(void *ptr);

#endif
