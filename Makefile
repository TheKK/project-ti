CXX = g++
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
DEP = $(SRC:.cpp=.d)

OUT_EXE = map

$(OUT_EXE): $(OBJ)
	$(CXX) $^ -std=c++14 -g -lSDL2 -lSDL2_image -ljsoncpp -o $@

%.o: %.cpp
	$(CXX) $^ -std=c++14 -g -c -o $@

clean:
	@rm -frv $(OBJ) $(OUT_EXE)
