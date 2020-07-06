CC = gcc

All: child parent

child: child.o
	gcc -o child child.o

child.o: child.c
	gcc -c -o child.o child.c

parent: parent.o
	gcc -o parent parent.o

parent.o: parent.c
	gcc -c -o parent.o parent.c

clean:
	rm child.o
	rm child
	rm parent.o
	rm parent