SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

map: $(OBJ)
	g++ $^ -std=c++11 -g -lSDL2 -lSDL2_image -ljsoncpp -o $@

%.o: %.cpp
	g++ $^ -std=c++11 -g -c -o $@
