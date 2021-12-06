/*  
 *  authors : Hector Padin Torrente, Luis Cascon Padron
 *  logins  : hector.padin@udc.es, luis.cascon@udc.es
 *  group   : 1.4
 *
 */

#include "list.h"


void createList(tList *l) {

	l->max = MAXENTRADA;
	l->count = 0;
}

//insertElementL(l,"malloc",0,0,addr,NULL,0,size)
int insertElementL(tList *l, int m_type, int p, void *a, char *f, key_t k, size_t s) {

	if (l == NULL) {	// checks list existence
		printf("FATAL ERROR: list does not exist.\n");
		return 0;
	} else {
		int i = l->count;
		if (i < MAXENTRADA) {		// checks if the list is not full
			l->nodo[i].mem_type = m_type;
			l->nodo[i].prot = p;
			l->nodo[i].address = a;
			l->nodo[i].file = strdup(f);
			l->nodo[i].key = k;
			l->nodo[i].size = s;
			l->nodo[i].time = time(NULL); 
			l->count++;
			return 1;
		} else {
			printf("FATAL ERROR: the list of allocated memory blocks filled it's memory space.\n");
			return 0;
		}
	}
}


int deleteElement(tList *l, void *addr) {
	int  i=0;

	if (l) {

		while (i<(l->count)) {
			if (l->nodo[i].address==addr) {
				free(l->nodo[i].file);
				if (l->nodo[i].mem_type==MMALLOC)
					free(l->nodo[i].address);
				else if (l->nodo[i].mem_type==MMAP) {
					munmap(l->nodo[i].address,l->nodo[i].size);
					close(l->nodo[i].key);
				} else if (l->nodo[i].mem_type==MSHARED) {
					shmdt(l->nodo[i].address);
					close(l->nodo[i].key);
				} else return 0;
				for (int j = i; j < l->count-1; j++)
					l->nodo[j]=l->nodo[j+1];
				l->count--;
				return 1;
			}
			i++;
		}
		return 0;
	} else {
		printf("error: the list does not exist.\n");
		return 0;
	}
}


int clearList(tList *l) {

	if (l == NULL) {	// checks list existence
		printf("FATAL ERROR: list does not exist.\n");
		return 0;
	} else {
		int b = 0;
		int x = l->count-1;

		do {
			b = deleteElement(l,l->nodo[x].address);
			x--;
		} while (0<=x && b);
		l->count = 0;
		return b;
	}
}


void showElement(tItem it) {

	printf ("%p: size: %ld ", it.address, it.size);
    if (it.mem_type == MMALLOC)
    	printf(" malloc");
    else if (it.mem_type == MSHARED)
    	printf (" shared memory (key %d)", it.key);
    else if (it.mem_type == MMAP)
    	printf (" mmap %s (fd:%d)", it.file, it.key);
    else {
    	printf("error: unvalid file type.\n");
    	return;  
    }
    printf (" %s",ctime(&it.time));
}


void showList(tList *l, int m_type){
	int i = 0;

	if (l) {		// checks list existence
		if (l->count > 0) {
			if (m_type == MMALLOC || m_type == MSHARED || m_type == MMAP) { // type of valid mem 
				while (i < l->count) {
					if (l->nodo[i].mem_type == m_type)	// prints one mem type
						showElement(l->nodo[i]);
					i++;
				}
			} else if (m_type==ALL) {
				for (i = 0; i < l->count; i++)	// prints addresses for all mem types
					showElement(l->nodo[i]);
			} else printf("error: invalid arguments for function showList.\n");
		}
	} else printf("FATAL ERROR: list does not exist.\n");
}