#ifndef _BUFKB_H
#define _BUFKB_H

#define SIZE 4096

typedef struct buff_kb *buff;

struct buff_kb {
	int read_pos;  // Posición de lectura
	int write_pos; // Posición de escritura
	int len;
	char buffer[SIZE];
};

int getPos();
int newPos(int newPos);
char *getAddress();
char getCharAt(int write_pos);
void setReadPos(int read_pos);
int getReadPos();

#endif