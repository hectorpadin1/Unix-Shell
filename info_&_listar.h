#ifndef _INFO
#define _INFO
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
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

#include "func.h"

#define MAX 2048


/*functions required for file operations*/

char TipoFichero(mode_t m);

char * ConvierteModo2 (mode_t m);

void listar(int r, int l, int v, char *path);
 

/*FUNCTION PROTOTYPES*/

/*checks for directory existance*/
int dirExists(char *path);


/*prints all the info for a file or directory*/
void muestraInfo(int l, char *path, char *file);

/*Gives info on the files and/or directories supplied*/
void do_info(char *trozos[]);

/*lists the directories and/or files supplied to it as command line arguments*/
void do_listar(char *trozos[]);


#endif
