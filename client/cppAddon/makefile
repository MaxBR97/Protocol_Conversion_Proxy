CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lpthread

all: StompWCIClient

StompWCIClient:  bin/ConnectionHandler.o bin/InputProtocol.o bin/event.o bin/StompFrame.o bin/StompProtocol.o bin/StompClient.o
	g++ -o bin/StompWCIClient bin/StompClient.o bin/ConnectionHandler.o bin/event.o bin/StompFrame.o bin/StompProtocol.o bin/InputProtocol.o $(LDFLAGS)

bin/StompClient.o: src/StompClient.cpp 
	g++ $(CFLAGS) -o bin/StompClient.o src/StompClient.cpp 
	
bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/event.o: src/event.cpp
	g++ $(CFLAGS) -o bin/event.o src/event.cpp

bin/StompFrame.o: src/StompFrame.cpp
	g++ $(CFLAGS) -o bin/StompFrame.o src/StompFrame.cpp

bin/StompProtocol.o: src/StompProtocol.cpp 
	g++ $(CFLAGS) -o bin/StompProtocol.o src/StompProtocol.cpp 

bin/InputProtocol.o: src/InputProtocol.cpp 
	g++ $(CFLAGS) -o bin/InputProtocol.o src/InputProtocol.cpp 


.PHONY: clean
clean:
	rm -f bin/*
	