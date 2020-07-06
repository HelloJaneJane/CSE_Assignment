CC = gcc

All: assignmentA assignmentB

assignmentA: assignmentA.o
	gcc -o assignmentA assignmentA.o -lpthread
	
assignmentA.o: assignmentA.c
	gcc -c -o assignmentA.o assignmentA.c -lpthread

assignmentB: assignmentB.o
	gcc -o assignmentB assignmentB.o -lpthread
	
assignmentB.o: assignmentB.c
	gcc -c -o assignmentB.o assignmentB.c -lpthread


clean:
	rm assignmentA.o
	rm assignmentA
	rm assignmentB.o
	rm assignmentB