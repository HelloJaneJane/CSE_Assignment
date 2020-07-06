CC = g++

peterson_algorithm: peterson_algorithm.o
	g++ -o peterson_algorithm peterson_algorithm.o -lpthread

peterson_algorithm.o: peterson_algorithm.cpp
	g++ -c -o peterson_algorithm.o peterson_algorithm.cpp -lpthread

clean:
	rm peterson_algorithm.o
	rm peterson_algorithm