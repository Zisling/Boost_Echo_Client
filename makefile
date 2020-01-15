CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude -pthread
LFLAGS:=-lboost_system -pthread

all: Client

Client: bin/connectionHandler.o bin/Books.o bin/SocketIO.o bin/UserIO.o bin/echoClient.o
	g++ -o bin/Client bin/connectionHandler.o bin/Books.o bin/SocketIO.o bin/UserIO.o bin/echoClient.o $(LFLAGS)


bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/Books.o: src/Books.cpp
	g++ $(CFLAGS) -o bin/Books.o src/Books.cpp

bin/SocketIO.o: src/connectionHandler.cpp src/SocketIO.cpp
	g++ $(CFLAGS) -o bin/SocketIO.o src/SocketIO.cpp

bin/UserIO.o: src/Books.cpp src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/UserIO.o src/UserIO.cpp

bin/echoClient.o: src/echoClient.cpp src/Books.cpp src/SocketIO.cpp src/UserIO.cpp
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp


.PHONY: clean
clean:
	rm -f bin/*
