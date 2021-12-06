/*  
 *  authors : Hector Padin Torrente, Luis Cascon Padron
 *  logins  : hector.padin@udc.es, luis.cascon@udc.es
 *  group   : 1.4
 *
 */

#include "info_&_listar.h"

/*checks the existence of a directory*/
int dirExists(char *path) {
    struct stat stats;

    if (!lstat(path, &stats)) {
    	if (S_ISDIR(stats.st_mode))		// check's for file existence
        	return 1;
    }
    return 0;
}



/*prints files in a directory*/
void listar(int r, int l, int v, char *path) {
  DIR *dir;
  struct dirent *file;
  

  /*directory is opened*/
  if ((dir = opendir(path))) {
    printf("*******************/%s\n", path);             //prints some * and the current directory 
    while ((file = readdir(dir)) != NULL) {               //all files are read
      if((v != 0) && (file -> d_name[0] == '.'))          //if it is a hidden file and option -v is activated, will not show that file
        continue;
      muestraInfo(l, path, file -> d_name);               //prints info for the file
    }
    
    if (r) {                       //if option -r was activated, directories will be listed recursively
      size_t path_len = strlen(path); 
      size_t len;
      char *ruta;

      rewinddir(dir);
      while ((file = readdir(dir)) != NULL) {
        if((v != 0) && (file -> d_name[0] == '.')){
          continue;
        }
        if (!strcmp(file -> d_name, ".") || (!strcmp(file -> d_name, ".."))) {
          continue; 
        }
        len = path_len + strlen(file -> d_name) + 2;
        ruta = malloc(len);
        snprintf(ruta, len,"%s/%s", path, file -> d_name);
        if (dirExists(ruta))             //if the file is a directory, will be listed
          listar(r,l,v,ruta);
        free(ruta);
      }
    }
    closedir(dir);
    /*directory is closed*/
  } else {
    char *directory = ".";
    printf("cannot access to %s: %s\n", path, strerror(errno));
    listar(r,l,v,directory);                      //if could not access to the directory, actual one will be printed
  }
}


/*options for command listar*/
void do_listar(char *trozos[]) {
  int i = 0, l = 0, v = 0, r = 0;
	
	while (trozos[i] != NULL) {
		if(!strcmp(trozos[i],"-l")) {    //option -l activated
			l = 1;
    } else if(!strcmp(trozos[i],"-v")) {    //option -v activated
    	v = 1;
    } else if (!strcmp(trozos[i],"-r")) {   //option -r activated
     	r = 1;
    } else break;
    i++;
  }

  if (trozos[i] != NULL) {   //if trozos[i] != NULL, there are directories to list.
      while (trozos[i] != NULL) {     //all arguments are listed
        listar(r, l, v, trozos[i]);
        i++;
      }
  } else listar(r, l, v, "."); 
  //if trozos[i] == NULL, means that there are no directories to list so actual directory is printed instead
}
