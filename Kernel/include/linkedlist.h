#ifndef LINKEDLIST_HEADER
#define LINKEDLIST_HEADER

#include <memoryManagerADT.h>

typedef struct LinkedListCDT * LinkedListADT;

typedef struct Node {
  void *data;
  struct Node *next;
  struct Node *prev;
} TNode;

LinkedListADT create_linked_list();
TNode *append_element(LinkedListADT list, void *data);
int delete_element(LinkedListADT list, void *data);
TNode *append_node(LinkedListADT list, TNode *node);
TNode *prepend_node(LinkedListADT list, TNode *node);
TNode *get_first(LinkedListADT list);
int is_empty(LinkedListADT list);
int get_length(LinkedListADT list);
void *remove_node(LinkedListADT list, TNode *node);
void begin(LinkedListADT list);
int has_next(LinkedListADT list);
void *next(LinkedListADT list);
void free_linked_list(LinkedListADT list);

#endif
