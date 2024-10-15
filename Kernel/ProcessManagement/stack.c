#include <stack.h>

// Función del proceso
// void process_function(main_function rip, char **argv, uint64_t argc) {
//     // Execute the main function of the process
//     int ret = rip(argc, argv);

//     // Clean up the process (e.g., mark it as DEAD)
//     kill_process(get_pid());

//     // Yield CPU to scheduler
//     yield();

//     // Halt to prevent further execution
//     while (1) {
//         _hlt();
//     }
// }


// Process_stack load_stack_pointer(main_function rip, uint64_t rsp, char **argv, uint64_t argc) {
// 	Process_stack my_stack = (Process_stack) alloc_memory(sizeof(stack));
// 	if(my_stack == NULL) {
// 		drawWord("load_stack_pointer: my_stack == NULL");
// 		enter();
// 		return NULL;
// 	}
// 	my_stack->rip = &process_function;
// 	my_stack->rsp = (uint64_t) (my_stack + sizeof(stack));

// 	my_stack->cs = 0x8;
// 	my_stack->eflags = 0x202;
// 	my_stack->ss = 0x0;
// 	my_memset(&my_stack->my_registers, 0, sizeof(Stack_registers));
// 	my_stack->my_registers.rdi = (uint64_t) rip;
// 	my_stack->my_registers.rsi = (uint64_t) argv;
// 	my_stack->my_registers.rdx = (uint64_t) argc;
	

// 	return my_stack;
// }

// Process_stack load_stack_pointer(main_function rip, uint64_t rsp, char **argv, uint64_t argc) {
//     uint64_t stack_size = 0x4000; // 16KB stack
//     uint8_t *process_stack = (uint8_t *)alloc_memory(stack_size);
//     if (process_stack == NULL) {
//         return NULL;
//     }

//     uint64_t initial_rsp = (uint64_t)(process_stack + stack_size);

//     // Push rip, cs, and rflags (in that order)
//     initial_rsp -= sizeof(uint64_t);
//     *(uint64_t *)initial_rsp = 0x202; // rflags with IF set

//     initial_rsp -= sizeof(uint64_t);
//     *(uint64_t *)initial_rsp = 0x08; // cs (code segment selector)

//     initial_rsp -= sizeof(uint64_t);
//     *(uint64_t *)initial_rsp = (uint64_t)process_function; // rip (entry point)

//     // Push general-purpose registers
//     initial_rsp -= sizeof(Stack_registers);
//     Stack_registers *regs = (Stack_registers *)initial_rsp;
//     my_memset(regs, 0, sizeof(Stack_registers));

//     // Initialize registers
//     regs->rax = 0;
//     regs->rbx = 0;
//     regs->rcx = 0;
//     regs->rdx = (uint64_t)argc;  // Third argument
//     regs->rbp = 0;
//     regs->rdi = (uint64_t)rip;   // First argument
//     regs->rsi = (uint64_t)argv;  // Second argument
//     // Initialize other registers as needed

//     return (Process_stack)initial_rsp;
// }



// Process_stack get_snapshot(Process_stack current_Stack, uint64_t *Stack_pointer) {
// 	if (current_Stack == NULL || Stack_pointer == NULL) {
// 		return NULL;
// 	}
// 	current_Stack->rsp = (uint64_t) Stack_pointer;

// 	return current_Stack;
// }
