#ifndef _HIST
#define _HIST
/*  
 *  authors : Hector Padin Torrente, Luis Cascon Padron
 *  logins  : hector.padin@udc.es, luis.cascon@udc.es
 *  group   : 1.4
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#define MAXENTRADA 4096


/*structure of the list*/

struct input {
	char *cmd;
};

typedef struct input tInput; 

struct hist{		
		tInput nodo[MAXENTRADA];
		int max;
		int count;
};

typedef struct hist tHist;


/*FUNCTION PROTOTYPES*/

/*Creates an empty list*/
void createHist(tHist *h);  

/*Insert an element in the list*/
void insertElement(char * input, tHist *h);

/*Clears the list*/
void clearHist(tHist *h);

/*Shows the list*/
void showHist(tHist *h);


#endif