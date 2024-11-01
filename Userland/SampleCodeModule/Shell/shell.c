#include <shell.h>

typedef struct command {
    char * name;
    union {
        void (*function_with_params)(int background, int argc, char **argv);
        void (*function_no_params)(int background);
    };
    char has_params;
} Command;

// Define si BACKGROUND es 0 o 1 según tu sistema
Command commandsList[COMMANDS] = {
    {"zoomIn", .function_no_params = zoomIn, 0},                          
    {"zoomOut", .function_no_params = zoomOut, 0},
    {"clear", .function_no_params = clear, 0},
    {"div0", .function_no_params = div0, 0},
    {"invalidOpcode", .function_no_params = invalidOpcode, 0},
    {"help", .function_no_params = help, 0},
    {"registers", .function_no_params = registers, 0},
    {"date", .function_no_params = date, 0},
    {"eliminator", .function_no_params = eliminator, 0},
    {"itba", .function_no_params = printLogo, 0},
    {"testprio", .function_no_params = test_prio_user, 0},              
    {"testmm", .function_with_params = test_mm_user, 1},
    {"testprocess", .function_with_params = test_process_user, 1},
    {"ps", .function_no_params = ps, 0},
    {"kill", .function_with_params = kill_process, 1},
    {"testsync", .function_with_params = test_sync_user, 1}
};

static void run_command(char *command, int argc, char **argv, char *flag, int background);

void initialize_shell(char *command, int argc, char **argv, char *command2, int argc2, char **argv2, int background) {
    char flag = 0;
    if(argc2 == 0){ // one process
        run_command(command, argc, argv, &flag, background);
        return;
    }

    run_command(command, argc, argv, &flag, background);
    if(flag == 0 || argc2 == 0){
        return;
    } else{
        run_command(command2, argc2, argv2, &flag, background);
    }
    
}

void run_command(char *command, int argc, char **argv, char *flag, int background){
    if(*command == 0){
        call_sys_enter();
        return;
    }
    int id = readCommand(command);
    executeCommand(id, flag, command, argc, argv, background);
}

int readCommand(char * command) {
    for (int i = 0; i < COMMANDS; i++) {
        if (strcmp(command, commandsList[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

void executeCommand(int index, char * flag, char * command, int argc, char **argv, int background) {
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

    if(commandsList[index].has_params != 0){
        if(argc == 1){
            printf("ERROR: Invalid number of arguments");
            call_sys_enter();
            *flag = 0;
            return;
        }
        commandsList[index].function_with_params(background, argc, argv);
    }else{
        commandsList[index].function_no_params(background);
    }

    call_sys_enter();
    
    *flag = 1;
   
}







