#ifndef _PROCESS_H
#define _PROCESS_H


#include <stdint.h>
#include <utils.h>

#define DESCRIPTORS 3


typedef struct PCB {
    char name[32];
    uint16_t pid;
    uint16_t ppid;
    Priority priority;
    PCBStatus status;
    int16_t file_descriptors[DESCRIPTORS];
    void * stack_base;
    void * stack_pointer;
} PCBT;

void init_procces(PCBT * process, char * name, uint16_t pid, uint16_t ppid, Priority priority, PCBStatus status, int16_t file_descriptors[], void * stack_base, void * stack_pointer);



#endif
