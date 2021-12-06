#ifndef _MEMC
#define _MEMC
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

#include "func.h"
#include "list.h"


#define TAMLINEA 25

/* FUNCTION PROTOTYPES */

/*asignacion de memoria(malloc, mmap, shared)*/
void do_asignar(tList *l, char *trozos[]);

/*desasigna memoria(mmaloc, mmap, shared)*/
void do_desasignar(tList *l, char *trozos[]);

/*elimina memoria compartida*/
void do_borrarkey(tList *l, char *trozos[]);

/*muestra las direciones de memoria*/
void do_mem(tList *l, char *type[]);

/*muestra los bytes de una direccion de memoria determinada*/
void do_volcar(char *trozos[]);

/*llena el contenido de una direccion de memoria*/
void do_llenar(char *trozos[]);

/*una funcion que se llama a si misma n veces*/
void do_recursiva(char *trozos[]);

/*lee un fichero de una direccion de memoria*/
void do_rfich(char *trozos[]);

/*escribe el contenido de una direccion de memoria en un fichero*/
void do_wfich(char *trozos[]);


#endif