#ifndef STATE_H
#define STATE_H

class Player;
class Controller;

class State
{
public:
	virtual void onEnter(Player& player) = 0;
	virtual void onExit(Player& player) = 0;
	virtual void update(Player& player, const Controller& controller) = 0;
};

#endif /* STATE_H */
