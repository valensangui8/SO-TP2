#include <process.h>


static void printHex(uint64_t value) {
    char buffer[17];
    for (int i = 15; i >= 0; i--) {
        buffer[i] = "0123456789ABCDEF"[value % 16];
        value /= 16;
    }
    buffer[16] = '\0';
    drawWord(buffer);  // Supongo que drawWord imprime en pantalla
}

void init_process(PCBT *process, char *name, uint16_t pid, uint16_t ppid, Priority priority, PCBState state, char foreground, char **argv, int argc, main_function rip) {
	process->pid = pid;
	process->ppid = ppid;
	process->priority = priority;
	process->state = state;
	process->foreground = foreground;
	//process->is_active = 1;
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
            process = &scheduler->processes[i];
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

    // Si la nueva prioridad es mayor, agregamos punteros adicionales
    if (new_priority > old_priority) {
        int additional_slots = new_priority - old_priority; // Cantidad de punteros adicionales necesarios
        int inserted = 0;

        // Insertar punteros adicionales en el round robin
        for (int i = 0; i < MAX_PROCESS * PRIORITY4 && inserted < additional_slots; i++) {
            if (scheduler->round_robin[i] == NULL) {
                scheduler->round_robin[i] = process;
                inserted++;
            }
        }

        drawWord("Prioridad aumentada correctamente");
    }
    // Si la nueva prioridad es menor, removemos punteros del round robin
    else if (new_priority < old_priority) {
        int to_remove = old_priority - new_priority; // Cantidad de punteros a remover
        int removed = 0;

        // Eliminar punteros del round robin
        for (int i = 0; i < MAX_PROCESS * PRIORITY4 && removed < to_remove; i++) {
            if (scheduler->round_robin[i] == process) {
                scheduler->round_robin[i] = NULL;
                removed++;
            }
        }

        drawWord("Prioridad reducida correctamente");
    }

    // Finalmente, actualizar la prioridad en el PCB del proceso
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


