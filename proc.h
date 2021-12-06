#ifndef _PROC
#define _PROC
	
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

#include "listP.h"
#include "func.h"


struct SEN{
  char *nombre;
  int senal;
};


/* FUNCTION PROTOTYPES */

/* changes a process priority */
void do_priority(char *trozos[]);


void do_fork();


void do_exec(char *trozos[]);


void do_pplano(char *trozos[]);


void do_splano(tListP *p, char *trozos[]);


void do_listarprocs(tListP *p);


void do_borrarprocs(tListP *p, char *trozos[]);


void do_proc(tListP *p, char *trozos[]);


void do_prog(tListP *p, char *trozos[]);

#endif