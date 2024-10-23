#include <stdio.h>
#include <stdlib.h>
#include <linkedlist.h>
#include <defs.h>
#include <stdlib.h>

struct LinkedListCDT {
	TNode *first;
	TNode *last;
	TNode *current;
	int len;
};

LinkedListADT create_linked_list() {
	LinkedListADT list = (LinkedListADT) alloc_memory(sizeof(struct LinkedListCDT));
	list->len = 0;
	list->first = NULL;
	list->last = NULL;
	list->current = NULL;
	return list;
}

TNode *append_element(LinkedListADT list, void *data) {
	if (list == NULL){
		return NULL;
	}
	TNode *newNode = (TNode *) alloc_memory(sizeof(TNode));
	newNode->data = data;
	return append_node(list, newNode);
}

int delete_element(LinkedListADT list, void *data) {
	if(list == NULL){
		return -1;
	}
	while(has_next(list)){
		TNode *current = list->current;
        list->current = current->next;
		if(current->data == data){
			remove_node(list, current);
			return 0;
		}
	}
	return -1;
}

TNode *append_node(LinkedListADT list, TNode *node) {
	if (list == NULL)
		return NULL;
	node->next = NULL;
	if (list->len > 0)
		list->last->next = node;
	else
		list->first = node;
	node->prev = list->last;
	list->last = node;
	list->len++;
	return node;
}

TNode *prepend_node(LinkedListADT list, TNode *node) {
	if (list == NULL)
		return NULL;
	node->prev = NULL;
	if (list->len > 0)
		list->first->prev = node;
	else
		list->last = node;
	node->next = list->first;
	list->first = node;
	list->len++;
	return node;
}

TNode *get_first(LinkedListADT list) {
	if (list == NULL)
		return NULL;
	return list->first;
}

int is_empty(LinkedListADT list) {
	if (list == NULL)
		return -1;
	return !list->len;
}

int get_length(LinkedListADT list) {
	if (list == NULL)
		return -1;
	return list->len;
}

void *remove_node(LinkedListADT list, TNode *node) {
	if (list == NULL || node == NULL)
		return NULL;

	if (list->first == node)
		list->first = node->next;
	else
		node->prev->next = node->next;

	if (list->last == node)
		list->last = node->prev;
	else
		node->next->prev = node->prev;

	list->len--;
	void *data = node->data;
	node->next = NULL;
	node->prev = NULL;

	return data;
}
void begin(LinkedListADT list) {
	if (list == NULL)
		return;
	list->current = list->first;
}

int has_next(LinkedListADT list) {
	if (list == NULL)
		return -1;
	return list->current != NULL;
}

void *next(LinkedListADT list) {
	if (!has_next(list))
		return NULL;
	void *data = list->current->data;
	list->current = list->current->next;
	return data;
}

void free_linked_list(LinkedListADT list) {
  TNode *current = list->first;
	TNode *next;
	while (current != NULL) {
		next = current->next;
		free_memory(current);
		current = next;
	}
	free_memory(list);
}