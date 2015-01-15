SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

OUT_EXE = map

$(OUT_EXE): $(OBJ)
	g++ $^ -std=c++11 -g -lSDL2 -lSDL2_image -ljsoncpp -o $@

%.o: %.cpp
	g++ $^ -std=c++11 -g -c -o $@

clean:
	@rm -frv $(OBJ) $(OUT_EXE)
