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
	TransportEvent(const SDL_Rect& posRect, Player& player,
		       const SDL_Point& dstPoint);

	virtual void update() override;
	virtual void render(const Graphics& graphics,
			    const Camera& camera) override;
private:
	SDL_Rect posRect_;
	SDL_Point dstPoint_;

	Player& player_;
};

#endif /* TRANSPOTR_EVENT_H */
