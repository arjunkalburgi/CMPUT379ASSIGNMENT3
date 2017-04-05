#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int val;
    struct node * prev;
    struct node * next;
} node_t;

typedef struct tlb {
    struct node * head; 
    struct node * end; 
    int length;
    int capacity; 
} tlb_t;

void print_list(tlb_t * t) {
    node_t * current = t->head;

    while (current != NULL) {
        printf("%d\n", current->val);
        current = current->next;
    }
}

node_t * tlb_match(tlb_t * t, int val) {
    node_t *previous, *current;
    node_t * head = t->head; 

    if (head == NULL) {
        return head;
    }

    if ((head)->val == val) {
        return head;
    }

    previous = current = (head)->next;
    while (current) {
        if (current->val == val) {
            previous->next = current->next;
            free(current);
            return current;
        }

        previous = current;
        current  = current->next;
    }
    return 0;
}

int tlb_insert(tlb_t * t, int val) {
    node_t * head = t->head;

    if (t->length < t->capacity) {
        // append to end
        node_t * new = malloc(sizeof(node_t));
        new->val = val; 
        new->next = NULL; 
        new->prev = t->end;
        t->end->next = new; 
        t->end = new; 
        t->length = t->length + 1; 
        return 1;
    }

    node_t * match = tlb_match(t, val); 
    if (match) {
        // already in there. shift it. 
        match->prev->next = match->next; 
        match->next->prev = match->prev; 
        // move to back
        match->next = NULL; 
        match->prev = t->end;
        t->end->next = match; 
        t->end = match;
        return 1; 
    }

    else {
        // evict 
        t->head = t->head->next; 
        free(t->head); 
        // append to end
        node_t * new = malloc(sizeof(node_t));
        new->val = val; 
        new->next = NULL; 
        new->prev = t->end;
        t->end->next = new; 
        t->end = new;
        return 1;
    }
    return 0; 
}

void tlb_flush(tlb_t * t) {
    node_t *head = t->head;
    node_t *current = head; 
    node_t *next = head;

    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
}

int main(int argc, char const *argv[]) {
    // node_t * test_list = malloc(sizeof(node_t));

    // test_list->val = 1;
    // test_list->next = malloc(sizeof(node_t));
    // test_list->next->val = 2;
    // test_list->next->next = malloc(sizeof(node_t));
    // test_list->next->next->val = 3;
    // test_list->next->next->next = malloc(sizeof(node_t));
    // test_list->next->next->next->val = 4;
    // test_list->next->next->next->next = NULL;

    // remove_by_value(&test_list, 3);

    // print_list(test_list);
    // delete_list(test_list);

    // return EXIT_SUCCESS;
}