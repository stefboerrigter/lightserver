CC?=gcc
CFLAGS=-Wall

all: lightserver
lightserver.o: main.o
	${CC} -c main.c -o main.o
lightserver: main.o
	${CC} -o lightserver main.o

clean:
	rm -f *.o lightserver