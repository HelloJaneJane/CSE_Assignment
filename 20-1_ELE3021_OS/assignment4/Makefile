CC = gcc

All: multithread_practice multithread_practice_solution

multithread_practice: multithread_practice.o
	gcc -o multithread_practice multithread_practice.o

multithread_practice.o: multithread_practice.c
	gcc -c -o multithread_practice.o multithread_practice.c

multithread_practice_solution: multithread_practice_solution.o
	gcc -o multithread_practice_solution multithread_practice_solution.o -lpthread

multithread_practice_solution.o: multithread_practice_solution.c
	gcc -c -o multithread_practice_solution.o multithread_practice_solution.c -lpthread

clean :
	rm multithread_practice.o
	rm multithread_practice
	rm multithread_practice_solution.o
	rm multithread_practice_solution
