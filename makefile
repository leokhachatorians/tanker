make:
	rm -f ./play
	g++ -Wall -g main.cpp Game.cpp Player.cpp Screen.cpp Tank.cpp Simplex.cpp -oplay -lpthread -lncurses -std=c++11
