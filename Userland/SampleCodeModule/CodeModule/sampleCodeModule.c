#include <myStdio.h>
#include <shell.h>
#include <syscalls.h>
#include <test.h>
#include <stddef.h>

#define CHARACTER_COLOR 0xB0CA07
#define TAB_SIZE 4
#define HEADER_SIZE 6
#define MIN_X 8
#define MIN_Y 8
#define WIDTH_FONT 8
#define HEIGHT_FONT 16
#define BUFFER_SIZE 256
#define MAX_ARGS 10

static void terminal();

int main() {
	terminal();
	
	return 0;
}

void terminal(){
	char c;
	while (1) {
		char buffer[BUFFER_SIZE] = {0};
		int buffer_pos = 0;
		while ((c = getChar()) != '\n') {
			if (c == 8) { // Backspace key
				if (buffer_pos > 0) {
					buffer_pos--;
					call_sys_delete();
				}
			}
			else if (c == 9) { // tab key
				for (int i = 0; i < TAB_SIZE; i++) {
					buffer[buffer_pos++] = ' ';
					call_sys_drawChar(' ');
				}
			}
			else if (c != 27) {
				buffer[buffer_pos++] = c;
				call_sys_drawChar(c);
			}
		}
		buffer[buffer_pos] = '\0';

		char *command = strtok(buffer, " ");
		char *token;
        
        char *args[MAX_ARGS] = {NULL};  
        int arg_count = 0;
        
        while (token != NULL) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
			if(token == '|' || token == '&' || token == '<' || token == '>'){ {
				//pipes, background and redirections processes
				//llamar a la segunda funcion
			}
        }
        args[arg_count] = NULL;

        initialize_shell(command, arg_count, args);
	}
}
