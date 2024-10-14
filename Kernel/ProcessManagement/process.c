#include <process.h>

static void printHex(uint64_t value) {
	char buffer[17];
	for (int i = 15; i >= 0; i--) {
		buffer[i] = "0123456789ABCDEF"[value % 16];
		value /= 16;
	}
	buffer[16] = '\0';
	drawWord(buffer); // Supongo que drawWord imprime en pantalla
}

void init_process(PCBT *process, char *name, uint16_t pid, uint16_t ppid, Priority priority, PCBState state, char foreground, char **argv, int argc, main_function rip) {
	process->pid = pid;
	process->ppid = ppid;
	process->priority = priority;
	process->state = state;
	process->foreground = foreground;
	// process->is_active = 1;
	my_strncpy(process->name, name, sizeof(process->name));
	process->stack_process = load_stack_pointer(rip, 0, argv, argc);
	process->argv = argv;
	process->argc = argc;
}

uint8_t has_children(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].is_active != 0 && scheduler->processes[i].ppid == pid) {
			return 1;
		}
	}
	return 0;
}

void wait_children(unsigned int ppid) {
	if (has_children(ppid)) {
		SchedulerInfo scheduler = get_scheduler();
		for (int i = 0; i < MAX_PROCESS; i++) {
			if (scheduler->processes[i].is_active == 1 && scheduler->processes[i].ppid == ppid) {
				while (scheduler->processes[i].state != DEAD) {
					// wait
				}
			}
		}
	}
}

uint8_t kill_process(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].is_active == 1 && scheduler->processes[i].pid == pid) {
			scheduler->processes[i].state = DEAD;
			scheduler->processes[i].is_active = 0;
			scheduler->amount_processes--;
			return 1;
		}
	}
	return 0;
}

void update_priority(unsigned int pid, Priority new_priority) {
	SchedulerInfo scheduler = get_scheduler();
	PCBT *process = NULL;

	// Buscar el proceso con el pid dado
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].is_active == 1 && scheduler->processes[i].pid == pid) {
			process = &(scheduler->processes[i]);
			break;
		}
	}

	// Si no se encuentra el proceso, no hacemos nada
	if (process == NULL) {
		drawWord("Proceso no encontrado");
		return;
	}

	int old_priority = process->priority;

	// Si la prioridad no cambia, no hacemos nada
	if (old_priority == new_priority) {
		drawWord("La prioridad ya es la misma");
		return;
	}

	if (new_priority > old_priority) {
		int additional_slots = new_priority - old_priority;
		int inserted = 0;

		// Insert additional slots for pointers in round robin
		for (int i = 0; i < MAX_PROCESS * PRIORITY4 && inserted < additional_slots; i++) {
			if (scheduler->round_robin[i] == NULL) {
				scheduler->round_robin[i] = process;
				inserted++;
			}
		}

		drawWord("Prioridad aumentada correctamente");
	}
	else if (new_priority < old_priority) {
		int to_remove = old_priority - new_priority;
		int removed = 0;

		// Remove the extra slot pointers from round robin
		for (int i = 0; i < MAX_PROCESS * PRIORITY4 && removed < to_remove; i++) {
			if (scheduler->round_robin[i] == process) {
				scheduler->round_robin[i] = NULL;
				removed++;
			}
		}

		drawWord("Prioridad reducida correctamente");
	}

	process->priority = new_priority;
}

uint16_t block_process(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].pid == pid && scheduler->processes[i].is_active == 1) {
			// Depuración: Verificar el estado del proceso antes de bloquearlo
			if (scheduler->processes[i].state == DEAD || scheduler->processes[i].state == BLOCKED) {
				drawWord("block_process: Proceso en estado inválido para bloquear");
				enter();
				return 0;
			}
			scheduler->processes[i].state = BLOCKED;
			return 1;
		}
	}
	drawWord("block_process: Proceso no encontrado");
	enter();
	return 0;
}

uint16_t unblock_process(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].is_active == 1 && scheduler->processes[i].pid == pid) {
			scheduler->processes[i].state = READY;
			return 1;
		}
	}
	return 0;
}

void yield() {
	SchedulerInfo scheduler = get_scheduler();
	scheduler->quantum_remaining = 0;
}

void process_status(int pid) {
	SchedulerInfo scheduler = get_scheduler();
	drawWord("NAME    ID     RSP      RBP    	STATE");
	enter();
	for (int i = 0; i < scheduler->amount_processes; i++) {
		if (scheduler->processes[i].pid == pid) {
			drawWord(scheduler->processes[i].name);
			drawWord("     ");
			drawInt(scheduler->processes[i].pid);
			drawWord("    ");
			drawHex(scheduler->processes[i].stack_process->rsp);
			drawWord("   ");
			drawHex(scheduler->processes[i].stack_process->my_registers.rbp);
			drawWord("   ");
			drawWord(process_state(scheduler->processes[i].state, scheduler->processes[i].priority, scheduler->processes[i].foreground));
		}
	}
	enter();
}

char *process_state(PCBState state, Priority priority, char foreground) {
	static char status[10];
	my_strcpy(status, "");

	// Base state
	switch (state) {
		case BLOCKED:
			my_strcat(status, "T");
			break;
		case READY:
			my_strcat(status, "S");
			break;
		case RUNNING:
			my_strcat(status, "R");
			break;
		case ZOMBIE:
			my_strcat(status, "Z");
			break;
		default:
			my_strcat(status, "UNKNOWN");
			return status;
	}
	if (priority == PRIORITY4) {
		my_strcat(status, "<"); // High priority
	}
	else if (priority == PRIORITY1) {
		my_strcat(status, "N"); // Low priority
	}
	if (foreground) {
		my_strcat(status, "+"); // Foreground process
	}
	my_strcat(status, "\0");
	return status;
}
