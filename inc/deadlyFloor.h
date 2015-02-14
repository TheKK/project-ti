#ifndef DEADLY_FLOOR_H
#define DEADLY_FLOOR_H

#include <SDL2/SDL.h>

#include "sprite.h"

#include "entity.h"

enum Direction
{
	UP = 0x00,
	DOWN,
	LEFT,
	RIGHT
};

class Player;

class DeadlyFloor : public Entity
{
public:
	DeadlyFloor(Graphics& graphics, const SDL_Rect& posRect,
		    Player& player);

	virtual void update() override;
	virtual void render(const Graphics& graphics,
			    const Camera& camera) override;

	void setDirection(enum Direction dir);
private:
	SDL_Rect posRect_ = {0};
	Sprite floorSprite_;

	Player& player_;
};

#endif /* DEADLY_FLOOR_H */
