#ifdef BUDDY_SYSTEM

#include "buddy.h"
#include <stddef.h>
#include <stdint.h>

#define MAX_LEVELS 12  // Define los niveles de potencia de dos (por ejemplo, de 4 KB a 1 MB)

static BuddyBlock *free_list[MAX_LEVELS];
static void *buddy_base_address;

// Redondea el tamaño al siguiente múltiplo de 2^12 (4 KB)
static uint64_t round_up_to_power_of_two(uint64_t size) {
    uint64_t rounded_size = 4096;  // Tamaño mínimo de bloque es 4 KB
    while (rounded_size < size) {
        rounded_size <<= 1;
    }
    return rounded_size;
}

void init_buddy_system(uint64_t size, void *start_address) {
    drawWord("Buddy System");
    buddy_base_address = start_address;
    for (int i = 0; i < MAX_LEVELS; i++) {
        free_list[i] = NULL;
    }

    int initial_level = MAX_LEVELS - 1;  // Nivel más grande
    free_list[initial_level] = (BuddyBlock *)start_address;
    free_list[initial_level]->next = NULL;
    free_list[initial_level]->level = initial_level;
}

void *alloc_buddy_memory(uint64_t size) {
    size = round_up_to_power_of_two(size);
    int level = 0;
    while ((1 << (level + 12)) < size) {  // Encuentra el nivel adecuado (12 porque 2^12 = 4096)
        level++;
    }

    if (level >= MAX_LEVELS) {
        
        return NULL;  // Tamaño solicitado demasiado grande
    }

    for (int i = level; i < MAX_LEVELS; i++) {
        if (free_list[i] != NULL) {
            BuddyBlock *block = free_list[i];
            free_list[i] = block->next;

            // Divide el bloque en bloques más pequeños si es necesario
            while (i > level) {
                i--;
                BuddyBlock *buddy = (BuddyBlock *)((uintptr_t)block + (1 << (i + 12)));
                buddy->next = free_list[i];
                buddy->level = i;
                free_list[i] = buddy;
            }

            // Inicializa la memoria antes de devolverla
            my_memset(block, 0, size);
            
            return (void *)block;
        }
    }
   
    return NULL;  // No hay memoria disponible
}


void free_buddy_memory(void *ptr) {
    if (ptr == NULL) return;

    BuddyBlock *block = (BuddyBlock *)ptr;
    int level = block->level;

    uintptr_t buddy_address = (uintptr_t)ptr ^ (1 << (level + 12));
    BuddyBlock *buddy = (BuddyBlock *)buddy_address;

    BuddyBlock **current = &free_list[level];
    while (*current != NULL) {
        if (*current == buddy && buddy->level == level) {
            *current = buddy->next;
            if (buddy_address < (uintptr_t)ptr) {
                block = buddy;
            }
            level++;
            buddy_address ^= (1 << (level + 12));
            current = &free_list[level];
        } else {
            current = &((*current)->next);
        }
    }

    block->next = free_list[level];
    block->level = level;
    free_list[level] = block;
}


#endif
