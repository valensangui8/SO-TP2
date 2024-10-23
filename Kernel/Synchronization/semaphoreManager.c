#include <semaphoreManager.h>

struct MySem{
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

SemaphoresADT create_semaphores() {
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
    for(int i = 0; i < MAX_SEMAPHORE; i++){
        if(semaphoreADT->semaphores[i] == NULL){
            semaphoreADT->semaphores[i] = sem;
            semaphoreADT->semaphores[i]->waiting_processes = create_linked_list(); 
            semaphoreADT->size++;
            return i;
        }
    }
    return -1;
}

int delete_semaphore(MySem_t sem) {
    SemaphoresADT semaphoreADT = get_semaphoresADT();
    semaphoreADT->semaphores[sem->id] = NULL;
    semaphoreADT->size--;
    return 0;
}

int is_created(char *sem_id) {
    SemaphoresADT semaphoreADT = get_semaphoresADT();
    for(int i = 0; i < MAX_SEMAPHORE; i++){
        if(my_strcmp(semaphoreADT->semaphores[i]->name, sem_id) == 0){
            return i;
        }
    }
    return -1;
}

int64_t sem_open(char *sem_id, uint64_t initial_value) {
    if ( is_created(sem_id) != -1 ) {
        return 0;
    }

    MySem_t sem = (MySem_t ) alloc_memory((sizeof(MySem_t)));
    sem->mutex = 1;
    acquire(&(sem->mutex));
    sem->name = (char *) alloc_memory(my_strlen(sem_id) + 1);
	my_strcpy(sem->name, sem_id);
    sem->value = initial_value;
    release(&(sem->mutex));
    int id;
    if ( (id = add_semaphore(sem)) != -1 ) {
        sem->id = id;
    } else {
        return -1;
    }
    return 0;
}

int64_t sem_wait(char *sem_id) {
    SemaphoresADT semaphoresADT = get_semaphoresADT();
    int id = is_created(sem_id);
    if(id == -1){
        return -1;
    }
    MySem_t sem = semaphoresADT->semaphores[id];
    sem->waiting_processes = append_element(sem->waiting_processes, (void *) get_pid());
    set_state(BLOCKED);
    acquire(&(sem->mutex));
    while(sem->value <= 0){
        release(&(sem->mutex));
        acquire(&(sem->mutex));
    }
    sem->value -= 1;
    release(&(sem->mutex));
    return 0;
}

int64_t sem_post(char *sem_id) {
    SemaphoresADT semaphoresADT = get_semaphoresADT();
     int id = is_created(sem_id);
    if(id == -1){
        return -1;
    }
    MySem_t sem = semaphoresADT->semaphores[id];
    remove_node(sem->waiting_processes, (void *) get_pid());
    set_state(READY);
    acquire(&(sem->mutex));
    semaphoresADT->semaphores[id]->value += 1;
    release(&(sem->mutex));
    return 0;
}

int64_t sem_close(char *sem_id) {
    SemaphoresADT semaphoresADT = get_semaphoresADT();
    int id = is_created(sem_id);
    if ( id == -1 ) {
        return -1;
    }
    MySem_t sem = semaphoresADT->semaphores[id];
    acquire(&(sem->mutex));
    delete_semaphore(sem);
    free_memory(sem->name);
    free_memory(sem);
    return 0;
}


