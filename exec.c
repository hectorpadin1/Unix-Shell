
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <grp.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char *argv[]) {
	int i = 0;

	while (argv[i]!=NULL) {
		printf("%s ", argv[i]);
		i++;
	}
	printf("\n");
	pid_t pid = getpid();
	int priority = getpriority(PRIO_PROCESS,pid);
	printf("priority of process:%d\n", priority);
	printf("pid of process:%d\n", pid);
	printf("pid of parent process:%d\n", getppid());
	exit(0);
}