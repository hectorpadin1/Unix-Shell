#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include "info_&_listar.h"

int main(int argc, char * argv[]) {
	
	do_listar(&argv[1]);
	
}
