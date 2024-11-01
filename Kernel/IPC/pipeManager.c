#include <pipeManager.h>

#define MAX_PIPES 12
#define bufferPosition(pipe) (((pipe)->startPosition + (pipe)->currentSize) % PIPE_SIZE)


typedef struct Pipe {
    char buffer[PIPE_SIZE];
    uint16_t write_index;
    uint16_t read_index;
    int16_t input_pid, output_pid;
    uint8_t is_blocking;
} Pipe;

struct PipeManagerCDT {
	Pipe *pipes[MAX_PIPES];
	uint16_t next_free_pipe;
	uint16_t pipes_used;
};

static Pipe *createPipe();
static void freePipe(Pipe *pipe);
void closePipe();
uint16_t getPipe();



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

Pipe *create_pipe() {
    Pipe *pipe = (Pipe *) malloc(sizeof(Pipe));
    if (pipe == NULL) {
        return NULL;
    }
    pipe->index = 0;
    pipe->input_pid = -1;
    pipe->output_pid = -1;
    pipe->is_blocking = 0;
    for(int i = 0; i < PIPE_SIZE; i++) {
        pipe->buffer[i] = 0;
    }
    return pipe;
}
// {0, 1, 2} ocupados {3, 4, 5, 6, 7, 8, 9, 10} --> {0, 1, 2, }


static Pipe *get_pipe(int id){
    PipeManagerADT pipe_manager = get_pipe_manager();
    if (id < 0 || id >= MAX_PIPES) {
        return NULL;
    }
    if(pipe_manager->pipes[id - BUILT_IN_FD] == NULL){
        pipe_manager->pipes[id - BUILT_IN_FD] = create_pipe();
        
    }
    return pipe_manager->pipes[id - BUILT_IN_FD];	
}

uint16_t get_pipe_fd(int id){
    Pipe *pipe = get_pipe(id);
    for(int i = 0; i < MAX_PIPES; i++){
        if(pipe == ergerg){
            return i;
        }
    }

}



uint16_t get_free_pipe(){
    PipeManagerADT pipe_manager = get_pipe_manager();
    if (pipe_manager->pipes_used == MAX_PIPES) {
        return -1;
    }
    Pipe *pipe = create_pipe();
    if (pipe == NULL) {
        return -1;
    }
    pipe_manager->pipes[pipe_manager->next_free_pipe] = pipe;
    uint16_t pipe_id = pipe_manager->next_free_pipe;
    pipe_manager->pipes_used++;
    pipe_manager->next_free_pipe = next_free_pipe();
    return pipe_id + BUILT_IN_FD;
}

uint16_t next_free_pipe() {
    PipeManagerADT pipe_manager = get_pipe_manager();
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipe_manager->pipes[i] == NULL) {
            return i;
        }
    }
    return -1;
}
