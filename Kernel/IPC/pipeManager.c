// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <pipeManager.h>

#define MAX_PIPES 12
#define bufferPosition(pipe) (((pipe)->startPosition + (pipe)->currentSize) % PIPE_SIZE)

typedef struct Pipe {
	char buffer[PIPE_SIZE];
	uint16_t write_index;
	uint16_t read_index;
	int16_t input_pid, output_pid;
	char readable[10];
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

static Pipe *create_pipe(uint64_t id) {
	Pipe *pipe = (Pipe *) alloc_memory(sizeof(Pipe));
	if (pipe == NULL) {
		draw_with_color("ERROR: Could not allocate memory for pipe", 0xFF0000);
		return NULL;
	}
	pipe->read_index = 0;
	pipe->write_index = 0;
	pipe->input_pid = -1;
	pipe->output_pid = -1;
	for (int i = 0; i < PIPE_SIZE; i++) {
		pipe->buffer[i] = 0;
	}
	return pipe;
}

static Pipe *get_pipe(int id) {
	PipeManagerADT pipe_manager = get_pipe_manager();
	if (id >= MAX_PIPES || id < BUILT_IN_FD) {
		draw_with_color("ERROR: Invalid pipe id", 0xFF0000);
		return NULL;
	}
	if (pipe_manager->pipes[id - BUILT_IN_FD] == NULL) {
		pipe_manager->pipes[id - BUILT_IN_FD] = create_pipe(id);
	}
	return pipe_manager->pipes[id - BUILT_IN_FD];
}

int16_t get_pipe_fd() {
	PipeManagerADT pipe_manager = get_pipe_manager();
	if (pipe_manager->pipes_used == MAX_PIPES) {
		draw_with_color("ERROR: No more pipes available", 0xFF0000);
		return -1;
	}
	for (int i = 0; i < MAX_PIPES; i++) {
		if (pipe_manager->pipes[i] == NULL) {
			Pipe *pipe = create_pipe(i + BUILT_IN_FD);
			if (pipe == NULL) {
				draw_with_color("ERROR: Could not create pipe", 0xFF0000);
				return -1;
			}
			pipe_manager->pipes[i] = pipe;
			pipe_manager->pipes_used++;
			return i + BUILT_IN_FD;
		}
	}
	return -1;
}

int16_t open_pipe(int id, char mode, int pid) {
	Pipe *pipe = get_pipe(id);
	if (pipe == NULL) {
		draw_with_color("ERROR: Pipe not found", 0xFF0000);
		return -1;
	}
	if (mode == 'r') {
		if (pipe->output_pid != -1) {
			draw_with_color("ERROR: Pipe already in use", 0xFF0000);
			return -1;
		}
		pipe->output_pid = pid;
	}
	else if (mode == 'w') {
		if (pipe->input_pid != -1) {
			draw_with_color("ERROR: Pipe already in use", 0xFF0000);
			return -1;
		}
		pipe->input_pid = pid;
		sem_open(my_itoa(pid), 0);
		my_strcpy(pipe->readable, my_itoa(pid));
	}
	else {
		draw_with_color("ERROR: Invalid mode", 0xFF0000);
		return -1;
	}
	return id;
}

static void free_pipe(Pipe *pipe) {
	sem_close(pipe->readable);
	free_memory(pipe);

	PipeManagerADT pipe_manager = get_pipe_manager();
	pipe_manager->pipes_used--;
	for (int i = 0; i < MAX_PIPES; i++) {
		if (pipe_manager->pipes[i] == pipe) {
			pipe_manager->pipes[i] = NULL;
		}
	}
}

int16_t close_pipe(int id, int16_t pid) {
	Pipe *pipe = get_pipe(id);
	if (pipe == NULL) {
		draw_with_color("ERROR: Pipe not found", 0xFF0000);
		return -1;
	}
	if (pipe->input_pid == pid) {
		pipe->input_pid = -1;
	}
	else if (pipe->output_pid == pid) {
		pipe->output_pid = -1;
	}
	else {
		draw_with_color("ERROR: No permission to close pipe", 0xFF0000);
		return -1;
	}

	if (pipe->input_pid == -1 && pipe->output_pid == -1) {
		free_pipe(pipe);
	}

	return 0;
}

int32_t write_pipe(uint16_t fd, char *buffer, uint32_t *count) {
	Pipe *pipe = get_pipe(fd);
	int len = my_strlen(buffer);
	*count = 0;

	if (pipe == NULL) {
		draw_with_color("ERROR: Pipe not found", 0xFF0000);
		return -1;
	}
	if (pipe->input_pid != get_pid()) {
		draw_with_color("ERROR: No permission to write pid", 0xFF0000);
		draw_int(get_pid());
		return -1;
	}
	for (int i = 0; i < len; i++) {
		if ((pipe->write_index + 1) % PIPE_SIZE == pipe->read_index) {
			return -1;
		}
		pipe->buffer[pipe->write_index] = buffer[i];
		pipe->write_index = (pipe->write_index + 1) % PIPE_SIZE;
		(*count)++;
		sem_post(pipe->readable);
	}
	return (int32_t) *count;
}

int32_t read_pipe(uint16_t fd, char *buffer, uint32_t *count) {
	Pipe *pipe = get_pipe(fd);

	*count = 0;
	if (pipe == NULL) {
		draw_with_color("ERROR: Pipe not found!", 0xFF0000);
		return -1;
	}
	if (pipe->output_pid != get_pid()) {
		draw_with_color("ERROR: No permission to read pid", 0xFF0000);
		return -1;
	}
	sem_wait(pipe->readable);
	if (pipe->read_index != pipe->write_index) {
		*buffer = pipe->buffer[pipe->read_index];
		pipe->read_index = (pipe->read_index + 1) % PIPE_SIZE;
		(*count)++;
	}
	return (int32_t) *count;
}
