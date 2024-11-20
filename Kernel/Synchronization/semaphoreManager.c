// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <semaphoreManager.h>

struct MySem {
	int id;
	char *name;
	uint64_t value;
	uint8_t mutex;
	LinkedListADT waiting_processes;
};

struct SemaphoresCDT {
	MySem_t semaphores[MAX_SEMAPHORE];
	int size;
};

SemaphoresADT create_semaphoreADT() {
	SemaphoresADT semaphores = (SemaphoresADT) SEMAPHORE_MANAGER_ADDRESS;
	for (int i = 0; i < MAX_SEMAPHORE; i++) {
		semaphores->semaphores[i] = NULL;
	}
	semaphores->size = 0;
	return semaphores;
}

SemaphoresADT get_semaphoresADT() {
	return (SemaphoresADT) SEMAPHORE_MANAGER_ADDRESS;
}

MySem_t get_semaphore(char *sem_id) {
	SemaphoresADT semaphores = get_semaphoresADT();
	for (int i = 0; i < MAX_SEMAPHORE; i++) {
		if (semaphores->semaphores[i] != NULL && my_strcmp(semaphores->semaphores[i]->name, sem_id) == 0) {
			return semaphores->semaphores[i];
		}
	}
	return NULL;
}

int add_semaphore(MySem_t sem) {
	SemaphoresADT semaphoreADT = get_semaphoresADT();
	for (int i = 0; i < MAX_SEMAPHORE; i++) {
		if (semaphoreADT->semaphores[i] == NULL) {
			semaphoreADT->semaphores[i] = sem;
			semaphoreADT->semaphores[i]->waiting_processes = create_linked_list();
			semaphoreADT->size++;
			return i;
		}
	}
	return -1;
}

static void delete_semaphore(MySem_t sem) {
	SemaphoresADT semaphoreADT = get_semaphoresADT();
	semaphoreADT->semaphores[sem->id] = NULL;
	semaphoreADT->size--;
	free_memory(sem->name);
	free_linked_list(sem->waiting_processes);
	free_memory(sem);
}

int create_sem(char *sem_id, uint64_t initial_value) {
	MySem_t sem = (MySem_t) alloc_memory(sizeof(struct MySem));
	if (sem == NULL) {
		return -1;
	}
	sem->mutex = 1;
	sem->name = (char *) alloc_memory(my_strlen(sem_id) + 1);
	if (sem->name == NULL) {
		free_memory(sem);
		return -1;
	}
	my_strcpy(sem->name, sem_id);
	sem->value = initial_value;
	int id = add_semaphore(sem);
	if (id == -1) {
		free_memory(sem->name);
		free_memory(sem);
		return -1;
	}
	sem->id = id;
	return id;
}

int64_t sem_open(char *sem_id, uint64_t initial_value) {
	MySem_t existing = get_semaphore(sem_id);
	if (existing != NULL) {
		return existing->id;
	}
	int id = create_sem(sem_id, initial_value);
	return id;
}

int64_t sem_wait(char *sem_id) {
	MySem_t sem = get_semaphore(sem_id);
	if (sem == NULL) {
		draw_with_color("Error: Semaphore not found", 0xFF0000);
		return -1;
	}
	acquire(&(sem->mutex));
	while (sem->value <= 0) {
		append_element(sem->waiting_processes, (void *) (uintptr_t) get_pid());
		block_process(get_pid());
		release(&(sem->mutex));
		yield();
		acquire(&(sem->mutex));
	}
	sem->value -= 1;

	release(&(sem->mutex));
	return 0;
}

int64_t sem_post(char *sem_id) {
	SemaphoresADT semaphoresADT = get_semaphoresADT();
	MySem_t sem = get_semaphore(sem_id);
	if (sem == NULL) {
		return -1;
	}
	acquire(&(sem->mutex));
	semaphoresADT->semaphores[sem->id]->value += 1;
	TNode *next_process = get_first(sem->waiting_processes);
	if (next_process != NULL) {
		delete_element(sem->waiting_processes, (void *) (uintptr_t) get_pid());
		unblock_process((unsigned int) (uintptr_t) next_process->data);
	}
	release(&(sem->mutex));
	return 0;
}

int64_t sem_close(char *sem_id) {
	MySem_t sem = get_semaphore(sem_id);
	if (sem == NULL) {
		return 2;
	}
	if (!is_empty(sem->waiting_processes)) {
		return 1;
	}
	acquire(&(sem->mutex));
	delete_semaphore(sem);
	return 0;
}
