#ifndef MEMORY_H
#define MEMORY_H

#ifdef BUDDY
#include "buddy.h"
#define init_memory_manager init_buddy_system
#define alloc_memory alloc_buddy_memory
#define free_memory free_buddy_memory
#define get_memory_info get_memory_info_buddy
#else
#include "memoryManagerADT.h"
#define init_memory_manager init_memory_manager_ADT
#define alloc_memory alloc_memory_ADT
#define free_memory free_memory_ADT
#define get_memory_info get_memory_info_ADT
#endif

#endif // MEMORY_H
