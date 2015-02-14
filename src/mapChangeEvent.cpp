#include "player.h"
#include "mainGame_gameScene.h"

#include "mapChangeEvent.h"

MapChangeEvent::MapChangeEvent(const SDL_Rect& posRect,
			       const std::string& mapFile, Player& player,
			       MainGame_GameScene& mainGame)
	: posRect_(posRect),
	  mapFile_(mapFile),
	  player_(player),
	  mainGame_(mainGame)
{
}

void
MapChangeEvent::update()
{
	const SDL_Rect& playerRect = player_.posRectOnMap();

	if (SDL_HasIntersection(&playerRect, &posRect_))
		mainGame_.changeMap(mapFile_);
}

void
MapChangeEvent::render(const Graphics& graphics, const Camera& camera)
{
}
