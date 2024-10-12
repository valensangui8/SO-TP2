#ifndef _STACK_H
#define _STACK_H

#include <utils.h>

Process_stack load_stack_pointer(uint64_t rip, uint64_t rsp, char ** argv, uint64_t argc);
Process_stack get_snapshot(Process_stack current_stack, uint64_t *stack_pointer);
void process_function(main_function rip, char **argv, uint64_t argc);

#endif
