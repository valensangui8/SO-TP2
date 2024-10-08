#ifndef _LISTADT_H
#define _LISTADT_H

typedef struct Node {
	void *data;
	struct Node *prev;
	struct Node *next;
} Node;

#endif