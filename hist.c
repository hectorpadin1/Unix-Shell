/*  
 *  authors : Hector Padin Torrente, Luis Cascon Padron
 *  logins  : hector.padin@udc.es, luis.cascon@udc.es
 *  group   : 1.4
 *
 */

#include "hist.h"


/*creates an empty list*/
void createHist(tHist *h) {
	
	// list is initialiced
	h -> nodo[0].cmd = NULL;
	h -> max = MAXENTRADA;
	h -> count = 0;
}


/*inserts an element on the list*/
void insertElement(char * input, tHist *h) {

	if (h == NULL) {	// checks list existence
		printf("FATAL ERROR: list does not exist.\n");
	} else {
		int i = h -> count;		// the number of actual commands saved in the list is stored
		if (i != MAXENTRADA) {		// checks if the list is not full
			h -> nodo[i].cmd = strdup(input);	// adds the command in the list
			h -> count++;		// counter is increased
		} else {
			printf("FATAL ERROR: the list of saved commands filled it's memory space.\n");
			printf("Please press 'hist -c' to free space.\n");
		}
	}
}


/*deletes all the contents of the list*/
void clearHist(tHist *h) {
	
	if (h == NULL) {	// checks list existence
		printf("FATAL ERROR: list does not exist.\n");
	} else {
		for (int i = 0; i < (h -> count); i++) {	// every item of the list is eliminated
			free(h -> nodo[i].cmd);
		}
		h -> count = 0;			// counter is reestarted
	}
}


/*prints the list*/
void showHist(tHist *h) {
	
	if (h == NULL) {		// checks list existence
		printf("FATAL ERROR: list does not exist.\n");
	} else {
		printf("\n");
		int c = (h -> count);
		for (int i = 0; i < c; i++) {
			printf("%s", h -> nodo[i].cmd);
		}
		printf("\n");
	}
}