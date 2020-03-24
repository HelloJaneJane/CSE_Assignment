CXX=g++

All: simple_int_set message_book sorted_array draw_shape

simple_int_set: simple_int_set_main.o simple_int_set.o
	$(CXX) -o simple_int_set simple_int_set_main.o simple_int_set.o
simple_int_set_main.o: simple_int_set.h
	$(CXX) -std=c++0x -c simple_int_set_main.cc
simple_int_set.o: simple_int_set.h
	$(CXX) -std=c++0x -c simple_int_set.cc

message_book: message_book_main.o message_book.o
	$(CXX) -o message_book message_book_main.o message_book.o
message_book_main.o: message_book.h
	$(CXX) -std=c++0x -c message_book_main.cc
message_book.o: message_book.h
	$(CXX) -std=c++0x -c message_book.cc

sorted_array: sorted_array_main.o sorted_array.o
	$(CXX) -o sorted_array sorted_array_main.o sorted_array.o
sorted_array_main.o: sorted_array.h
	$(CXX) -std=c++0x -c sorted_array_main.cc
sorted_array.o: sorted_array.h
	$(CXX) -std=c++0x -c sorted_array.cc

draw_shape: draw_shape_main.o draw_shape.o
	$(CXX) -o draw_shape draw_shape_main.o draw_shape.o
draw_shape_main.o: draw_shape.h
	$(CXX) -std=c++0x -c draw_shape_main.cc
draw_shape.o: draw_shape.h
	$(CXX) -std=c++0x -c draw_shape.cc

clean:
	rm simple_int_set_main.o
	rm simple_int_set.o
	rm simple_int_set
	rm message_book_main.o
	rm message_book.o
	rm message_book
	rm sorted_array_main.o
	rm sorted_array.o
	rm sorted_array
	rm draw_shape_main.o
	rm draw_shape.o
	rm draw_shape
