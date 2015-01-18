#include "player.h"
#include "graphics.h"
#include "camera.h"

#include "transportEvent.h"

TransportEvent::TransportEvent()
{
}

TransportEvent::~TransportEvent()
{
}

void
TransportEvent::update(Player& player)
{
	SDL_Rect playerPosRect;

	playerPosRect = player.posRectOnMap();
	if (SDL_HasIntersection(&playerPosRect, &posRect_)) {
		player.setX(destX_);
		player.setY(destY_);
	}
}

void
TransportEvent::render(const Graphics& graphics, const Camera& camera)
{
}

void
TransportEvent::setEventPosRect(const SDL_Rect& posRect)
{
	posRect_ = posRect;
}

void
TransportEvent::setTransportDestination(int x, int y)
{
	destX_ = x;
	destY_ = y;
}
