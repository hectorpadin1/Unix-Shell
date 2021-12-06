# Unix-Shell
A customized shell for Unix enviroments with some customized programs.

Commands:
* **autores** [-l|-n] Prints the names and logins of the program authors. authors -l prints only the logins and authors -n prints only the names
* **pid** [-p] Prints the pid of the process executing the shell.
  * pid -p rints the pid of the shell’s parent process.
* **carpeta** [direct] Changes the current working directory of the shell to direct (using the chdir system call). When invoked without auguments it prints the current working directory (using the getcwd system call.
* **fecha** [-d|-h] Without arguments it prints both the current date and the current time. fecha -d prints the current date in the format DD/MM/YYYY. fecha -h prints the current time in the format hh:mm:ss.
* **hist** [-c|-N] Shows/clears the historic of commands executed by this shell.
  * hist Prints all the comands that have been input with their order number.
  * hist -c Clears (empties) the list of historic commands.
  * hist -N Prints the first N comands
* **comando** N Repeats command number N.
* **crear** [-f ] name Creates a file or directory in the file system.
* **borrar** name1 name2 . . . Deletes files and/or empty directories.
* **borrarrec** name1 name2 . . . Deletes files and/or non empty directories.
* **listfich** [-long] [-link] [-acc] name1 name2 name3 . . . Gives info on files (or directories, or devices . . . ) name1, name2 . . . in ONE LINE per file.
* **listdir** [-reca] [-recb] [-hid] [-long] [-link] [-acc] name1 name2 . . . Lists the contents of directories with names name1, name2 . . . . If any of name1, name2 . . . is not a directory, info on it will be printed EXACTLY as with command listfich.
* **malloc** [-free] [tam] The shell allocates tam bytes using malloc and shows the memory address returned by malloc. If used as malloc -free [tam] The shell deallocates one of the blocks of size tam that has been allocated with the command malloc.
* **mmap** [-free] fich [perm] Maps in memory the file fich (all of its length starting at offset 0) and shows the memory address where the file has been mapped. perm represents the mapping permissions (rwx format, without spaces).
* **shared** [-free|-create|-delkey ] cl [tam] Gets shared memory of key cl, maps it in the proccess address space and shows the memory address where the shared memory has been mapped.
* **dealloc** [-malloc|-shared|-mmap] . . . deallocates one of the memory blocks allocated with the command malloc, mmap or shared and removes it from the list.
* **memoria** [-blocks] [-vars] [-funcs] [-all] [-pmap] Shows addresses inside the process memory space. 
* **volcarmem** addr [cont] Shows the contents of cont bytes starting at memory address addr.
* **llenarmem** addr [cont] [byte] Fills cont bytes of memory starting at address addr with the value ’byte’.
* **recursiva** n. Calls a recursive function passing the integer n n as its parameter.
* **priority** [pid] [value]. if value is not given shows the priority of process pid.
* **rederr** [-reset] fich
  * rederr file Redirects the standard error of the shell to file fich.
  * rederr Shows were the standard error is currently going to.
  * rederr -reset Restores the standard error to what it was originally.
* **entorno** [-environ] Shows the environment of the shell process.
* **uid** -get|-set [-l] id
  * uid -get o simply uid Prints the real and effective user credentialas of the process running the shell (both the number and the associated login)
  * uid -set [-l] id Establishes the efective user id of the shell process (see notes on uids).
* **fork** The shell creates a child process.
* **ejec** prog arg1 arg2 . . . Executes, without creating a process (REPLACING the shell’s code) the program prog with its arguments.
* **ejecpri** prio prog arg1 arg2. . . Does the same as the previous execute command, but before executing prog it changes the priority of the proccess to prio.
* **fg** prog arg1 arg2. . . The shell creates a process that executes in foreground.
* **fgpri** prio prog arg1 arg2. . . Does the same as the previous command, but before executing prog it changes the priority of the proccess that executes prog to prio.
* **back** prog arg1 arg2. . . The shell creates a process that executes in background the program prog with its arguments.
* **backpri** prio prog arg1 arg2. . . Does the same as the previous command, but before executing prog it changes the priority of the proccess that executes prog to prio. 
* **listjobs** Shows the list of background processes of the shell.
* **job** [-fg] id Shows information on process pid.
* **borrarjobs** -term|-sig|-all|-clear.
