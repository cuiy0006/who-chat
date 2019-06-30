CC = g++
CPPFLAGS = -std=c++11
INCLUDE=-Isrc/common

who_chat_client: src/client/client-main.o src/client/client.o
	$(CC) $(CPPFLAGS) $^ -o $@

src/client/client-main.o: src/client/client-main.cpp
	$(CC) $(CPPFLAGS) $(INCLUDE) -c $^ -o $@

src/client/client.o: src/client/client.cpp
	$(CC) $(CPPFLAGS) $(INCLUDE) -c $^ -o $@

clean:
	rm -rf *.o who_chat_client
	rm -rf src/client/*.o