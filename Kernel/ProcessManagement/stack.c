#include <stack.h>

// Función del proceso
void process_function(main_function rip, char **argv, uint64_t argc) {
    // Ejecutar la función principal del proceso y obtener el valor de retorno
    int ret = rip(argv, argc);

    // Aquí podrías manejar lo que debe pasar cuando el proceso termina
    // Por ejemplo, podrías llamar a una función de "kill" o "exit" para terminar el proceso
    kill_process(get_pid());

    // Bucle infinito para evitar que el proceso siga corriendo accidentalmente
    while (1) {
        _hlt();  // Detener el procesador para evitar el uso de CPU (opcional)
    }
}


Process_stack load_stack_pointer(main_function rip, uint64_t rsp, char **argv, uint64_t argc) {
	Process_stack my_stack = (Process_stack) alloc_memory(sizeof(stack));
	if(my_stack == NULL) {
		drawWord("load_stack_pointer: my_stack == NULL");
		enter();
		return NULL;
	}
	my_stack->rip = &process_function;
	my_stack->rsp = (uint64_t) my_stack + sizeof(stack);

	my_stack->cs = 0x8;
	my_stack->eflags = 0x202;
	my_stack->ss = 0x0;
	my_memset(&my_stack->my_registers, 0, sizeof(Stack_registers));
	my_stack->my_registers.rdi = (uint64_t) rip;
	my_stack->my_registers.rsi = (uint64_t) argv;
	my_stack->my_registers.rdx = (uint64_t) argc;
	

	return my_stack;
}

Process_stack get_snapshot(Process_stack current_Stack, uint64_t *Stack_pointer) {
	if (current_Stack == NULL || Stack_pointer == NULL) {
		return NULL;
	}
	current_Stack->rsp = (uint64_t) Stack_pointer;

	return current_Stack;
}
