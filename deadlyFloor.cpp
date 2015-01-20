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

DeadlyFloor::DeadlyFloor(Graphics& graphics, const SDL_Rect& posRect):
	posRect_(posRect),
	floorSprite_(graphics, kSpriteName, kSpriteClipRect)
{
}

DeadlyFloor::~DeadlyFloor()
{
}

void
DeadlyFloor::update(Player& player)
{
	SDL_Rect playerRect = player.posRectOnMap();

	if (SDL_HasIntersection(&posRect_, &playerRect))
		player.suicide();
}

void
DeadlyFloor::render(const Graphics& graphics, const Camera& camera)
{
	SDL_Rect posToDraw = posRect_;

	posToDraw.x -= camera.getLeft();
	posToDraw.y -= camera.getTop();

	floorSprite_.render(graphics, posToDraw);
}

void
DeadlyFloor::setDirection(enum Direction dir)
{
}
