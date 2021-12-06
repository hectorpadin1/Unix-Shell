/*  
 *  authors : Hector Padin Torrente, Luis Cascon Padron
 *  logins  : hector.padin@udc.es, luis.cascon@udc.es
 *  group   : 1.4
 *
 */

#include "func.h"


/*prints an error*/
void cmd_error() {

	printf("error: invalid command.\n");
	printf("type 'help' for a list of valid commands in the shell.\n");
}


/*shows the authors of the program*/
void do_autores(char * cmd) {
	
	if (cmd == NULL){
		printf ("authors : Hector Padin Torrente, Luis Cascon Padron \n");
		printf("logins  : hector.padin@udc.es, luis.cascon@udc.es. \n");
	}
	else if (!strcmp("-n", cmd)){
		printf ("authors : Hector Padin Torrente, Luis Cascon Padron. \n");

	}
	else if (!strcmp ("-l", cmd)){
		printf("logins  : hector.padin@udc.es, luis.cascon@udc.es.\n");

	} else 	cmd_error();
}


int TrocearCadena(char * cadena, char * trozos[]) {
	int i=1;
  
  	if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
    	return 0;
  	while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
    	i++;
  	return i;
}


/*prints the pid, and fathers pid*/
void do_pid (char * cmd) {
	
	if (cmd == NULL) {
		printf("pid of shell: %d\n", getpid());
	} else
	if (!strcmp(cmd,"-p")) {
		printf("pid of shell's parent process: %d\n", getppid());
	} else 	cmd_error();
}


/*prints actual directory*/
void do_actual() {
  	char dir[MAXENTRADA];

	if (getcwd(dir, sizeof(dir)) != NULL)
      	printf("current working directory: %s\n", dir);
  	else
      	perror("FATAL ERROR: actual directory could not be found. Maybe try again ¿?\n");
}

 
/*changes the directory*/
void do_cdir(char * dir) {

  	if (dir == NULL) {
    	do_actual();
  	} else if (chdir(dir) != -1) {
  		do_cdir(NULL);
  	} else {
  		perror("error:");
  		printf("\nyou may have tried to access to an non-existant directoy or typed an unvalid command.\n");
  		printf("press 'help' to see a list of valid commands at the shell.\n");
  	}
}


/*prints the system date*/
void do_fecha() {
	time_t tiempo = time(0);
    struct tm * time = localtime(&tiempo);
    char output[128];

    strftime(output, 128, "current date: %d/%m/%y", time);
    printf("%s\n", output);
}


/*prints the system time*/
void do_hora() {	
	time_t tiempo = time(0);
    struct tm * time = localtime(&tiempo);
    char output[128];

    strftime(output, 128, "current time: %H:%M:%S", time);
    printf("%s\n", output);
}

 /*does different options for the command hist*/
void do_hist(char * cmd, tHist *h) {
	
	if (cmd == NULL) {
		showHist(h);
	} else
	if (!strcmp(cmd,"-c")) {
		clearHist(h);
	} else 	cmd_error();
}


/*creates an empty file*/
void createFile(char *file_name) {
	
	FILE *file = fopen(file_name, "w");		// we use fopen with option "w" to create a new file
	fclose(file);			// then we close it
	if (file != NULL) {			// file existence is checked
		printf("file %s was created in the actual directory.\n" , file_name);
	} else printf("file coudl not be created.\n");
}


/*creates a new directory*/
void createDir(char *dir_name) {
	char cmd[MAXENTRADA];
	
	strcpy(cmd,"mkdir ");strcat(cmd, dir_name);		// command is created
    system(cmd);			// directory is created, if it was not, will show an error message
}


/*options for the command crear*/
void do_crear(char *trozo1, char *trozo2) {
	
	if (trozo1 == NULL) {
		printf("error: cannot create a file with empty name.\n");
		listar(0,0,0,"."); 	
	} else {
		if (!strcmp(trozo1,"-d")) {
			if (trozo2 == NULL) {
				printf("error: cannot create a directory with empty name.\n");
				listar(0,0,0,".");
			} else createDir(trozo2);
		} else if (trozo2 == NULL) {
				createFile(trozo1);
		} else  cmd_error();
	} 
}



/*removes a file*/
void rmFile(char *trozo1, char *trozo2) {

	if (!strcmp(trozo1,"-r")) {				// if arg -r is given, the program does the same
		if (access(trozo2, F_OK) != -1) {	// check's file existence
			if (remove(trozo2) == 0)		// removes the file
    			printf("file %s deleted successfully.\n", trozo2);
  			else {
    			printf("unable to delete the file.\n");
    			perror("following error occurred: ");
			}
		} else {
			printf("file was not found in the actual directory.\n"); // this message wille be shown if user introduces a non valid directory 
			listar(0,0,0,".");
		}
	} else if (!access(trozo1, F_OK)) {		// check's file existence
		if (remove(trozo1) == 0)		// removes the file
    		printf("file %s deleted successfully.\n", trozo1);
  		else {
    		printf("unable to delete the file.\n");
    		perror("following error occurred");
		}
	} else {
		printf("file was not found in the actual directory.\n"); // this message wille be shown if user introduces a non valid directory 
		listar(0,0,0,".");
	}
}


/*removes a directory*/
void rmDir(char *trozo1, char *trozo2) {

	// for command -r <dir>
	if (!strcmp(trozo1,"-r") && (trozo2 != NULL)) { 	// remove directory and all of it's files
		DIR *d = opendir(trozo2);		// directory is opened
		size_t trozo2_len = strlen(trozo2);	

		if (d) {
			struct dirent *dir;
			char *f;
			size_t len;
			
			while ((dir = readdir(d))) {	// dir reads an entry from directory

				if (!strcmp(dir -> d_name,".") || (!strcmp(dir -> d_name,"..")))	// files "." and ".." cannot be deleted
					continue;
				len = trozo2_len + strlen(dir -> d_name) + 2;
				f = malloc(len);
				if (f) {
					struct stat st;
					snprintf(f, len,"%s/%s", trozo2, dir -> d_name);	// route is saved in f
					if (!stat(f, &st)) {
						if (S_ISDIR(st.st_mode))
							rmDir("-r",f);
						else unlink(f);
					}
				}
				free(f);
			}
			closedir(d);	// closes the directory
		}
		if (!rmdir(trozo2))		//removes the directory if it's empty
			printf("directory '%s' deleted successfully.\n", trozo2);
	} 
	// for command <dir>
	else if (!rmdir(trozo1)) {	//removes the directory if it's empty
		printf("directory '%s' deleted successfully.\n", trozo1);
	} else perror("following error occurred when trying to delete the directory");
}


/*options for command borrar*/
void do_borrar(char *trozo1, char *trozo2) {

	if (trozo1 == NULL) {
		printf("error: no name given.\n");
		listar(0,0,0,".");	
	}
	else {
		if (dirExists(trozo1)) { 
			rmDir(trozo1,trozo2);
		} else if (dirExists(trozo2)) {
			rmDir(trozo1,trozo2);
		} else rmFile(trozo1,trozo2);
	}
}


/*finish the shell*/
void do_exit(char * cmd, tHist *h, tList *l, tListP *p) {
	
	if(cmd != NULL){
		cmd_error();
	} else {
		clearHist(h);
		clearList(l);
		clearListP(p);
		exit(0);
	}
}


/*help command*/
void do_help(char * cmd) {
	
	if (cmd == NULL) {
		printf("List of valid commands:\n\n");
		printf("autores [-l|-n]: prints the names and logins of the program authors.\n");
		printf("\t-l: prints only the logins.\n");
		printf("\t-n: prints only the names.\n");
		printf("pid [-p]: prints the pid of the process executing the shell.\n");
		printf("\t-p: prints the pid of the parent process.\n");
		printf("cdir <path>: prints the current working directory of the shell.\n");
		printf("\t'path': changes the current working directory of the shell to new one.\n");
		printf("fecha: prints the current date.\n");
		printf("hora: prints the current time.\n");
		printf("hist [-c]: shows the historic of commands executed.\n");
		printf("\t-c: clears the historic.\n");
		printf("crear [-d] <name>: creates a new empty file.\n");
		printf("\t-d: creates a empty directory.\n");
		printf("borrar [-r] <name>: deletes a file, or directory(if it is not empty).\n");
		printf("\t-r: deletes non empty directories.\n");
		printf("info <name1> <name2> ...: gives info on the files and/or directories supplied as its arguments.\n");
		printf("listar: lists the directories and/or files supplied to it as command line arguments.\n");
		printf("\t-l: detailed information of files.\n");
		printf("\t-v: avoids listing hidden elements.\n");
		printf("\t-r: directories will be listed recursively.\n");
		printf("asignar: allocates memory in the shell.\n");
		printf("\t-malloc [tam]: The shell allocates tam bytes using malloc and shows the memory address returned by malloc.\n");
		printf("\t-mmap fich [perm]: Maps in memory the file fich (all of its length starting at offset 0) and shows the memory address where the file has been mapped.\n");
		printf("\t-crearshared [cl] [tam]: Gets shared memory of key cl, maps it in the proccess address space and shows the memory address where shared memory has been mapped.\n");
		printf("\t-shared] [cl]: Gets shared memory of key cl, maps it in the proccess address space and shows the memory address where the shared memory has been mapped.\n");
		printf("desasignar: deallocates one of the memory blocks allocated with the command allocate and removes it from the list.\n");
		printf("\t-malloc [tam]: The shell deallocates one of the blocks of size tam that has been allocated with the command asignar -malloc.\n");
		printf("\t-mmap fich: Unmaps and closes the file fich and removes the address where it was mapped from the list.\n");
		printf("\t[-shared] [cl]: Detaches the shared memory block with key cl from the process’ address space ad eliminates its address from the list.\n");
		printf("\t-addr: Deallocates addr (it searchs in the list how it was allocated, and proceeds accordingly) and removes it from the list.\n");
		printf("borrarkey cl Removes the shared memory region of key cl.\n");
		printf("mem: Shows addresses inside the process memory space.\n");
		printf("\t-malloc: shows the list of addresses (and size, and time . . . ) allocated with the asignar -malloc.\n");
		printf("\t-shared: shows the list of addresses (and size, and time . . . ) allocated with the asignar -createshared and asignar -shared commands.\n");
		printf("\t-mmap: shows the list of addresses (and size, and time . . . ) allocated with the asignar -mmap.\n");
		printf("\t-all: shows the list of addresses (and size, and time . . . ) allocated with the asignar -malloc, asignar -mmap, asignar -createshared and asignar -shared commands.\n");
		printf("volcar addr [cont]: Shows the contents of cont bytes starting at memory address addr.\n");
		printf("llenar addr [cont] [byte]: Fills cont bytes of memory starting at address addr with the value ’byte’.\n");
		printf("recursiva n: Calls a recursive function passing the integer n n as its parameter.\n");
		printf("rfich fich addr cont: Reads cont bytes from file fich into memory address addr. \n");
		printf("wfich [-o] fich addr cont: Writes cont bytes from memory address addr into file fich. If file fich does not exist it gets created, if it already exists it is not overwritten unless “-o” \n");
		printf("fin: ends the shell.\n");
		printf("end: ends the shell.\n");
		printf("exit: ends the shell.\n\n");
 	} else 
 		cmd_error();
}
