#include "player.h"
#include "graphics.h"
#include "camera.h"

#include "transportEvent.h"

TransportEvent::TransportEvent(const SDL_Rect& posRect, Player& player,
			       const SDL_Point& dstPoint)
	: posRect_(posRect),
	  dstPoint_(dstPoint),
	  player_(player)
{
}

void
TransportEvent::update()
{
	const SDL_Rect& playerPosRect = player_.posRectOnMap();

	if (SDL_HasIntersection(&playerPosRect, &posRect_)) {
		player_.setX(dstPoint_.x);
		player_.setY(dstPoint_.y);
	}
}

void
TransportEvent::render(const Graphics& graphics, const Camera& camera)
{
}
