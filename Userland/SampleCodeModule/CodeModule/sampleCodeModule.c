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

static int parse_command(char *buffer, char *argv1[], int *argc1, char *argv2[], int *argc2, int *background){
	char *token = strtok(buffer, " ");
	int one_process = 1;
	while (token != NULL) {
		if (strcmp(token, "|") == 0) {
			one_process = 0;
			token = strtok(NULL, " ");
			continue;
		}
			if (strcmp(token, "&") == 0) {
			if (strtok(NULL, " ") == NULL) {
				*background = 1;
				break;
			} else {
				call_sys_command_enter();
				call_sys_error("Parse error: near '&'", STDERR);
				call_sys_enter();
				return 1;
			}
		}

		if (one_process) {
			argv1[(*argc1)++] = token;
		} else {
			argv2[(*argc2)++] = token;
		}

		token = strtok(NULL, " ");
	}
	argv1[*argc1] = NULL;
	argv2[*argc2] = NULL;
	return 0;
}

void terminal(){
	char c;
	while (1) {
		char buffer[BUFFER_SIZE] = {0};
		int buffer_pos = 0;
		while ((c = get_char()) != '\n') {
			if (c == 8) { // Backspace key
				if (buffer_pos > 0) {
					buffer_pos--;
					call_sys_delete();
				}
			}
			else if (c == 9) { // tab key
				for (int i = 0; i < TAB_SIZE; i++) {
					buffer[buffer_pos++] = ' ';
					call_sys_draw_char(' ');
				}
			}
			else if (c != 27) {
				buffer[buffer_pos++] = c;
				call_sys_draw_char(c);
			}
		}

		int foreground = call_sys_foreground();
		if(!foreground){
			buffer[buffer_pos] = '\0';
		}
		else{
			buffer[buffer_pos] = '\0';
			call_sys_draw_char('\n');
		}

		char *argv1[MAX_ARGS] = {NULL};
		char *argv2[MAX_ARGS] = {NULL};   
        int argc1 = 0, argc2 = 0, background = 0;

		int not_valid = parse_command(buffer, argv1, &argc1, argv2, &argc2, &background);
		
		if(!not_valid){
			initialize_shell(argv1[0], argc1, argv1, argv2[0], argc2, argv2, background);
		}
	}

}