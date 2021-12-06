/*  
 *  authors : Hector Padin Torrente, Luis Cascon Padron
 *  logins  : hector.padin@udc.es, luis.cascon@udc.es
 *  group   : 1.4
 *
 */

#include "listP.h"


void createListP(tListP *p) {

	p->max = MAX_LISTP;
	p->count = 0;
}


int insertElementP(tListP *p, pid_t pid, int pr, char *c, char *st) {

	if (p) {		// checks list existence
		if (p->count < MAX_LISTP) {		// chechs if the list has space
			int i = p->count;

			p->node[i].pid = pid;
			p->node[i].priority = pr;
			p->node[i].cmd = strdup(c);
			p->node[i].time = time(NULL);
			p->node[i].status = strdup(st);
			p->count++;
			return 1;
		} else {
			printf("error: you do not have enought space in the list.\n");
			return 0;
		}
	} else {
		printf("error: list does not exist.\n");
		return 0;
	}
}


int deleteElementP(tListP *p, pid_t pid) {

	if (!p) {
		printf("error: list does not exist.\n");
		return 0;
	}
	for (int i = 0; i < p->count; i++) {
		if (p->node[i].pid == pid) {
			free(p->node[i].cmd);
			for (int j = i; j < p->count; j++)
				p->node[j]=p->node[j+1];
			p->count--;
			return 1;
		}
	}
	return 0;
}


int clearListP(tListP *p) {

	if (p) {
		if (p->count > 0) {
			int b = 0, i = 0;

			do {
				b = deleteElementP(p,p->node[0].pid);
				i++;
			} while (i<p->count && b);
			p->count = 0;
			return b;
		} else return 1;
	} else {
		printf("error: list does not exist.\n");
		return 0;
	}
}


void updateProcs(tListP *p) {
	pid_t pd;
	int st;

	for(int i=0 ; i <= p->count; i++) {
		pd = p->node[i].pid; 
		p->node[i].priority = getpriority(PRIO_PROCESS,pd);
		if (pd == waitpid(pd, &st, WNOHANG | WUNTRACED | WCONTINUED)) {
			char *estado = TipoSenal(st);
			p->node[i].status = estado;
		}
	}
}



char *TipoSenal(int status){
	if (WIFEXITED(status)) 
		return "finished";
	else if (WIFSIGNALED(status))
		return "finished by signal";
	else if (WIFSTOPPED(status))
		return "stopped";
	else if (WIFCONTINUED(status))
		return "running...";
	else return ("uknown signal");
}


void showElementP(tItemP it) {
	printf("Pid of process: %d ", it.pid);
	printf("| Priority: %d | Command: %s ", it.priority, it.cmd);
	printf("| State: %s ", it.status);
	printf("| Created at: %s", ctime(&it.time));	
}