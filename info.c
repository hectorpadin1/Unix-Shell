/*  
 *  authors : Hector Padin Torrente, Luis Cascon Padron
 *  logins  : hector.padin@udc.es, luis.cascon@udc.es
 *  group   : 1.4
 *
 */

#include "info_&_listar.h"



char TipoFichero(mode_t m) {

	switch (m&S_IFMT) {  /*and bit a bit con los bits de formato,0170000 */
		case S_IFSOCK:   return 's'; /*socket */
		case S_IFLNK:    return 'l'; /*symbolic link*/
		case S_IFREG:    return '-'; /*fichero normal*/
		case S_IFBLK:    return 'b'; /*block device*/
		case S_IFDIR:    return 'd'; /*directorio */
		case S_IFCHR:    return 'c'; /*char device*/
		case S_IFIFO:    return 'p'; /*pipe*/
		default: return  '?';        /*desconocido, no deberia aparecer*/
	}
}


char * ConvierteModo (mode_t m, char *permisos) {

	strcpy (permisos,"---------- ");
	permisos[0]=TipoFichero(m);
	if (m&S_IRUSR) permisos[1] = 'r';	/*propietario*/
	if (m&S_IWUSR) permisos[2] = 'w';
	if (m&S_IXUSR) permisos[3] = 'x';
	if (m&S_IRGRP) permisos[4] = 'r';	/*grupo*/
	if (m&S_IWGRP) permisos[5] = 'w';
	if (m&S_IXGRP) permisos[6] = 'x';
	if (m&S_IROTH) permisos[7] = 'r';	/*resto*/
	if (m&S_IWOTH) permisos[8] = 'w';
	if (m&S_IXOTH) permisos[9] = 'x';
	if (m&S_ISUID) permisos[3] = 's';	/*setuid, setgid, stickybit*/
	if (m&S_ISGID) permisos[6] = 's';
	if (m&S_ISVTX) permisos[9] = 't';
	return permisos;
}


/*prints information for the given file*/
void muestraInfo(int l, char *path, char *file) {
	struct stat st;
	char buffer[MAX];

	/*buffer saves the whole path to file*/
	if (file == NULL) {
		sprintf(buffer, "%s", path);
		file = path;
	} else sprintf(buffer, "%s/%s", path, file);

	if (-1 == lstat(buffer,&st)) {
		printf("cannot access to %s: %s\n", buffer, strerror(errno));
		return;
	}

	if (l == 1) {	/*long listing*/
		/*printing the inode number*/
		printf("%lu ", (unsigned long) st.st_ino);

		/*gets file permissions and show them*/
		char modo[12];
		ConvierteModo(st.st_mode,modo);
		printf("%s ", modo);

		/*shows the number of hard links to the file*/
		printf("%ld ", st.st_nlink);

		/*getting user ID of the file’s owner and printing it*/
		struct passwd *usr = getpwuid(st.st_uid);
		if (usr) {
			printf("%s ", usr -> pw_name);
		} else printf("%lu ", (unsigned long)st.st_uid);

		/*gets group ID of the file and prints it*/
		struct group *grp = getgrgid(st.st_gid);
		if (grp) 
			printf("%s ", grp -> gr_name);
		else printf("%lu ", (unsigned long) st.st_gid);

		/*prints the size of file*/
		printf("%ld ", st.st_size);
		
		/*shows time of last modification*/
		char *s = ctime(&st.st_mtime);
		strtok(s,"\n");
		printf("%s ", s);

		/*prints the actual path to the file*/
		if (file != NULL) {
			printf("%s ", file);
		} else printf("%s ", buffer);
		/*if the file is a symbolic link*/
		if (modo[0] == 'l') {
			char link[st.st_size+1]; 

	   	    readlink(buffer,link,st.st_size);
	       	link[st.st_size]='\0';
			printf("-> %s\n", link);
		} else printf("\n");
	} else {
		/*short listing*/
		/*prints the name of the file*/
		printf("%s ", file);

		/*prints the size of file*/
		printf("%ld\n", st.st_size);
	}
}


/*recursively prints the information for the given files*/
void do_info(char *trozos[]) {
	int i = 0;

	while (trozos[i] != NULL) {
		muestraInfo(1,trozos[i],NULL);
		i++;
	}
}