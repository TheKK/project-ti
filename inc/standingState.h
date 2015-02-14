#ifndef STAND_STATE_H
#define STAND_STATE_H

#include "state.h"

class Player;
class Controller;

class StandingState : public State
{
public:
	virtual void onEnter(Player& player) override;
	virtual void onExit(Player& player) override;
	virtual void update(Player& player,
			    const Controller& controller) override;
private:
	void handleInput_(Player& player,
			  const Controller& controller);
};

#endif /* STAND_STATE_H */

