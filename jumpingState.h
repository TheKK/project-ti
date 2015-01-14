#ifndef JUMPING_STATE_H
#define JUMPING_STATE_H

#include "state.h"

class Player;
class Controller;

class JumpingState : public State
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

#endif /* JUMPING_STATE_H */

