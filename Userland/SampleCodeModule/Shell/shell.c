// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shell.h>

typedef struct command {
	char *name;
	union {
		int (*process_params)(int16_t fds[], int argc, char **argv);
		int (*process_no_params)(int16_t fds[]);
		void (*function_void)();
		void (*function_params)(int argc, char **argv);
	};
	Type_t type;
} Command;

// Define si BACKGROUND es 0 o 1 según tu sistema
Command commands_list[COMMANDS] = {
	{"zoomIn", .function_void = zoom_in, VOID},
	{"zoomOut", .function_void = zoom_out, VOID},
	{"clear", .function_void = clear, VOID},
	{"div0", .function_void = div0, VOID},
	{"invalidOpcode", .function_void = invalidOpcode, VOID},
	{"help", .function_void = help, VOID},
	{"registers", .function_void = registers, VOID},
	{"date", .function_void = date, VOID},
	{"eliminator", .function_void = eliminator, VOID},
	{"itba", .function_void = printLogo, VOID},
	{"testprio", .process_no_params = test_prio_user, PROCESS_NO_PARAMS},
	{"testmm", .process_params = test_mm_user, PROCESS_PARAMS},
	{"testprocess", .process_params = test_process_user, PROCESS_PARAMS},
	{"ps", .function_void = ps, VOID},
	{"kill", .function_params = kill_process, FUNC_PARAMS},
	{"testsync", .process_params = test_sync_user, PROCESS_PARAMS},
	{"cat", .process_no_params = cat, PROCESS_NO_PARAMS},
	{"filter", .process_no_params = filter, PROCESS_NO_PARAMS},
	{"wc", .process_no_params = wc, PROCESS_NO_PARAMS},
	{"loop", .process_params = loop, PROCESS_PARAMS},
	{"nice", .function_params = nice, FUNC_PARAMS},
	{"block", .function_params = block_process, FUNC_PARAMS},
	{"mem", .function_void = get_memory_info, VOID},
	{"text", .process_no_params = text, PROCESS_NO_PARAMS},
	{"philo", .process_no_params = philo, PROCESS_NO_PARAMS},
	{"killphilo", .function_void = kill_philosophers, VOID}};

static int run_command(char *command, int argc, char **argv, char *flag, int16_t fds[]);

void initialize_shell(char *command, int argc, char **argv, char *command2, int argc2, char **argv2, int background) {
	char flag = 0;
	int16_t fds1[3], fds2[3];
	int pid1, pid2;
	if (argc2 == 0) { // one process
		fds1[STDIN] = (background) ? DEV_NULL : STDIN;
		fds1[STDOUT] = STDOUT;
		fds1[STDERR] = STDERR;
		pid1 = run_command(command, argc, argv, &flag, fds1);
		if (!background) {
			call_sys_wait_children(pid1);
		}
		return;
	}

	int16_t pipe_fd = call_sys_get_pipe_fd();
	if (pipe_fd == -1) {
		call_sys_error("Error: Pipe failed", STDERR);
		call_sys_enter();
		return;
	}

	fds1[STDIN] = (background) ? DEV_NULL : STDIN;
	fds1[STDOUT] = pipe_fd;
	fds1[STDERR] = STDERR;

	fds2[STDIN] = pipe_fd;
	fds2[STDOUT] = STDOUT;
	fds2[STDERR] = STDERR;

	pid1 = run_command(command, argc, argv, &flag, fds1);
	pid2 = run_command(command2, argc2, argv2, &flag, fds2);
	if (!background) {
		call_sys_wait_children(pid1);
		call_sys_wait_children(pid2);
	}
}

int run_command(char *command, int argc, char **argv, char *flag, int16_t fds[]) {
	if (*command == 0) {
		call_sys_enter();
		return -1;
	}
	int id = read_command(command);
	int pid = 0;
	execute_command(id, flag, command, argc, argv, fds, &pid);
	return pid;
}

int read_command(char *command) {
	for (int i = 0; i < COMMANDS; i++) {
		if (strcmp(command, commands_list[i].name) == 0) {
			return i;
		}
	}
	return -1;
}

void execute_command(int index, char *flag, char *command, int argc, char **argv, int16_t fds[], int *pid) {
	if (index == -1) {
		call_sys_command_enter();
		call_sys_error("ERROR: Command not found: ", STDERR);
		call_sys_error(command, STDERR);
		call_sys_enter();
		*flag = 0;
		return;
	}
	call_sys_command_enter();
	char HeightPassed = 0;
	call_sys_check_height(&HeightPassed, index);
	if (HeightPassed == 1) {
		call_sys_clear();
	}

	switch (commands_list[index].type) {
		case PROCESS_PARAMS:
			if (argc == 1) {
				call_sys_error("ERROR: Invalid number of arguments", STDERR);
				call_sys_enter();
				*flag = 0;
				return;
			}
			*pid = commands_list[index].process_params(fds, argc, argv);
			break;
		case PROCESS_NO_PARAMS:
			*pid = commands_list[index].process_no_params(fds);
			break;
		case FUNC_PARAMS:
			commands_list[index].function_params(argc, argv);
			call_sys_enter();
			break;
		case VOID:
			commands_list[index].function_void();
			call_sys_enter();
			break;
	}
	*flag = 1;
}
