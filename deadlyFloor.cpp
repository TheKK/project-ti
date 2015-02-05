#include "player.h"
#include "camera.h"

#include "deadlyFloor.h"

namespace
{
	const int kFloorWidth = 16;
	const int kFloorHeight = 16;

	const std::string kSpriteName = "deadlyFloor.png";
	const SDL_Rect kSpriteClipRect = {0, 0, kFloorWidth, kFloorHeight};
}

DeadlyFloor::DeadlyFloor(Graphics& graphics, const SDL_Rect& posRect,
			 Player& player)
	: posRect_(posRect),
	  floorSprite_(graphics, kSpriteName, kSpriteClipRect),
	  player_(player)
{
}

void
DeadlyFloor::update()
{
	const SDL_Rect& playerRect = player_.posRectOnMap();

	if (SDL_HasIntersection(&posRect_, &playerRect))
		player_.suicide();
}

void
DeadlyFloor::render(const Graphics& graphics, const Camera& camera)
{
	SDL_Rect posToDraw = posRect_;

	posToDraw.x -= camera.getLeft();
	posToDraw.y -= camera.getTop();

	floorSprite_.render(graphics, &posToDraw);
}

void
DeadlyFloor::setDirection(enum Direction dir)
{
}
