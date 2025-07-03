CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall

SRC = src/main.cpp src/server.cpp src/thread_pool.cpp src/http.cpp src/utils.cpp
OBJ = $(SRC:.cpp=.o)

all: server

server: $(SRC)
	$(CXX) $(CXXFLAGS) -o server $(SRC)

clean:
	rm -f server *.o