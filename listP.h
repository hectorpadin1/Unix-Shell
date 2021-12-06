#ifndef _LISTP
#define _LISTP
/*  
 *  authors : Hector Padin Torrente, Luis Cascon Padron
 *  logins  : hector.padin@udc.es, luis.cascon@udc.es
 *  group   : 1.4
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <grp.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_LISTP 4096
#define RNN 0
#define TRM 1
#define TBS 2
#define STP 3


/*structure of the list*/

typedef struct node {
	pid_t pid;
	int priority;
	char *cmd;
	time_t time;
	char *status;
} tItemP;


typedef struct listp {		
		tItemP node[MAX_LISTP];
		int max;
		int count;
} tListP;


/* FUNCTION PROTOTYPES */

/*creates an empty list*/
void createListP(tListP *list);

/*inserts an element on the list*/
int insertElementP(tListP *list, pid_t pid, int priority, char *cmd, char *status);

/*changes the priority of process at the list*/
int chPriority(tListP *l, int pid, int pr);


void showElementP(tItemP it);


char *TipoSenal(int status);

/* updates list of processes */
void updateProcs(tListP *p);

/*deletes a position of the list*/
int deleteElementP(tListP *list, pid_t pid);

/*clears the all commands*/
int clearListP(tListP *list);


#endif