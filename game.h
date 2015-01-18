#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "sprite.h"
#include "controller.h"
#include "camera.h"
#include "player.h"
#include "graphics.h"
#include "window.h"
#include "sdlSystem.h"
#include "transportEvent.h"

class Game
{
public:
	Game();
	~Game();

	int execute();
private:
	bool appIsRunning_ = true;

	SDLSystem sdlSystem_;
	Window window_;
	Graphics graphics_;

	MapTileLayer backLayer_;
	MapTileLayer foreLayer_;
	MapObjectLayer objectLayer_;

	TransportEvent te_;

	Camera camera_;

	Controller controller_;

	Player player_;

	void eventHandler_(const SDL_Event& event);
	void update_();
	void render_();

	void loadMap_(const std::string& filePath);
};

#endif /* GAME_H */