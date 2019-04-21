CC = g++
CPPFLAGS = -std=c++11

all: src/server/server-main.cpp src/server/server.o
	$(CC) $(CPPFLAGS) src/server/server-main.cpp src/server/server.o -o who_chat_server

server.o: src/server/server.cpp src/server/server.h src/common/common.h
	$(CC) $(CPPFLAGS) -c src/server/server.cpp

clean: rm -rf *.o who_chat_server