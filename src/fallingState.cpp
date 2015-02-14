#include "player.h"
#include "controller.h"

#include "fallingState.h"

#include <iostream>

namespace
{
	const float kMoveAccX = 0.001f;

	const int16_t kJoyAxisMax = 32767;
}

void
FallingState::onEnter(Player& player)
{
	std::cout << "enter falling\n";
}

void
FallingState::onExit(Player& player)
{
}

void
FallingState::update(Player& player,
			     const Controller& controller)
{
	handleInput_(player, controller);

	if (player.isOnGround())
		player.setNextState(STATE_STANDING);
}

void
FallingState::handleInput_(Player& player,
				   const Controller& controller)
{
	if (controller.getButtonState(BUTTON_LEFT))
		player.setAccX(-kMoveAccX);
	else if (controller.getButtonState(BUTTON_RIGHT))
		player.setAccX(kMoveAccX);
	else if (std::abs(controller.getAxisValue(0)) > 4000)
		player.setAccX(0.6f * ((float) controller.getAxisValue(0) /
				       (float) kJoyAxisMax));
}
