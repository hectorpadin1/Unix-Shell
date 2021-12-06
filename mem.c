/*  
 *  authors : Hector Padin Torrente, Luis Cascon Padron
 *  logins  : hector.padin@udc.es, luis.cascon@udc.es
 *  group   : 1.4
 *
 */

#include "mem.h"


void * findMalloc(tList *l, size_t tam) {

	for (int i = 0; i < l->count; i++) {
		if (l->nodo[i].mem_type==MMALLOC && l->nodo[i].size==tam)
			return l->nodo[i].address;
		i++;
	}
	return NULL;
}


void Malloc(char *trozos[], tList *l, int b) {

	if (l) {
		if (trozos[0]!=NULL && trozos[1]!=NULL) {
			if (b) {	// allocate
				size_t size = atoi(trozos[1]);
				void *addr;

				if (size!=0 && (addr=malloc(size))!=NULL) {
					if (insertElementL(l,MMALLOC,0,addr,"",0,size))
						printf("allocated %lu at %p\n", (unsigned long)size, addr);
					else printf("error: element could not be inserted at the list.\n");
				}
			} else {	// deallocate
				size_t size = atoi(trozos[1]);
				void *addr = findMalloc(l,size);

				if (addr) {
					printf("deallocated %lu at %p\n", (unsigned long)size, addr);
					if (!deleteElement(l,addr))
						printf("error: element could not be deleted from list.\n");
				} else printf("error: element could not be found.\n");
			}
		}  else showList(l,MMALLOC);
	} else printf("error: list does not exist.\n");
}


void * findMMapA(tList *l, char *file) {

	for (int i = 0; i < l->count; i++) {
		if (l->nodo[i].mem_type==MMAP && !strcmp(l->nodo[i].file,file))
			return l->nodo[i].address;
	}
	return NULL;
}


size_t findMMapS(tList *l, void *a) {

	for (int i = 0; i < l->count; i++) {
		if (l->nodo[i].mem_type==MMAP && l->nodo[i].address==a)
			return l->nodo[i].size;
	}
	return 0;
}


void *MmapFichero(tList *l, char *arg[], int protection) {
	int df,map=MAP_PRIVATE, modo=O_RDONLY;
	struct stat s;
	void *p;

	if (protection&PROT_WRITE)
		modo=O_RDWR;
	if (stat(arg[0],&s)==-1 || (df=open(arg[0], modo))==-1)
		return NULL;
	if ((p=mmap(NULL,s.st_size,protection,map,df,0))==MAP_FAILED)
		return NULL;
	else {
		int x = 0;
		if (arg[1]!=NULL)
			x = atoi(arg[1]);
		insertElementL(l,MMAP,x,p,arg[0],df,s.st_size);
	}
	return p;
}


void Cmd_AsignarMmap(tList *l, char *arg[]) {
	char *perm;
	void *p;
	int protection=0;

	if (arg[0]==NULL) {
		showList(l,MMAP);
		return;
	}
	if ((perm=arg[1])!=NULL && strlen(perm)<4) {
		if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
		if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
		if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
	}
	if ((p=MmapFichero(l,&arg[0],protection))==NULL)
		perror("file cannot be mapped");
	else 
		printf("file %s mapped at %p\n", arg[0], p);
}


void Cmd_DesasignarMmap(tList *l, char *trozos[]) {

	if (l) {
		if (trozos[0]!=NULL) {
			void *p = findMMapA(l,trozos[0]);
			if (p && deleteElement(l,p)) {
				printf("block at address %p deallocated (mmap)\n",p);
			} else printf("error: file could not be found.\n");
		} else showList(l,MMAP);
	} else printf("error: list of memory addresses does not exist.\n");
}


void Mmap(char *trozos[], tList *l, int b) {

	if (l) {
		if (b)
			Cmd_AsignarMmap(l,&trozos[0]);
		else Cmd_DesasignarMmap(l,&trozos[0]);
	} else printf("error: list does not exist.\n");
}


void * findShared(tList *l, key_t k) {

	for (int i = 0; i < l->count; i++) {
		if (l->nodo[i].key==k) {
			return l->nodo[i].address;
		}
	}
	return NULL;
}


size_t findShTam(tList *l, key_t k) {

	for (int i = 0; i < l->count; i++) {
		if (l->nodo[i].key==k) {
			return l->nodo[i].size;
		}
	}
	return (size_t)-1;
}


void * ObtenerMemoriaShmget(tList *l, key_t clave, size_t tam){
	void * p;
	int aux,id,flags = 0777;
	struct shmid_ds s;
	if(tam) //Si tam no es 0 la crea en modo exclusivo
		flags=flags | IPC_CREAT | IPC_EXCL;
	if(clave==IPC_PRIVATE){
		errno=EINVAL;
		return NULL;
	}
	if((id=shmget(clave,tam,flags))==-1)
			return (NULL);
	if ((p=shmat(id,NULL,0))==(void*) -1){
		aux=errno;	/*si se ha creado y no se puede mapear*/
		if (tam)	/*se borra*/
			shmctl(id,IPC_RMID,NULL);
		errno=aux;
		return (NULL);
	}
	if (shmctl(id,IPC_STAT,&s)!=-1){
		if (tam == 0)
			tam = s.shm_segsz;
		if (!insertElementL(l,MSHARED,0,p,"",clave,tam))
			printf("error: element could not be inserted in in the list.\n");
	}
	return (p);
}


void Cmd_AsignarCreateShared(tList *l, char *arg[]) {
	key_t k;
	size_t tam=0;
	void *p;

	if (arg[0]==NULL || arg[1]==NULL){
		showList(l,MSHARED);
		return;
	}
	k=(key_t) atoi(arg[0]);
	if (arg[1]!=NULL)
		tam=(size_t) atoll(arg[1]);
	if ((p=ObtenerMemoriaShmget(l,k,tam))==NULL)
		perror("mem shmget could not be taken");
	else printf("allocated shared memory (key %d) at %p\n",k,p);
}


void ShCreate(char *trozos[], tList *l){
	if (trozos[0]!=NULL) {
		Cmd_AsignarCreateShared(l,&trozos[1]);
	} else showList(l,MSHARED);
}


void shAllocate(tList *l, char *trozos[]) {
	key_t k = atoi(trozos[0]);
	void *p = ObtenerMemoriaShmget(l,k,0);

	if (p)
		printf("allocated shared memory (key %d) at %p\n",k,p);
	else perror("mem shmget could not be taken");
}


void shDeallocate(tList *l, char *trozos[]) {
	key_t k = atoi(trozos[0]);
	void *p = findShared(l,k);

	if (p) {
		printf("block at address %p deallocated\n", p);
		deleteElement(l,p);
	} else perror("error");
}


void Shared(char *trozos[], tList *l, int b) {

	if (trozos[0]!=NULL) {
		if (b)
			shAllocate(l,&trozos[0]);
		else shDeallocate(l,&trozos[0]);
	} else showList(l,MSHARED);
}


void do_asignar(tList *l, char *trozos[]) {

 	if (trozos[0]!=NULL) {
		if (!strcmp(trozos[0],"-malloc"))
	  		Malloc(trozos,l,1);		//pasarle un int para saber si hacer allocate o deallocate
	  	else if (!strcmp(trozos[0], "-mmap"))
	  		Mmap(&trozos[1],l,1);
		else if (!strcmp(trozos[0], "-crearshared"))
	  		ShCreate(trozos,l);
	 	else if(!strcmp(trozos[0], "-shared"))
	  		Shared(&trozos[1],l,1);
	  	else if(!strcmp(trozos[0], "-all"))
	  		showList(l,ALL);
	  	else cmd_error();
  } else showList(l,ALL);
}


void desasignarAddr(char *trozos[], tList *l) {

	if (l==NULL) {
		printf("error: list does not exist.\n");
	} else if (trozos[0]!=NULL) {
		void *p = (void*) strtoull(trozos[0],NULL,16);
		tItem t;
		
		for (int i = 0; i < l->count; i++) {
			if (l->nodo[i].address==p) {
				t = l->nodo[i];
			}
		}
		if (deleteElement(l,p)) {
			printf("block at address %p deallocated ", p);
			if (t.mem_type==MMALLOC) 
				printf("(malloc)\n");
			else if (t.mem_type==MMAP) 
				printf("(mmap)\n");
			else if (t.mem_type==MSHARED) 
				printf("(shared)\n");
		} else showList(l,ALL);
	} else showList(l,ALL);
}


void do_desasignar(tList *l, char *trozos[]) {
	
	if (trozos[0]!=NULL) {
	  	if (!strcmp(trozos[0],"-malloc"))
	  		Malloc(trozos,l,0);		//pasarle un int para saber si hacer allocate o deallocate
	  	else if (!strcmp(trozos[0], "-mmap"))
	  		Mmap(&trozos[1],l,0);
	  	else if(!strcmp(trozos[0], "-shared"))
	  		Shared(&trozos[1],l,0);
	 	else desasignarAddr(&trozos[0],l);
	} else showList(l,ALL);
}


void Cmd_borrakey (char *args[]){
	key_t clave;
	int id;
	char *key = args[0];

	if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
		printf("rmkey: unvalid key.\n");
		return;
	}
	if ((id=shmget(clave,0,0666))==-1){
		perror ("shmget: shared memory could not be taken");
		return;
	}
	if (shmctl(id,IPC_RMID,NULL)==-1)
		perror("shmctl: shared memory could not be deleted");

}


void do_borrarkey(tList *l, char *trozos[]) {

	if (l) {
		if (trozos[0]!=NULL)
			Cmd_borrakey(&trozos[0]);
		else printf("error: a not valid key was introduced.\n");
	} else printf("error: list does not exist.\n");
}


void do_mem(tList *l, char *type[]) {

	if (l) {	// if the list exists
		if (type[0]!=NULL) {
			if (!strcmp(type[0],"-malloc"))		// prints malloc addresses
				showList(l,MMALLOC);
			else if (!strcmp(type[0],"-shared") || !strcmp(type[0],"-crearshared"))	// prints only shared addresses
				showList(l,MSHARED);
			else if (!strcmp(type[0],"-mmap"))	// prints only mmap addresses
				showList(l,MMAP);
			else showList(l,ALL);	// if something different is given, prints the list
		} else showList(l,ALL);		// if no arguments given will print the whole list
	} else printf("error: list does not exist.\n");
}


void memdup(void *p, int count) {
	int nl = count/TAMLINEA;
	int r = count%TAMLINEA;
	int i = 0;
	unsigned char *c;

	while (i < nl) {
		c = (unsigned char *) p;
		for (int j = 0; j < TAMLINEA; j++) {
			if (!isprint(c[j])) 
				printf("    ");
			else printf("%4c", c[j]);
		}
		printf("\n");
		for (int j = 0; j < TAMLINEA; j++) {
			if (!isprint(c[j])) 
				printf("    ");
			else printf("%4x", c[j]);
		}
		printf("\n");
		i++;
		p += TAMLINEA;
	}
	c = (unsigned char *) p;
	for (int j = 0; j < r; j++) {
		if (!isprint(c[j])) 
			printf("    ");
		else printf("%4c", c[j]);
	}
	printf("\n");
	for (int j = 0; j < r; j++) {
		if (!isprint(c[j])) 
			printf("    ");
		else printf("%4x", c[j]);
	}
	printf("\n");
}


void do_volcar(char *trozos[]) {
	void *p = (void*) strtoull(trozos[0],NULL,16);
	int count = TAMLINEA;

	if (trozos[1]!=NULL) 
		count = atoi(trozos[1]);
	memdup(p,count);
}


void do_llenar(char *trozos[]) {
	unsigned long long *offset = (unsigned long long *) strtoull(trozos[0],NULL,16);
	char *p = (char *) offset;

	if (trozos[0]!=NULL) {
		char byte = 'A';
		int cont = 128;

		if (trozos[1]!=NULL) {
			cont =(int) atoi(trozos[1]);
			if (trozos[2]!=NULL) {
				char *c = trozos[2];
				byte = *c;
			}
		}
		for (int i = 0; i < cont; i++) {
			p[i] = byte;
		}
	} else printf("error: invalid arguments for function 'llenar'\n");
}


void recursiva(int n){
	char automatico[MAXENTRADA];
	static char estatico[MAXENTRADA];

	printf("parametro n: %d en %p\n",n ,&n);
	printf("array estatico en: %p\n",estatico);	
	printf("array automatico en: %p\n",automatico);
	if(n>0)
		recursiva(n-1);
}


void do_recursiva(char *trozos[]) {
	int n;

	if (trozos[0] != NULL) {
		n = atoi(trozos[0]);	// char * to int
		if (n >= 0)
			recursiva(n);
		else printf("error: invalid arguments for function ¨recursiva¨.\n");
	} else printf("error: invalid arguments for function ¨recursiva¨.\n");
}


#define LEERCOMPLETO ((ssize_t)-1)
ssize_t LeerFichero (char *fich, void *p, ssize_t n){
	ssize_t nleidos, tam=n;
	int df, aux;
	struct stat s;
	if (stat(fich,&s)==-1 || (df=open(fich,O_RDONLY))==-1) {
		return ((ssize_t)-1);
	}
	if (n==LEERCOMPLETO)
		tam=(ssize_t) s.st_size;
	if((nleidos=read(df,p,tam))==-1){
		aux=errno;
		close(df);
		errno=aux;
		return ((ssize_t)-1);
	}
	close (df);
	return (nleidos);
}


void do_rfich(char *trozos[]) {

	if (trozos[0]!=NULL && trozos[1]!=NULL) {
		void *p = (void *) strtoull(trozos[1],NULL,16);
		ssize_t c = -1;
		ssize_t b;

		if(trozos[2]!=NULL)
			c = (ssize_t) atoll(trozos[2]);
		if ((b=LeerFichero(trozos[0],p,c))!=(ssize_t)-1)
			printf("%lld bytes read from %s in %p\n",(long long)b,trozos[0],p);
		else printf("error: file %s can not be read.\n", trozos[0]);
	} else printf("error: invalid arguments for function 'rfich'\n");
}


ssize_t writeFich(char *file, void *p, size_t cont,int overwrite){
	ssize_t n;
	int df, aux, flags=O_CREAT | O_EXCL | O_WRONLY;

	if(overwrite)
		flags=O_CREAT | O_WRONLY | O_TRUNC;

	if((df=open(file,flags,0777))==-1)
    	return -1;

  	if((n=write(df,p,cont))==-1){
		aux=errno;
		close(df);
		errno=aux;
		return -1;
	}
	close(df);
	return n;
}


void do_wfich(char *trozos[]) {

	if (trozos[0]!=NULL && trozos[1]!=NULL && trozos[2]!=NULL) {
		void *p;
		ssize_t c = -1;
		ssize_t b;

		if (!strcmp(trozos[0],"-o")){
			p = (void *) strtoull(trozos[2],NULL,16);
			c = (ssize_t) atoll(trozos[3]);
		
			if ((b=writeFich(trozos[1],p,c,1)) != (ssize_t)-1)
				printf("%lld bytes writen from %p in %s\n",(long long)b,p,trozos[1]);
			else printf("error: invalid arguments for function 'rfich'\n");
		} else {
			p = (void *) strtoull(trozos[1],NULL,16);
			c = (ssize_t) atoll(trozos[2]);

			if ((b=writeFich(trozos[0],p,c,0))!=(ssize_t)-1)
				printf("%lld bytes writed from %p in %s\n",(long long)b,p,trozos[0]);
			else perror("error:\n");
		}
	} else printf("error: invalid arguments for function 'wfich'\n");
}