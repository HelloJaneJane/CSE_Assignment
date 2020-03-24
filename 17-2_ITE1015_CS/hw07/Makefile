CXX=g++

All: draw_shape

draw_shape: draw_shape_main.o draw_shape.o
	$(CXX) -o draw_shape draw_shape_main.o draw_shape.o
draw_shape_main.o: draw_shape.h
	$(CXX) -std=c++0x -c draw_shape_main.cc
draw_shape.o: draw_shape.h
	$(CXX) -std=c++0x -c draw_shape.cc

clean:
	rm draw_shape_main.o
	rm draw_shape.o
	rm draw_shape
