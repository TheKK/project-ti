#ifndef MAP_CHANGE_EVENT_H
#define MAP_CHANGE_EVENT_H

#include <SDL2/SDL.h>
#include <string>

#include "entity.h"

class Graphics;
class MainGame_GameScene;
class Player;

class MapChangeEvent : public Entity
{
public:
	MapChangeEvent(const SDL_Rect& posRect, const std::string& mapFile,
		       Player& player, MainGame_GameScene& mainGame);

	virtual void update() override;
	virtual void render(const Graphics& graphics,
			    const Camera& camera) override;
private:
	SDL_Rect posRect_;
	std::string mapFile_;

	Player& player_;
	MainGame_GameScene& mainGame_;
};

#endif /* MAP_CHANGE_EVENT_H */
