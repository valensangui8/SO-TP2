#include <pipeManager.h>

#define MAX_PIPES 12
#define bufferPosition(pipe) (((pipe)->startPosition + (pipe)->currentSize) % PIPE_SIZE)


typedef struct Pipe {
    char buffer[PIPE_SIZE];
    uint16_t write_index;
    uint16_t read_index;
    int16_t input_pid, output_pid;
    int64_t sem_write;
    int64_t sem_read;
} Pipe;

struct PipeManagerCDT {
	Pipe *pipes[MAX_PIPES];
	uint16_t next_free_pipe;
	uint16_t pipes_used;
};

static PipeManagerADT get_pipe_manager() {
	return (PipeManagerADT) PIPE_MANAGER_ADDRESS;
}

PipeManagerADT create_pipe_manager() {
    PipeManagerADT pipe_manager = get_pipe_manager();
    pipe_manager->next_free_pipe = 0;
    pipe_manager->pipes_used = 0;
    for (int i = 0; i < MAX_PIPES; i++) {
        pipe_manager->pipes[i] = NULL;
    }
    return pipe_manager;
}

static char *name_pipe_sem(int fd, char mode) {
    char *name = (char *) alloc_memory(10); // CAMBIAR DESPUES
    if (name == NULL) {
        return NULL;
    }
    my_strcpy(name, "pipe_");
    char *fd_str = my_itoa(fd);
    if (fd_str == NULL) {
        free_memory(name);
        return NULL;
    }
    my_strcat(name, fd_str);
    return name;
}

static Pipe *create_pipe(uint64_t id) {
    Pipe *pipe = (Pipe *) alloc_memory(sizeof(Pipe));
    if (pipe == NULL) {
        return NULL;
    }
    pipe->read_index = 0;
    pipe->write_index = 0;
    pipe->input_pid = -1;
    pipe->output_pid = -1;
    for(int i = 0; i < PIPE_SIZE; i++) {
        pipe->buffer[i] = 0;
    }

    pipe->sem_write = sem_open(name_pipe_sem(id,'w'), 0);
    pipe->sem_read = sem_open(name_pipe_sem(id,'r'), 0);

    return pipe;
}

static Pipe *get_pipe(int id){
    PipeManagerADT pipe_manager = get_pipe_manager();
    if (id < 0 || id >= MAX_PIPES || id < BUILT_IN_FD) {
        return NULL;
    }
    if(pipe_manager->pipes[id - BUILT_IN_FD] == NULL){
        pipe_manager->pipes[id - BUILT_IN_FD] = create_pipe(id);
        
    }
    return pipe_manager->pipes[id - BUILT_IN_FD];	
}

int16_t get_pipe_fd() {
    PipeManagerADT pipe_manager = get_pipe_manager();
    if (pipe_manager->pipes_used == MAX_PIPES) {
        return -1;
    }
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipe_manager->pipes[i] == NULL) {
            pipe_manager->pipes[i] = create_pipe(i + BUILT_IN_FD);
            return i + BUILT_IN_FD;
        }
    }
    return -1;
}

static int16_t open_pipe_pid(int id, char mode, int16_t pid) {
    Pipe *pipe = get_pipe(id);
    if (pipe == NULL) {
        return -1;
    }
    if (mode == 'r') {
        if (pipe->input_pid != -1) {
            return -1;
        }
        pipe->input_pid = pid;
    } else if (mode == 'w') {
        if (pipe->output_pid != -1) {
            return -1;
        }
        pipe->output_pid = pid;
    } else {
        return -1;
    }
    return id;
}

int16_t open_pipe(int id, char mode) {
    return open_pipe_pid(id, mode, get_pid());
}

static void free_pipe(Pipe *pipe) {
    sem_close(pipe->sem_write);
    sem_close(pipe->sem_read);
    free_memory(pipe);

    PipeManagerADT pipe_manager = get_pipe_manager();
    pipe_manager->pipes_used--;
    for(int i = 0; i < MAX_PIPES; i++){
        if(pipe_manager->pipes[i] == pipe){
            pipe_manager->pipes[i] = NULL;
        }
    }
}

static int16_t close_pipe_pid(int id, int16_t pid) {
    Pipe *pipe = get_pipe(id);
    if (pipe == NULL) {
        return -1;
    }
    if (pipe->input_pid == pid) {
        pipe->input_pid = -1;
    } else if (pipe->output_pid == pid) {
        pipe->output_pid = -1;
    } else { // The process is not using the pipe
        return -1;
    }

    if (pipe->input_pid == -1 && pipe->output_pid == -1) {
        free_pipe(pipe);
    }

    return 0;
}

int16_t close_pipe(uint16_t fd) {
    return close_pipe_pid(fd, get_pid());
}

int16_t write_pipe(uint16_t fd, char *buffer, uint16_t *count) {
    Pipe *pipe = get_pipe(fd);
    int len = my_strlen(buffer);
    *count = 0;
    if (pipe == NULL) {
        return -1;
    }
    if (pipe->output_pid != get_pid()) {
        return -1;
    }
    for (int i = 0; i < len; i++) {
        sem_wait(pipe->sem_write);
        pipe->buffer[pipe->write_index] = buffer[i];
        pipe->write_index = (pipe->write_index + 1) % PIPE_SIZE;
        *count++;
        sem_post(pipe->sem_read);
    }
    
    return count;
}

int16_t read_pipe(uint16_t fd, char *buffer, uint16_t *count) {
    Pipe *pipe = get_pipe(fd);
    *count = 0;
    if (pipe == NULL) {
        return -1;
    }
    if (pipe->input_pid != get_pid()) {
        return -1;
    }
    for (int i = 0; pipe->read_index != pipe->read_index; i++) {
        sem_wait(pipe->sem_read);
        buffer[i] = pipe->buffer[pipe->read_index];
        pipe->read_index = (pipe->read_index + 1) % PIPE_SIZE;
        *count++;
        sem_post(pipe->sem_write);
    }
    return count;
}