#ifndef _BUFKB_H
#define _BUFKB_H

#define SIZE 256

typedef struct buff_kb *buff;

struct buff_kb {
	int read_pos;  // Posición de lectura
	int write_pos; // Posición de escritura
	int len;
	char buffer[SIZE];
};

int get_pos();
int newPos(int newPos);
char *get_address();
char get_charAt(int write_pos);
void set_read_pos(int read_pos);
int get_read_pos();

#endif