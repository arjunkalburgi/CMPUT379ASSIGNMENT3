// implementation modified from http://www.learn-c.org/en/Linked_lists

#include "tlb.h"

tlb_t * make_tlb(int cap) {
    tlb_t * new_tlb = malloc(sizeof(tlb_t)); 

    node_t * head = malloc(sizeof(node_t));
    head->pagenumber;
    head->prev = NULL; 
    head->next = NULL; 

    new_tlb->head = head;
    new_tlb->end = new_tlb->head; 
    new_tlb->length = 1; 
    new_tlb->capacity = cap; 

    printf("tlblen: %d, tlbcap: %d, tlbhead: %d, tlbend: %d\n", new_tlb->length, new_tlb->capacity, new_tlb->head->pagenumber, new_tlb->end->pagenumber);

    return new_tlb; 
}

void print_tlb_info(tlb_t * tlb) {
    printf("tlblen: %d, tlbcap: %d, tlbhead: %d, tlbend: %d\n", tlb->length, tlb->capacity, tlb->head->pagenumber, tlb->end->pagenumber);
}

void print_node_info(node_t * node) {
    if (node->next == NULL) {
        printf("nodeprev: %d, pagenumber: %d, nodenext: %s\n", node->prev->pagenumber, node->pagenumber, "NULL");
    } else if (node->prev == NULL) {
        printf("nodeprev: %s, pagenumber: %d, nodenext: %d\n", "NULL", node->pagenumber, node->next->pagenumber);
    } else {
        printf("nodeprev: %d, pagenumber: %d, nodenext: %d\n", node->prev->pagenumber, node->pagenumber, node->next->pagenumber);
    }
}

void print_list(tlb_t * t) {
    node_t * current = t->head;
    int num = 1; 
    while (current != NULL) {
        printf("%d: %d\n", num, current->pagenumber);
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

node_t * tlb_get(tlb_t * t, int pagenumber, measurementarray_t *m) {
    /*
        Get stuff from the tlb or insert it from the pagetable
    */
    node_t * match = tlb_match(t, pagenumber); 

    if (match == NULL) {
        return tlb_insert(t, pagenumber); 
    }

    return match; 
}

node_t * tlb_insert(tlb_t * t, int pagenumber) {
    /*
        this is called if tlb does not have pagenumber
    */
    node_t * head = t->head;

    // get from pagetable
    // page_t * page = pgtbl_get()
    node_t * new = malloc(sizeof(node_t));
    new->pagenumber = pagenumber; 
    new->next = NULL; 
    new->prev = t->end;

    if (t->length < t->capacity) {
        // increase length
        t->length = t->length + 1; 
    } else {
        // evict 
        node_t *temp = t->head; 
        t->head = t->head->next; 
        free(temp); 
    }

    // append to end (unless tlb is empty (head is null))
    if (t->head->pagenumber) {
        t->end->next = new; 
    } else {
        t->head = new; 
    }
    t->end = new;

    return new;
}
/*
int main(int argc, char const *argv[]) {

    tlb_t *tlb = make_tlb(3); 
    tlb_put(tlb, 1); 
    tlb_put(tlb, 2); 
    tlb_put(tlb, 3); 
    print_tlb_info(tlb);
    print_list(tlb); 

    // match end
    tlb_put(tlb, 3); 
    print_tlb_info(tlb);
    print_list(tlb);

    // match 
    tlb_put(tlb, 2); 
    print_tlb_info(tlb);
    print_list(tlb); 

    // match head
    tlb_put(tlb, 1); 
    print_tlb_info(tlb);
    print_list(tlb); 

    // no match
    tlb_put(tlb, 4); 
    print_tlb_info(tlb);
    print_list(tlb);
}
*/


node_t * tlb_match(tlb_t * t, int pagenumber) {
    node_t *previous, *current;
    node_t * head = t->head; 

    if (head == NULL) {
        return head;
    }

    if ((head)->pagenumber == pagenumber) {
        return head;
    }

    previous = current = (head)->next;
    while (current) {
        if (current->pagenumber == pagenumber) {
            return current;
        }

        previous = current;
        current  = (current)->next;
    }
    return NULL;
}

int tlb_put(tlb_t * t, int pagenumber) {
    node_t * head = t->head;

    if (t->length < t->capacity) {
        // append to end
        node_t * new = malloc(sizeof(node_t));
        new->pagenumber = pagenumber; 
        new->next = NULL; 
        new->prev = t->end;
        t->end->next = new; 
        t->end = new; 
        t->length = t->length + 1; 
        return 1;
    }

    node_t * match = tlb_match(t, pagenumber); 
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
        new->pagenumber = pagenumber; 
        new->next = NULL; 
        new->prev = t->end;
        t->end->next = new; 
        t->end = new;
        return 1;
    }

    return 0; 
}