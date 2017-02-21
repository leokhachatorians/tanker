make:
	g++ main.cpp Game.cpp Player.cpp Screen.cpp Tank.cpp -oplay -lpthread -lncurses -std=c++11
