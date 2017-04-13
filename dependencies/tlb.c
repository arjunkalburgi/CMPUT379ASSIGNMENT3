// implementation modified from http://www.learn-c.org/en/Linked_lists

#include "tlb.h"

tlb_t * make_tlb(int cap, hashtable_t * h, ffl_t * f) {
    tlb_t * new_tlb = malloc(sizeof(tlb_t)); 

    node_t * head = malloc(sizeof(node_t));
    head = NULL;

    new_tlb->head = head;
    new_tlb->end = new_tlb->head; 
    new_tlb->length = 1; 
    new_tlb->capacity = cap;
    new_tlb->hash = h; 
    new_tlb->frameslist = f; 
    printf("new tlb made:\n tlblen: %d, tlbcap: %d\n", new_tlb->length, new_tlb->capacity);
    return new_tlb; 
}

void print_tlb_info(tlb_t * tlb) {
    printf("tlblen: %d, tlbcap: %d, tlbhead: %d, tlbend: %d\n", tlb->length, tlb->capacity, tlb->head->data->pagenumber, tlb->end->data->pagenumber);
}

void print_node_info(node_t * node) {
    if (node->next == NULL) {
        printf("nodeprev: %d, pagenumber: %d, nodenext: %s\n", node->prev->data->pagenumber, node->data->pagenumber, "NULL");
    } else if (node->prev == NULL) {
        printf("nodeprev: %s, pagenumber: %d, nodenext: %d\n", "NULL", node->data->pagenumber, node->next->data->pagenumber);
    } else {
        printf("nodeprev: %d, pagenumber: %d, nodenext: %d\n", node->prev->data->pagenumber, node->data->pagenumber, node->next->data->pagenumber);
    }
}

void print_list(tlb_t * t) {
    node_t * current = t->head;
    int num = 1; 
    while (current != NULL) {
        printf("%d: %d\n", num, current->data->pagenumber);
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
    if(match == NULL) {
        // TLB MISS
        m->tlbmisses++;
        return tlb_insert(t, pagenumber, m); 
    }

    // TLB HIT UPDATE TLB, UPDATE FFL (if lru)
    ffl_update(t->frameslist, match->data->framenumber);
    m->tlbhits++;
    return match; 
}

node_t * tlb_insert(tlb_t * t, int pagenumber, measurementarray_t *m) {
    /*
        this is called if tlb does not have pagenumber
    */
    node_t * head = t->head;

    char pagenumberkey[5]; 
    snprintf(pagenumberkey, 5,"%d", pagenumber);
    
    // make new node
    node_t * new = malloc(sizeof(node_t));
    new->data = malloc(sizeof(page_t)); 
    new->data = ht_get(t->hash, pagenumberkey); // return page from pagetable, = pagenumber;
    new->next = NULL;
    new->prev = t->end;

    if (t->length < t->capacity) {
        // increase length
        t->length = t->length + 1; 
    } else {
        // evict 
        m->evictedpages++; 
        node_t *temp = t->head; 
        t->head = t->head->next; 
        free(temp); 
    }

    // append to end (unless tlb is empty (head is null))
    if (t->head != NULL) {
        t->end->next = new; 
    } else {
        t->head = new; 
    }
    t->end = new;

    return new;
}

node_t * tlb_match(tlb_t * t, int pagenumber) {
    node_t *previous, *current;
    node_t * head = t->head; 

    if (head == NULL) {
        return NULL;
    }

    if ((head)->data->pagenumber == pagenumber) {
        return head;
    }

    previous = current = (head)->next;
    while (current) {
        if (current->data->pagenumber == pagenumber) {
            current->prev->next = current->next; 
            current->next->prev = current->prev; 
            current->prev = t->end; 
            current->next = NULL; 
            t->end->next = current; 
            t->end = current; 
            return current;
        }

        previous = current;
        current  = (current)->next;
    }
    return NULL;
}

void tlb_framematch(tlb_t * t, int framenumber) {
    // frame has been assigned elsewhere, remove entry

    node_t *previous, *current;
    node_t * head = t->head; 

    if (head == NULL) {
        return;
    }

    if ((head)->data->framenumber == framenumber) {
        // GET RID OF HEAD
        head->next->prev = NULL; 
        head = head->next; 
        free(head);
        return; 
    }

    previous = current = (head)->next;
    while (current) {
        if (current->data->framenumber == framenumber) {
            // GET RID OF CURRENT
            current->prev->next = current->next; 
            current->next->prev = current->prev; 
            free(current); 
            return; 
        }

        previous = current;
        current  = (current)->next;
    }
}
