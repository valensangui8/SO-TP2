#include <stdint.h>
#include <process.h>



void init_process(PCBT * process, char * name, uint16_t pid, uint16_t ppid, Priority priority, PCBStatus status, int16_t file_descriptors[], void * stack_base, void * stack_pointer){
    process->pid = pid;
    process->ppid = ppid;
    process->priority = priority;
    process->status = status;
    process->stack_base = stack_base;
    process->stack_pointer = stack_pointer;
    for (int i = 0; i < DESCRIPTORS; i++){
        process->file_descriptors[i] = file_descriptors[i];
    }
    for (int i = 0; i < 32; i++){  // revisar como ponemos el nombre
        process->name[i] = name[i];
    }

}
