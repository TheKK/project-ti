#ifndef MOVING_GROUND_SIGNAL_RECEIVER_H
#define MOVING_GROUND_SIGNAL_RECEIVER_H

#include <SDL/SDL.h>
#include <cstdint>

#include "sprite.h"
#include "camera.h"
#include "player.h"

#include "signalReceiver.h"

/* TODO Create cpp file */
class MovingGraound_signalReciever : public SignalReceiver
{
public:
	MovingGraound_signalReciever(Graphics& graphics,
				     const SDL_Rect& posRect):
		groundSprite_(graphics, "./assets/images/sewer.png",
			      SDL_Rect({0, 0, 16, 16})),
		posRect_(posRect)
	{
	}

	virtual ~MovingGraound_signalReciever() override {};

	virtual void update(Player& player,
			    const Controller& controller) override
	{
		if (visibleCounter_ == 0)
			return;
		else
			--visibleCounter_;

		player.testCollide(std::vector<SDL_Rect>({posRect_}));
	}

	virtual void render(Graphics& graphics, const Camera& camera) override
	{
		if (visibleCounter_ == 0)
			return;

		SDL_Rect posToDraw = posRect_;

		posToDraw.x -= camera.getLeft();
		posToDraw.y -= camera.getTop();

		groundSprite_.render(graphics, &posToDraw);
	}

	virtual void sendSignal() override
	{
		visibleCounter_ = 10;
	}
private:
	Sprite groundSprite_;
	SDL_Rect posRect_;

	int visibleCounter_ = 0;
};

#endif /* MOVING_GROUND_SIGNAL_RECEIVER_H */
