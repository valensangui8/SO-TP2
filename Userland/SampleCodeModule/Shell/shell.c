#include <shell.h>

char * commands[COMMANDS] = {"zoomIn", "zoomOut", "clear", "div0", "invalidOpcode", "help", "registers", "date", "eliminator", "itba", "testprio", "testmm", "testprocess", "ps"};
void (* commandsFunctions[])(int argc, char **argv) = {zoomIn, zoomOut, clear, div0, invalidOpcode, help, registers, date, eliminator, printLogo, test_prio_user, test_mm_user, test_process_user, ps};

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
        if (strcmp(command, commands[i]) == 0) {
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
    if(index == 12 || index == 11){
        commandsFunctions[index](argc, argv);
    }else{
        commandsFunctions[index](0,NULL);
    }
    call_sys_enter();
    *flag = 1;
}







