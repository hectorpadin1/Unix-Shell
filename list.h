#ifndef _LIST
#define _LIST
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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>

#define MAXENTRADA 4096
#define MMALLOC 0
#define MMAP 1
#define MSHARED 2
#define ALL 3


/*structure of the list*/

typedef struct nodo {
	int mem_type;		// malloc, shared, mmap
	int prot;
	void *address;
	char *file;
	key_t key;
	size_t size;
	time_t time;
} tItem;


typedef struct list{		
		tItem nodo[MAXENTRADA];
		int max;
		int count;
} tList;

/* FUNCTION PROTOTYPES */

/*creates an empty list*/
void createList(tList *l);

/*inserts an element on the list*/
int insertElementL(tList *l, int m_type, int p, void *a, char *f, key_t k, size_t s);

/*deletes a position of the list*/
int deleteElement(tList *l, void *addr);

/*clears the all commands*/
int clearList(tList *l);

/*prints the list*/
void showList(tList *l, int m_type);


#endif