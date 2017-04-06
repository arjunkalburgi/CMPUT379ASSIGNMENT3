#include "tlb.h"

tlb_t * make_tlb(int cap, int headvalue) {
    tlb_t * new_tlb = malloc(sizeof(tlb_t)); 

    node_t * head = malloc(sizeof(node_t));
    head->val = headvalue;
    head->prev = NULL; 
    head->next = NULL; 

    new_tlb->head = head;
    new_tlb->end = new_tlb->head; 
    new_tlb->length = 1; 
    new_tlb->capacity = cap; 

    printf("tlblen: %d, tlbcap: %d, tlbhead: %d, tlbend: %d\n", new_tlb->length, new_tlb->capacity, new_tlb->head->val, new_tlb->end->val);

    return new_tlb; 
}

void print_tlb_info(tlb_t * tlb) {
    printf("tlblen: %d, tlbcap: %d, tlbhead: %d, tlbend: %d\n", tlb->length, tlb->capacity, tlb->head->val, tlb->end->val);
}

void print_node_info(node_t * node) {
    if (node->next == NULL) {
        printf("nodeprev: %d, val: %d, nodenext: %s\n", node->prev->val, node->val, "NULL");
    } else if (node->prev == NULL) {
        printf("nodeprev: %s, val: %d, nodenext: %d\n", "NULL", node->val, node->next->val);
    } else {
        printf("nodeprev: %d, val: %d, nodenext: %d\n", node->prev->val, node->val, node->next->val);
    }
}

void print_list(tlb_t * t) {
    node_t * current = t->head;
    int num = 1; 
    while (current != NULL) {
        printf("%d: %d\n", num, current->val);
        current = current->next;
        num++; 
    }
    printf("\n");
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
            return current;
        }

        previous = current;
        current  = (current)->next;
    }
    return NULL;
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
    if (match != NULL) {
        // already in there
        // if it's the head
        if (match == t->head) {
            // remove it. 
            match->next->prev = match->prev; 
            t->head = match->next; 
            // move to back
            match->next = NULL; 
            match->prev = t->end;
            t->end->next = match; 
            t->end = match;
        }
        // if it's the end, then it's already at the back
        else if (match != t->end) {
            // remove it. 
            match->prev->next = match->next; 
            match->next->prev = match->prev; 
            // move to back
            match->next = NULL; 
            match->prev = t->end;
            t->end->next = match; 
            t->end = match;
        }
        return 1; 
    }

    else {
        // evict 
        node_t *temp = t->head; 
        t->head = t->head->next; 
        free(temp); 
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

int main(int argc, char const *argv[]) {

    tlb_t *tlb = make_tlb(3, 1); 
    tlb_insert(tlb, 2); 
    tlb_insert(tlb, 3); 
    print_tlb_info(tlb);
    print_list(tlb); 

    // match end
    tlb_insert(tlb, 3); 
    print_tlb_info(tlb);
    print_list(tlb);

    // match 
    tlb_insert(tlb, 2); 
    print_tlb_info(tlb);
    print_list(tlb); 

    // match head
    tlb_insert(tlb, 1); 
    print_tlb_info(tlb);
    print_list(tlb); 

    // no match
    tlb_insert(tlb, 4); 
    print_tlb_info(tlb);
    print_list(tlb);
}