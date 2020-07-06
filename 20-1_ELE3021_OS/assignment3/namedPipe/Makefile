CC = gcc

All: reader_Aprocess writer_Bprocess

reader_Aprocess : reader_Aprocess.o
	gcc -o reader_Aprocess reader_Aprocess.o

reader_Aprocess.o : reader_Aprocess.c
	gcc -c -o reader_Aprocess.o reader_Aprocess.c

writer_Bprocess : writer_Bprocess.o
	gcc -o writer_Bprocess writer_Bprocess.o

writer_Bprocess.o : writer_Bprocess.c
	gcc -c -o writer_Bprocess.o writer_Bprocess.c

clean :
	rm reader_Aprocess.o
	rm reader_Aprocess
	rm writer_Bprocess.o
	rm writer_Bprocess
	rm named_pipe_file
