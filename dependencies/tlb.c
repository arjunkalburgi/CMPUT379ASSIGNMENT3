// implementation modified from http://www.learn-c.org/en/Linked_lists

#include "tlb.h"

tlb_t * make_tlb(int cap, hashtable_t * h, ffl_t * f) {
    tlb_t * new_tlb = malloc(sizeof(tlb_t)); 

    node_t * head = malloc(sizeof(node_t));
    //printf("made it b4 pg num assign\n");
    head = NULL;
    //((head)->data)->pagenumber = -1;
    //printf("made it after pg num assign\n");
/*
    head->prev = NULL; 
    head->next = NULL; 
 */
    new_tlb->head = head;
    new_tlb->end = new_tlb->head; 
    new_tlb->length = 1; 
    new_tlb->capacity = cap;
    new_tlb->hash = h; 
    new_tlb->frameslist = f; 
    printf("new tlb made:\n tlblen: %d, tlbcap: %d\n", new_tlb->length, new_tlb->capacity);
    /*, tlbhead: %d, tlbend: %d\n",*/
    /*, new_tlb->head->data->pagenumber, new_tlb->end->data->pagenumber*/
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
    //printf("made it in get\n");
    node_t * match = tlb_match(t, pagenumber); 
    //printf("made it in get after match\n");
    if(match == NULL) {
        //printf("supggg\n");

        m->tlbmisses = m->tlbmisses + 1;
        //printf("bout to insert\n");
        //printf("PN: %d\n", pagenumber);
        return tlb_insert(t, pagenumber); 
    } else {
        // tlb hit 
        // if (t->frameslist->flag == "l") {ffl_update(t->frameslist, t->data->framenumber)}
    }
    //printf("made it in get match=null\n");
    m->tlbhits++;
    return match; 
}

node_t * tlb_insert(tlb_t * t, int pagenumber) {
    /*
        this is called if tlb does not have pagenumber
    */
    node_t * head = t->head;
    //printf("inside insert\n");
    // get from pagetable
    node_t * new = malloc(sizeof(node_t));
    //printf("inside insert2\n");
    new->data = malloc(sizeof(page_t)); 

    new->data->pagenumber = pagenumber; 
    //printf("inside insert3\n"); 
    // new->data = pgtbl_get(hash, itoa(pagenumber)); // return frame number 
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
    if (t->head != NULL) {
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
    //printf("made it to tlb_match\n");

    if (head == NULL) {
        return NULL;
    }
    //printf("made it past 1PN head\n");

    if ((head)->data->pagenumber == pagenumber) {
        return head;
    }

    //printf("made it past PN head\n");
    previous = current = (head)->next;
    while (current) {
        if (current->data->pagenumber == pagenumber) {
            return current;
        }

        previous = current;
        current  = (current)->next;
    }
    return NULL;
}

void tlb_framematch(tlb_t * t, int framenumber){ 
    node_t *previous, *current;
    node_t * head = t->head; 

    if (head == NULL) {
        return;
    }

    if ((head)->data->framenumber == framenumber) {
        return;
    }

    previous = current = (head)->next;
    while (current) {
        if (current->data->framenumber == framenumber) {
            // GET RID OF CURRENT
        }

        previous = current;
        current  = (current)->next;
    }
}

/*int tlb_put(tlb_t * t, int pagenumber) {
    node_t * head = t->head;

    if (t->length < t->capacity) {
        // append to end
        node_t * new = malloc(sizeof(node_t));
        new->data->pagenumber = pagenumber; 
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
        new->data->pagenumber = pagenumber; 
        new->next = NULL; 
        new->prev = t->end;
        t->end->next = new; 
        t->end = new;
        return 1;
    }

    return 0; 
}*/