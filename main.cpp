#include <SDL2/SDL.h>
#include <stdexcept>

#include "gameSceneManager.h"
#include "game.h"

int
main(int argc, char* argv[])
{
	try {
		Game game;
		game.execute(TotalGameScene::MAIN_GAME_SCENE);
		return 0;
	} catch (std::runtime_error e) {
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"Something bad happended",
			e.what(),
			nullptr);
		return 1;
	}
}
