#ifndef _PROCESS_H
#define _PROCESS_H

#include <interrupts.h>
#include <pipeManager.h>
#include <scheduler.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <utils.h>

#define SESSION_LEADER 2 // shell
#define STACK_SIZE 4096

void init_process(PCBT *process, char *name, uint16_t pid, uint16_t ppid, Priority priority, char **argv, int argc, main_function rip, const int16_t fds[]);
int64_t wait_children(unsigned int pid);
void yield();
void run_process(PCBT *process);
void update_process_stack(void *sp);
void process_status(unsigned int pid);
char *process_state(PCBT process);
unsigned int get_pid();
unsigned int get_ppid();

#endif
