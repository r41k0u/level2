#include "linked_list.h"
#include <stdlib.h>

// Function pointers to (potentially) custom malloc() and
// free() functions.
//
static void * (*malloc_fptr)(size_t size) = NULL;
static void   (*free_fptr)(void* addr)    = NULL;

bool linked_list_register_malloc(void * (*malloc)(size_t)) {
    if (malloc == NULL) return false;
    malloc_fptr = malloc;

    return true;
}

bool linked_list_register_free(void (*free)(void*)) {
    if (free == NULL) return false;
    free_fptr = free;

    return true;
}

struct linked_list *linked_list_create(void) {
    struct linked_list *ll = malloc_fptr(sizeof(struct linked_list));
    if (!ll) return NULL;
    ll->head = ll->tail = NULL;
    ll->size = 0;

    return ll;
}

bool linked_list_delete(struct linked_list *ll) {
    if (!ll) return false;

    struct node *cur = ll->head;
    while (cur) {
        struct node *next = cur->next;
        free_fptr(cur);
        cur = next;
    }

    free_fptr(ll);

    return true;
}

size_t linked_list_size(struct linked_list * ll) {
    if (!ll) return SIZE_MAX;

    return ll->size;
}

bool linked_list_insert_end(struct linked_list * ll, unsigned int data) {
    if (!ll) return false;

    struct node *new_node = malloc_fptr(sizeof(struct node));
    if (!new_node) return false;

    new_node->data = data;
    new_node->next = NULL;

    if (!ll->tail) ll->head = ll->tail = new_node;
    else {
	ll->tail->next = new_node;
	ll->tail = ll->tail->next;
    }

    ll->size++;
    return true;
}

bool linked_list_insert_front(struct linked_list * ll, unsigned int data) {
    if (!ll) return false;

    struct node *new_node = malloc_fptr(sizeof(struct node));
    if (!new_node) return false;

    new_node->data = data;
    new_node->next = ll->head;
    ll->head = new_node;

    ll->size++;
    if (ll->size == 1) ll->tail = ll->head;
    return true;
}

bool linked_list_insert(struct linked_list * ll, size_t index, unsigned int data) {
    if (!ll) return false;

    size_t size = linked_list_size(ll);
    if (index > size) return false;
    if (index == 0) return linked_list_insert_front(ll, data);
    if (index == size) return linked_list_insert_end(ll, data);

    size_t cur_idx = 0;
    struct node *cur = ll->head;
    while (cur_idx != index - 1) {
	cur = cur->next;
	cur_idx++;
    }

    struct node *new_node = malloc_fptr(sizeof(struct node));
    if (!new_node) return false;
    new_node->data = data;
    new_node->next = cur->next;
    cur->next = new_node;

    ll->size++;
    return true;
}

size_t linked_list_find(struct linked_list * ll, unsigned int data) {
    if (!ll) return SIZE_MAX;

    size_t idx = 0;
    struct node *cur = ll->head;
    while (cur) {
	if (cur->data == data) return idx;
	cur = cur->next;
	idx++;
    }

    return SIZE_MAX;
}

bool linked_list_remove(struct linked_list * ll, size_t index) {
    if (!ll) return false;
    if (index >= linked_list_size(ll)) return false;

    if (index == 0) {
	struct node *del = ll->head;
	ll->head = ll->head->next;
	free_fptr(del);
        ll->size--;
	if (ll->size == 0) ll->tail = NULL;
	return true;
    }

    size_t cur_idx = 0;
    struct node *cur = ll->head;
    while (cur_idx != index - 1) cur = cur->next;
    struct node *del = cur->next;
    cur->next = cur->next->next;
    if (del == ll->tail) ll->tail = cur;
    free_fptr(del);

    ll->size--;
    return true;
}

struct iterator * linked_list_create_iterator(struct linked_list * ll, size_t index) {
    if (!ll) return NULL;
    if (index >= linked_list_size(ll)) return NULL;

    struct iterator *iter = malloc_fptr(sizeof(struct iterator));
    if (!iter) return NULL;
    iter->ll = ll;
    iter->current_index = index;

    size_t cur_idx = 0;
    struct node *cur = ll->head;
    while (cur_idx != index) {
	cur = cur->next;
	cur_idx++;
    }
    iter->current_node = cur;
    iter->data = cur->data;

    return iter;
}

bool linked_list_delete_iterator(struct iterator * iter) {
    if (!iter) return false;
    free_fptr(iter);

    return true;
}

bool linked_list_iterate(struct iterator * iter) {
    if (!iter) return false;
    if (!iter->current_node->next) return false;

    iter->current_node = iter->current_node->next;
    iter->current_index++;
    iter->data = iter->current_node->data;

    return true;
}
