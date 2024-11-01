#ifndef MEMORY_H
#define MEMORY_H

#ifdef BUDDY_SYSTEM
#include "buddy.h"
#define init_memory_manager init_buddy_system
#define alloc_memory alloc_buddy_memory
#define free_memory free_buddy_memory
#else
#include "memoryManagerADT.h"
#define init_memory_manager init_memory_manager_ADT
#define alloc_memory alloc_memory_ADT
#define free_memory free_memory_ADT
#endif

#endif // MEMORY_H
