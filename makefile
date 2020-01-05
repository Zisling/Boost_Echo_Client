CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system-mt -lboost_filesystem-mt
export CXXFLAGS=-pthread
all: EchoClient
	g++ -o bin/echoExample bin/connectionHandler.o bin/echoClient.o bin/Books.o $(LDFLAGS) $(CXXFLAGS)

EchoClient: bin/connectionHandler.o bin/echoClient.o bin/Books.o
	
bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp $(LDFLAGS)
bin/echoClient.o: src/echoClient.cpp
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp bin/Books.o src/Books.cpp $(LDFLAGS)
	
.PHONY: clean
clean:
	rm -f bin/*
