CXX=g++

All: point2d parse_radix

point2d: point2d_main.o point2d.o
	$(CXX) -o point2d point2d_main.o point2d.o
point2d_main.o: point2d.h
	$(CXX) -std=c++0x -c point2d_main.cc
point2d.o: point2d.h
	$(CXX) -std=c++0x -c point2d.cc

parse_radix: parse_radix_main.o parse_radix.o
	$(CXX) -o parse_radix parse_radix_main.o parse_radix.o
parse_radix_main.o: parse_radix.h
	$(CXX) -std=c++0x -c parse_radix_main.cc
parse_radix.o: parse_radix.h
	$(CXX) -std=c++0x -c parse_radix.cc

clean:
	rm point2d_main.o
	rm point2d.o
	rm point2d
	rm parse_radix_main.o
	rm parse_radix.o
	rm parse_radix
