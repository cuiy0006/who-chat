CC = g++
CFLAGS = -std=c++11

all: src/server/ServerMain.cpp src/server/Server.o
	$(CC) $(CFLAGS) src/server/ServerMain.cpp src/server/Server.o -o who_chat_server

Server.o: src/server/Server.cpp src/server/Server.h src/common/common.h
	$(CC) $(CFLAGS) -c src/server/Server.cpp

clean: rm -rf *.o who_chat_server