#include <shell.h>

typedef struct command {
    char * name;
    union {
        int (*process_params)(int16_t fds[], int argc, char **argv);
        int (*process_no_params)(int16_t fds[]);
        void (*function_void)();
        void (*function_params)(int argc, char **argv);
    };
    Type_t type;
} Command;

// Define si BACKGROUND es 0 o 1 según tu sistema
Command commandsList[COMMANDS] = {
    {"zoomIn", .function_void = zoomIn, VOID},                          
    {"zoomOut", .function_void = zoomOut, VOID},
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
    {"ps", .process_no_params = ps, VOID},
    {"kill", .function_params = kill_process, FUNC_PARAMS},
    {"testsync", .process_params = test_sync_user, PROCESS_PARAMS}
};

static int run_command(char *command, int argc, char **argv, char *flag, int16_t fds[]);

void initialize_shell(char *command, int argc, char **argv, char *command2, int argc2, char **argv2, int background) {
    char flag = 0;
    int16_t fds1[3], fds2[3];
    if(argc2 == 0){ // one process
        if(background){
            fds1[STDIN] = DEV_NULL;
            fds1[STDOUT]= STDOUT; 
            fds1[STDERR] = STDERR;
        }else{
            fds1[STDIN] = STDIN;
            fds1[STDOUT] = STDOUT; 
            fds1[STDERR] = STDERR;
        }
        run_command(command, argc, argv, &flag, fds1);
        return;
    }

    int16_t pipe_fd = call_sys_get_pipe_fd();
	if (pipe_fd == -1) {
		printf("Error: Pipe failed");
        call_sys_enter();
		return;
	}

    fds1[STDIN] =  (background)? DEV_NULL : STDIN;
    fds1[STDOUT] = pipe_fd;
    fds1[STDERR] = STDERR;

    fds2[STDIN] = pipe_fd;
    fds2[STDOUT] = STDOUT;
    fds2[STDERR] = STDERR;

    int pid = run_command(command, argc, argv, &flag, fds1);
    if(flag == 0 || argc2 == 0){
        return;
    } else{
        run_command(command2, argc2, argv2, &flag, fds2);
    }
    if(!background){
        call_sys_wait_children(pid);
    }
}

int run_command(char *command, int argc, char **argv, char *flag, int16_t fds[]){
    if(*command == 0){
        call_sys_enter();
        return;
    }
    int id = readCommand(command);
    int pid = 0;
    executeCommand(id, flag, command, argc, argv, fds, &pid);
    return pid;
}

int readCommand(char * command) {
    for (int i = 0; i < COMMANDS; i++) {
        if (strcmp(command, commandsList[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

void executeCommand(int index, char * flag, char * command, int argc, char **argv, int16_t fds[], int *pid) {
    if (index == -1 ) {
        call_sys_drawError(command);
        *flag = 0;
        return;
    }
    call_sys_commandEnter();
    char HeightPassed = 0;
    call_sys_checkHeight(&HeightPassed,index);
    if(HeightPassed == 1){
        call_sys_clear();
    }

    call_sys_draw_int(commandsList[index].type);
    call_sys_commandEnter();

    switch(commandsList[index].type){
        case PROCESS_PARAMS:
            if(argc == 1){
                printf("ERROR: Invalid number of arguments");
                call_sys_enter();
                *flag = 0;
                return;
            }
            *pid = commandsList[index].process_params(fds, argc, argv);
            break;
        case PROCESS_NO_PARAMS:
            call_sys_draw_int(1);
            *pid = commandsList[index].process_no_params(fds);
            break;
        case FUNC_PARAMS:
            call_sys_draw_int(2);
            commandsList[index].function_params(argc, argv);
            break;
        case VOID:
            call_sys_draw_int(3);
            commandsList[index].function_void();
            break;
    }
    call_sys_enter();
    
    *flag = 1;
}







