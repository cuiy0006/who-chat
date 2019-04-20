CC = g++
CFLAGS = -std=c++11

all: src/server/server-main.cpp src/server/server.o
	$(CC) $(CFLAGS) src/server/server-main.cpp src/server/server.o -o who_chat_server

Server.o: src/server/server.cpp src/server/server.h src/common/common.h
	$(CC) $(CFLAGS) -c src/server/server.cpp

clean: rm -rf *.o who_chat_server