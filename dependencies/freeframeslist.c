// implementation modified from http://www.learn-c.org/en/Linked_lists

/*class with 
	int size; 
	int freeframelist[size]; 
	idkwhatthisreturns getFreeFrame(); */

#include "freeframeslist.h"

ffl_t * make_ffl(int cap, const char * flag) {
    ffl_t * new_ffl = malloc(sizeof(ffl_t)); 

    // frame_t * head = malloc(sizeof(frame_t));
    // head = NULL; 

    new_ffl->head = NULL;
    new_ffl->end = new_ffl->head; 
    new_ffl->length = cap-1; 
    new_ffl->capacity = cap;
    new_ffl->type = flag; 
    new_ffl->hash = hash; 
    new_ffl->tlb = tlb;

    return new_ffl; 
}

int ffl_get(ffl_t * l) {
    /*
        Get a frame from the ffl or evict
    */

	// there are free frames
	if (l->length) {
		// add the frame to the used frames list
		ffl_addframeToUsedList(l, l->length); 
		l->length+=-1; 
		// return the framenumber
		return l->length+1; 
	}

	// there aren't free frames 
	if (l->length == 0) {
		// evict
        int framenumbertemp = l->head->framenumber; 
        frame_t *temp = l->head; 
        l->head = l->head->next; 
        free(temp); 
        ht_framematch(l->hash, framenumbertemp); 
        tlb_framematch(l->tlb, framenumbertemp); 
		// return the framenumber
        return framenumbertemp; 
	}

    printf("SOMETHING VERY WRONG HAPPENED IN FFL\n");
    printf("SOMETHING VERY WRONG HAPPENED IN FFL\n");
    printf("SOMETHING VERY WRONG HAPPENED IN FFL\n");
    printf("LENGTH OF FFL WENT NEGATIVE\n");
    return NULL; 
}

void ffl_addframeToUsedList(ffl_t * l, int framenumber) {
    
    // make a new node
    frame_t * new = malloc(sizeof(frame_t));
	
	new->framenumber = framenumber; 
    new->next = NULL; 
    new->prev = l->end;

    // append to end 
    if (l->head != NULL) {
        l->end->next = new; 
    } 
    // unless head is null
    else {
        l->head = new; 
    }
    l->end = new;
}

void ffl_update(ffl_t * l, int framenumber) {
    if (l->flag == "l") {

        frame_t *previous, *current;
        frame_t *head = l->head; 

        if (head == NULL) {
            return NULL;
        }
        if ((head)->framenumber == framenumber) {
            return head;
        }
        previous = current = (head)->next;
        while (current) {
            if (current->framenumber == framenumber) {
                // AMAN TODO; 
                // REMOVE CURRENT FROM LIST 
                    //(current->prev->next = current->next)
                    //(current->next->prev = current->prev)
                // APPEND CURRENT TO END 
                    //(ffl_addframeToUsedList(l, current->framenumber)) // adds to back
                // FREE CURRENT
                return; 
            }

            previous = current;
            current  = (current)->next;
        }
        return;

        // pgtbl.c:140
        // PAGETABLE HIT
        // UPDATE FFL

        // tlb.c:81
        // TLB HIT
        // UPDATE FFL 
    }

}
/*
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
            return current;
        }

        previous = current;
        current  = (current)->next;
    }
    return NULL;
}
*/