#ifndef TRANSPOTR_EVENT_H
#define TRANSPOTR_EVENT_H

#include <SDL2/SDL.h>

#include "entity.h"

class Player;
class Graphics;
class Camera;

class TransportEvent : public Entity
{
public:
	TransportEvent();
	~TransportEvent() override;

	virtual void update(Player& player) override;
	virtual void render(const Graphics& graphics,
			    const Camera& camera) override;

	void setEventPosRect(const SDL_Rect& posRect);
	void setTransportDestination(int x, int y);
private:
	SDL_Rect posRect_ = {0};

	int destX_, destY_;
};

#endif /* TRANSPOTR_EVENT_H */
