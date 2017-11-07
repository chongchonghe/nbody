CC = gcc
FLAG = -O2

all: main

main: main.o
	${CC} ${FLAG} -o main main.o

main.o: Makefile

clean:
	-rm -f *.o *~ main
