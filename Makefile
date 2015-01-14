OBJ = main.o map.o sprite.o controller.o camera.o player.o timer.o

map: $(OBJ)
	g++ $^ -std=c++11 -g -lSDL2 -lSDL2_image -ljsoncpp -o $@

%.o: %.cpp
	g++ $^ -std=c++11 -g -c -o $@
