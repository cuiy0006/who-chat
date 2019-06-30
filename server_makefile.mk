CC = g++
CPPFLAGS = -std=c++11
INCLUDE=-Isrc/common

who_chat_server: src/server/server-main.o src/server/server.o
	$(CC) $(CPPFLAGS) $^ -o $@

src/server/server-main.o: src/server/server-main.cpp
	$(CC) $(CPPFLAGS) $(INCLUDE) -c $^ -o $@

src/server/server.o: src/server/server.cpp
	$(CC) $(CPPFLAGS) $(INCLUDE) -c $^ -o $@

clean:
	rm -rf *.o who_chat_server
	rm -rf src/server/*.o