#include <shell.h>

typedef struct command {
    char * name;
    void (* function)(int argc, char **argv);
    char has_params; 
}Command;

Command commandsList[COMMANDS] = {
    {"zoomIn", zoomIn, 0},                          
    {"zoomOut", zoomOut, 0},
    {"clear", clear, 0},
    {"div0", div0, 0},
    {"invalidOpcode", invalidOpcode, 0},
    {"help", help, 0},
    {"registers", registers, 0},
    {"date", date, 0},
    {"eliminator", eliminator, 0},
    {"itba", printLogo, 0},
    {"testprio", test_prio_user, 0},              
    {"testmm", test_mm_user, 1},
    {"testprocess", test_process_user, 1},
    {"ps", ps, 0},
    {"kill", kill_process, 1}
};

void initialize_shell(char *command, int argc, char **argv) {
    if(*command == 0){
        call_sys_enter();
        return;
    }
    int id = readCommand(command);
    char flag = 0;
    executeCommand(id, &flag, command, argc, argv);
}

int readCommand(char * command) {
    for (int i = 0; i < COMMANDS; i++) {
        if (strcmp(command, commandsList[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

void executeCommand(int index, char * flag, char * command, int argc, char **argv) {
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
        commandsList[index].function(argc, argv);
    }else{
        commandsList[index].function(0,NULL);
    }

    call_sys_enter();
    *flag = 1;
}







