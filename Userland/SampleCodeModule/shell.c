#include <shell.h>
#include <stdint.h>
#include <commands.h>

char * commands[COMMANDS] = {"zoomIn", "zoomOut", "clear", "div0", "invalidOpcode", "help", "registers", "date", "eliminator", "itba"};
void (* commandsFunctions[])() = {zoomIn, zoomOut, clear, div0, invalidOpcode, help, registers, date, eliminator, printLogo};

void initialize_shell(char *command) {
    if(*command == 0){
        call_sys_enter();
        return;
    }
    int id = readCommand(command);
    char flag = 0;
    executeCommand(id, &flag, command);
}

int readCommand(char * command) {
    for (int i = 0; i < COMMANDS; i++) {
        if (strcmp(command, commands[i]) == 0) {
            return i;
        }
    }
    return -1;
}

void executeCommand(int index, char * flag, char * command) {
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
    commandsFunctions[index]();
    
    call_sys_enter();
    *flag = 1;
}







