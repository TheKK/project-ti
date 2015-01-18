#include <iostream>
#include <stdexcept>

#include "game.h"

int
main(int argc, char* argv[])
{
	try {
		Game game;
		game.execute();
		return 0;
	} catch (std::runtime_error e) {
		std::cout << e.what() << "\n";
		return 1;
	}
}
