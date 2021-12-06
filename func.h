#ifndef _FUNC
#define _FUNC
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
#include <grp.h>

#include "info_&_listar.h"
#include "hist.h"
#include "listP.h"
#include "list.h"


/*FUNCTION PROTOTYPES*/

/*Prints an error of an unvalid command*/
void cmd_error();

/*Trocea una nota*/
int TrocearCadena(char * cadena, char * trozos[]);

/*Prints names and logins from authors*/
void do_autores(char *cmd);

/*Prints actual pid, and parent's pid*/
void do_pid (char *cmd);

/*prints actual directory and user is able to change it*/
void do_cdir(char *dir);

/*Prints actual date*/
void do_fecha();

/*Prints actual time*/
void do_hora();

/*Shows the list of commands with an option to delete the typed commands*/
void do_hist(char *cmd, tHist *h);

/*Creates a file or directory*/
void do_crear(char *trozo1, char *trozo2);

/*Deletes a file or directory*/
void do_borrar(char *trozo1, char *trozo2);

/*Ends the program*/
void do_exit(char *cmd, tHist *h, tList *l, tListP *p);

/*Prints a guide for the shell*/
void do_help();





#endif
