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
        // copy this from tlb_match
            // that will give you the frame_t that needs to be in the back 
            // take it out of it's spot, and put it in the back 
        // call this every time we get a tlb or pgtable hit 
            // both the tlb and the pgtable have ffl_t * frameslist on it. 
            // call ffl_update(tlb->frameslist, tlb->data->framenumber); 
    }
}
