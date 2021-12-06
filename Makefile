CFLAGS = -Wall -g -O0
CC = gcc

all: clean shell listMain infoMain


shell:	func.o hist.o list.o listP.o info.o listar.o mem.o proc.o
	$(CC) $(CFLAGS) -o shell shell.c func.o hist.o list.o listP.o info.o listar.o mem.o proc.o

listMain:	listar.o info.o
	$(CC) $(CFLAGS) -o listar listMain.c listar.o info.o

infoMain:	info.o
	$(CC) $(CFLAGS) -o info infoMain.c info.o


func.o:
	$(CC) $(CFLAGS) -c func.c

hist.o:
	$(CC) $(CFLAGS) -c hist.c

list.o:
	$(CC) $(CFLAGS) -c list.c

listP.o:
	$(CC) $(CFLAGS) -c listP.c

info.o:
	$(CC) $(CFLAGS) -c info.c

listar.o:
	$(CC) $(CFLAGS) -c listar.c

mem.o:
	$(CC) $(CFLAGS) -c mem.c

proc.o:
	$(CC) $(CFLAGS) -c proc.c


clean:
	rm -f *.o shell listar info

tar:
	tar czvf p3.tar.gz *.c *.h Makefile
