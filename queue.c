#include "queue.h"

// Function pointers to (potentially) custom malloc() and
// free() functions.
//
static void * (*malloc_fptr)(size_t size) = NULL;
static void   (*free_fptr)(void* addr)    = NULL; 

// Implement your queue functions here.
//

bool queue_register_malloc(void * (*malloc)(size_t)) {
    if (malloc == NULL) return false;
    malloc_fptr = malloc;
    linked_list_register_malloc(malloc);

    return true;
}

bool queue_register_free(void (*free)(void*)) {
    if (free == NULL) return false;
    free_fptr = free;
    linked_list_register_free(free);

    return true;
}

struct queue *queue_create(void) {
    struct queue *q = malloc_fptr(sizeof(struct queue));
    if (!q) return NULL;\

    q->ll = linked_list_create();
    if (!q->ll) {
	free_fptr(q);
	return NULL;
    }

    return q;
}

bool queue_delete(struct queue *queue) {
    if (!queue) return false;

    bool result = linked_list_delete(queue->ll);
    if (result) free_fptr(queue);

    return result;
}

bool queue_push(struct queue *queue, unsigned int data) {
    if (!queue) return false;

    return linked_list_insert_end(queue->ll, data);
}

size_t queue_size(struct queue *queue) {
    if (!queue) return SIZE_MAX;

    return linked_list_size(queue->ll);
}

bool queue_has_next(struct queue *queue) {
    if (!queue) return false;

    return linked_list_size(queue->ll) > 0;
}

bool queue_next(struct queue *queue, unsigned int *popped_data) {
    if (!queue || !popped_data) return false;

    if (linked_list_size(queue->ll) == 0) return false;

    *popped_data = queue->ll->head->data;
    return true;
}

bool queue_pop(struct queue *queue, unsigned int *popped_data) {
    return queue_next(queue, popped_data) && linked_list_remove(queue->ll, 0);
}
