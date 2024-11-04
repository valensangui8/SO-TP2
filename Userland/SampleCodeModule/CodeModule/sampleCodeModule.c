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

		char *argv1[MAX_ARGS] = {NULL};
		char *argv2[MAX_ARGS] = {NULL};   
        int argc1 = 0, argc2 = 0, one_process = 1, background = 0;

		char not_valid = 0;
        char *token = strtok(buffer, " ");
		while (token != NULL) {
			if (strcmp(token, "|") == 0) {
				one_process = 0;
				token = strtok(NULL, " ");
				continue;
			}
			 if (strcmp(token, "&") == 0) {
        		if (strtok(NULL, " ") == NULL) {
					background = 1;
					break;
				} else {
					call_sys_commandEnter();
					call_sys_error("Parse error: near '&'", STDERR);
					call_sys_enter();
					not_valid = 1; 
					break;
				}
    		}

			if (one_process == 1) {
				argv1[argc1++] = token;
			} else {
				argv2[argc2++] = token;
			}

			token = strtok(NULL, " ");
		}
		if(strcmp(argv1[argc1 - 1]," ") == 0){
			argc1--;
		}else if(argc2 !=0 && strcmp(argv2[argc2 - 1]," ") == 0){
			argc2--;
		}
		argv1[argc1] = NULL;
		argv2[argc2] = NULL;
		if(!not_valid){
			initialize_shell(argv1[0], argc1, argv1, argv2[0], argc2, argv2, background);
		}
	}

}