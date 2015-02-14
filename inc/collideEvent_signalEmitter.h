#ifndef COLLIDE_EVENT_SIGNAL_EMITTER_H
#define COLLIDE_EVENT_SIGNAL_EMITTER_H

#include <vector>
#include <SDL2/SDL.h>

#include "player.h"

#include "signalReceiver.h"
#include "signalEmitter.h"

class CollideEvent_signalEmitter : public SignalEmitter
{
public:
	CollideEvent_signalEmitter(const SDL_Rect& posRect):
		posRect_(posRect)
	{
	}

	virtual ~CollideEvent_signalEmitter() override {};

	virtual void update(const Player& player,
			    const Controller& controller) override
	{
		SDL_Rect playerPosRect = player.posRectOnMap();

		if (SDL_HasIntersection(&playerPosRect, &posRect_))
			notifyAllReceiver_();
	}

	virtual void render(Graphics& graphics, const Camera& camera) override
	{
	}

	void addReceiver(SignalReceiver& target)
	{
		notifyList_.push_back(&target);
	}
private:
	SDL_Rect posRect_ = {0};
	std::vector<SignalReceiver*> notifyList_;

	void notifyAllReceiver_()
	{
		for (SignalReceiver* e : notifyList_)
			e->sendSignal();
	}
};

#endif /* COLLIDE_EVENT_SIGNAL_EMITTER_H */
