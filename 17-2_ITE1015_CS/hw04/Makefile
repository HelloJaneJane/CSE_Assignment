CXX=g++

All: blackjack reply_admin minesweeper

blackjack: blackjack.o
	$(CXX) -o blackjack blackjack.o
blackjack.o: blackjack.cc
	$(CXX) -std=c++0x -c blackjack.cc

reply_admin: reply_admin_main.o reply_admin.o
	$(CXX) -o reply_admin reply_admin_main.o reply_admin.o
reply_admin_main.o: reply_admin.h
	$(CXX) -std=c++0x -c reply_admin_main.cc
reply_admin.o: reply_admin.h
	$(CXX) -std=c++0x -c reply_admin.cc

minesweeper: minesweeper_main.o  minesweeper.o
	$(CXX) -o minesweeper minesweeper_main.o minesweeper.o
minesweeper_main.o: minesweeper.h
	$(CXX) -std=c++0x -c minesweeper_main.cc
minesweeper.o: minesweeper.h
	$(CXX) -std=c++0x -c minesweeper.cc

clean:
	rm blackjack.o
	rm blackjack
	rm reply_admin_main.o
	rm reply_admin.o
	rm reply_admin
	rm minesweeper_main.o
	rm minesweeper.o
	rm minesweeper