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

#include "mem.h"
#include "proc.h"
#include "hist.h"
#include "func.h"
#include "info_&_listar.h"


/*prints the prompt*/
void imprimirPrompt() {

	printf("@user:$ ");
}


/*reads input*/
void leerEntrada(char * input, tHist *h) {
	
	if (!fgets(input,MAXENTRADA,stdin)) {
		printf("FATAL ERROR : input could not be read. Maybe try again ¿?\n");
	} else insertElement(input,h);
}


/*selects and option depending of what has been typed*/
void procesarEntrada(char * trozos[], tHist *h, tList *l, tListP *p) {

	if (trozos[0] == NULL)
		return;
	if (!strcmp(trozos[0],"autores")){
		do_autores(trozos[1]);
	} else
	if (!strcmp(trozos[0],"pid")){
		do_pid(trozos[1]);
	} else
	if (!strcmp(trozos[0],"cdir")){
		do_cdir(trozos[1]);
	} else
	if (!strcmp(trozos[0],"fecha")){
		do_fecha(trozos[1]);
	} else
	if (!strcmp(trozos[0],"hora")){
		do_hora(trozos[1]);
	} else
	if (!strcmp(trozos[0],"hist")){
		do_hist(trozos[1],h);
	} else 
	if (!strcmp(trozos[0],"crear")){
		do_crear(trozos[1],trozos[2]);
	} else 
	if (!strcmp(trozos[0],"borrar")){
		do_borrar(trozos[1],trozos[2]);
	} else 
	if (!strcmp(trozos[0],"info")){
		do_info(&trozos[1]);
	} else 
	if (!strcmp(trozos[0],"listar")){
		do_listar(&trozos[1]);
	} else
	if (!strcmp(trozos[0],"asignar")){
		do_asignar(l,&trozos[1]);
	} else
	if (!strcmp(trozos[0],"desasignar")){
		do_desasignar(l,&trozos[1]);
	} else
	if (!strcmp(trozos[0],"borrarkey")){
		do_borrarkey(l,&trozos[1]);
	} else
	if (!strcmp(trozos[0],"mem")){
		do_mem(l,&trozos[1]);
	} else
	if (!strcmp(trozos[0],"recursiva")){
		do_recursiva(&trozos[1]);
	} else
	if (!strcmp(trozos[0],"volcar")){
		do_volcar(&trozos[1]);
	} else
	if (!strcmp(trozos[0],"llenar")){
		do_llenar(&trozos[1]);
	} else
	if (!strcmp(trozos[0],"rfich")){
		do_rfich(&trozos[1]);
	} else
	if (!strcmp(trozos[0],"wfich")){
		do_wfich(&trozos[1]);
	} else
	if (!strcmp(trozos[0],"priority")){
		do_priority(&trozos[1]);
	} else
	if (!strcmp(trozos[0],"fork")){
		do_fork();
	} else
	if (!strcmp(trozos[0],"exec")){
		do_exec(&trozos[1]);
	} else
	if (!strcmp(trozos[0],"pplano")){
		do_pplano(&trozos[1]);
	} else
	if (!strcmp(trozos[0],"splano")){
		do_splano(p,&trozos[1]);
	} else
	if (!strcmp(trozos[0],"listarprocs")){
		do_listarprocs(p);
	} else
	if (!strcmp(trozos[0],"proc")){
		do_proc(p,&trozos[1]);
	} else
	if (!strcmp(trozos[0],"borrarprocs")){
		do_borrarprocs(p,&trozos[1]);
	} else
	if (!strcmp(trozos[0],"fin") || !strcmp(trozos[0],"end") || !strcmp(trozos[0],"exit")){
		do_exit(trozos[1],h,l,p);
	} else 
	if ((!strcmp(trozos[0],"help")) || (!strcmp(trozos[0],"man")) || (!strcmp(trozos[0],"help"))){
		do_help(trozos[1]);
	} else do_prog(p,&trozos[0]);		
}


/*shell programm*/
void shell() {
	char * trozos[MAX];
	char input[MAX];
	tHist h;
	tList l;
	tListP p;
	
	createHist(&h);		// list of commands is created
	createList(&l);		// list of memory addresses
	createListP(&p);	// list of process
	while (!0) {
		imprimirPrompt();
		leerEntrada(input,&h);
		TrocearCadena(input, trozos);
		procesarEntrada(trozos,&h,&l,&p);
	}
}


/*here runs the shell*/
int main() {

	shell();
	return 0;
}