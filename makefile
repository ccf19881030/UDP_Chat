GCC=g++
CFLAGS=-Wall -g

all:server client

server:server.cpp pub.h
	$(GCC) -o server server.cpp $(CFLAGS)
client:client.cpp pub.h
	$(GCC)  -o client client.cpp $(CFLAGS)

clean:
	rm server client *.o -rf
