#ifndef FALLING_STATE_H
#define FALLING_STATE_H

#include "state.h"

class Player;
class Controller;

class FallingState : public State
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

#endif /* FALLING_STATE_H */

