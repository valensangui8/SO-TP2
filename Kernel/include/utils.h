#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>

#define MEM_ADDRESS 0x50000
#define SCHEDULER_ADDRESS 0x60000
#define FREE_ 0x9FFFF

#define MEM_ADDRESS_MAX 0x9FFFF

typedef enum { BLOCKED = 0, READY, RUNNING, ZOMBIE, DEAD
                } PCBStatus;

typedef enum { PRIORITY0 = 0, PRIORITY1, PRIORITY2, PRIORITY3, PRIORITY4
                } Priority;


#endif