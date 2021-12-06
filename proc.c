/*  
 *  authors : Hector Padin Torrente, Luis Cascon Padron
 *  logins  : hector.padin@udc.es, luis.cascon@udc.es
 *  group   : 1.4
 *
 */

#include "proc.h"


//char *searchlist[F_LIST] = {NULL};

/* definition of array of signals */

static struct SEN sigstrnum[]={
  {"HUP", SIGHUP},
  {"INT", SIGINT},
  {"QUIT", SIGQUIT},
  {"ILL", SIGILL},
  {"TRAP", SIGTRAP},
  {"ABRT", SIGABRT},
  {"IOT", SIGIOT},
  {"BUS", SIGBUS},
  {"FPE", SIGFPE},
  {"KILL", SIGKILL},
  {"USR1", SIGUSR1},
  {"SEGV", SIGSEGV},
  {"USR2", SIGUSR2},
  {"PIPE", SIGPIPE},
  {"ALRM", SIGALRM},
  {"TERM", SIGTERM},
  {"CHLD", SIGCHLD},
  {"CONT", SIGCONT},
  {"STOP", SIGSTOP},
  {"TSTP", SIGTSTP},
  {"TTIN", SIGTTIN},
  {"TTOU", SIGTTOU},
  {"URG", SIGURG},
  {"XCPU", SIGXCPU},
  {"XFSZ", SIGXFSZ},
  {"VTALRM", SIGVTALRM},
  {"PROF", SIGPROF},
  {"WINCH", SIGWINCH},
  {"IO", SIGIO},
  {"SYS", SIGSYS},
/*senales que no hay en todas partes*/
#ifdef SIGPOLL
  {"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
  {"PWR", SIGPWR},
#endif
#ifdef SIGEMT
  {"EMT", SIGEMT},
#endif
#ifdef SIGINFO
  {"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
  {"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
  {"CLD", SIGCLD},
#endif
#ifdef SIGLOST
  {"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
  {"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
  {"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
  {"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
  {"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
  {"WAITING", SIGWAITING},
#endif
  {NULL,-1},
}; /*end of array sigstrnum */


int Senal(char * sen) { /*devuelve el numero de senial a partir del nombre*/
  int i;
  for (i=0; sigstrnum[i].nombre!=NULL; i++)
    if (!strcmp(sen, sigstrnum[i].nombre))
      return sigstrnum[i].senal;
  return -1;
}


char *NombreSenal(int sen) { /*devuelve el nombre senal a partir de la senal*/
 /* para sitios donde no hay sig2str*/
  int i;
  for (i=0; sigstrnum[i].nombre!=NULL; i++)
    if (sen==sigstrnum[i].senal)
      return sigstrnum[i].nombre;
  return ("SIGUNKNOWN");
}



/* code of processes */


void do_priority(char * trozos[]) {

	if (trozos[0]==NULL) {
		cmd_error();
		return;
	}
	else {
		pid_t pid = (int) atoi(trozos[0]);
		if (trozos[1]==NULL) 
			printf("priority of process %d: %d\n", pid, getpriority(PRIO_PROCESS,pid));
		else {
			int priority = atoi(trozos[1]);
			setpriority(PRIO_PROCESS,pid,priority);
		}
	}
}


void do_fork() {
	pid_t pid = getpid();

	if (!((pid==fork())==0))
		waitpid(pid,NULL,0);
}


void do_exec(char *trozos[]) {

	if (&trozos[0]==NULL) {
		cmd_error();
		return;
	}
	if (trozos[0][0]=='@') {
		pid_t p = getpid();
		int pr;
		trozos[0][0] = ' ';
		pr = atoi(&trozos[0][0]);
		setpriority(PRIO_PROCESS,p,pr);
		int i = 0;
		while (trozos[i]!=NULL){
			trozos[i] = trozos[i+1];
			i++;
		}
	} 
	char *c = trozos[0];
	if (!(execvp(c,trozos)))
		perror("exec failed");
}


void do_pplano(char *trozos[]) {
	
	if (&trozos[0]==NULL) {
		cmd_error();
		return;
	}
	pid_t pid = getpid();
	char comando[MAX];
	int i = 0;
	strcpy(comando,"");
	for (i=0; trozos[i]!=NULL; i++) {
		strcat(comando,trozos[i]);
		strcat(comando," ");
	}
	if ((pid=fork())==0) {
		if (trozos[0][0]=='@') {
			pid_t p = getpid();
			int pr;
			trozos[0][0] = ' ';
			pr = atoi(&trozos[0][0]);
			setpriority(PRIO_PROCESS,p,pr);
			int i = 0;
			while (trozos[i]!=NULL){
				trozos[i] = trozos[i+1];
				i++;
			}
		} 
		char *c = trozos[0];
		if (!(execvp(c,trozos)))
			perror("exec failed");
	} else {
		waitpid(pid,NULL,0);
	}
}


void do_splano(tListP *p, char *trozos[]) {
	
	if (&trozos[0]==NULL) {
		cmd_error();
		return;
	}
	if (trozos[0][0]=='@') {
		pid_t p = getpid();
		int pr;
		trozos[0][0] = ' ';
		pr = atoi(&trozos[0][0]);
		setpriority(PRIO_PROCESS,p,pr);
		int i = 0;
		while (trozos[i]!=NULL){
			trozos[i] = trozos[i+1];
			i++;
		}
	} 
	char *c = trozos[0];
	pid_t child = getpid();
	int i = 0;
	char comando[MAX];
	strcpy(comando,"");
	for (i=0; trozos[i]!=NULL; i++) {
		strcat(comando,trozos[i]);
		strcat(comando," ");
	}
	if ((child=fork())==0) {
		if (execvp(c,trozos)==-1) {
			perror("error");
			exit(1);
		}
	} else {
		waitpid(child,NULL, WNOHANG | WUNTRACED | WCONTINUED);
		insertElementP(p,child,getpriority(PRIO_PROCESS,child),comando,"running...");
	}
}


void do_listarprocs(tListP *p) {
	
	if (!p) {
		printf("error: list does not exist.\n");
		return;
	}
	if (p->count <= 0) {
		printf("error: list is empty.\n");
		return;
	} else {
		updateProcs(p);
		for (int i = 0; i < p->count; i++) 
			showElementP(p->node[i]);
	}	
}


void do_prog(tListP *p, char *trozos[]) {
	
	if (&trozos[0]==NULL) {
		cmd_error();
		return;
	}
	int i = 0, b = 0;
	while(trozos[i]!=NULL) {
		if (trozos[i][0]=='&') {
			b = 1;
			trozos[i]=NULL;
		}
		i++;
	}
	if (b)
		do_splano(p,trozos);
	else
		do_pplano(trozos);
}


void do_proc(tListP *p, char *trozos[]) {
	int i, b = 0;
	pid_t pid;

	if (trozos[0]==NULL)
		do_listarprocs(p);
	if (!(strcmp(trozos[0],"-fg"))) {
		if (trozos[1]==NULL)
			do_listarprocs(p);
		b = 1;
		pid = (pid_t) atoi(trozos[1]);
	} else 
		pid = (pid_t) atoi(trozos[0]);
	for (i = 0; i <= p->count; i++) {
		if(p->node[i].pid == pid) {
			if (b) {
				char *comando[MAX];
				TrocearCadena(p->node[i].cmd,comando);
				do_pplano(comando);
				deleteElementP(p,p->node[i].pid);
				return;
			}
		}
	}
	do_listarprocs(p);
}


void do_borrarprocs(tListP *p, char *trozos[]) {

	if (trozos[0]==NULL) {
		cmd_error();
		return;
	}
	int i;

	if (!strcmp(trozos[1],"-term")) {
		for (i = 0; i < p->count; i++) {
			char *st = p->node[i].status;
			if (!strcmp(st,"finished"))
				deleteElementP(p,p->node[i].pid);
		}
	} else if (!strcmp(trozos[1],"-sig")) {
		for (i = 0; i < p->count; i++) {
			char *st = p->node[i].status;
			if (!strcmp(st,"finished by signal"))
				deleteElementP(p,p->node[i].pid);
		}
	}
}