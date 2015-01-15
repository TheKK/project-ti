#include "player.h"
#include "controller.h"

#include "jumpingState.h"

#include <iostream>

namespace
{
	const float kMoveAccX = 0.1f;

	const float kJumpSpeed = -3.f;

	const int16_t kJoyAxisMax = 32767;
}

void
JumpingState::onEnter(Player& player)
{
	std::cout << "enter jumping\n";

	player.setVelY(kJumpSpeed);
}

void
JumpingState::onExit(Player& player)
{
}

void
JumpingState::update(Player& player,
			     const Controller& controller)
{
	handleInput_(player, controller);

	if (player.isOnGround())
		player.setNextState(STATE_STANDING);

	//if (player.getVelY() >= 0.f)
		//player.setNextState(STATE_FALLING);
}

void
JumpingState::handleInput_(Player& player,
				   const Controller& controller)
{
	if (controller.getButtonState(BUTTON_LEFT))
		player.setAccX(-kMoveAccX);
	else if (controller.getButtonState(BUTTON_RIGHT))
		player.setAccX(kMoveAccX);
	else if (std::abs(controller.getAxisValue(0)) > 4000)
		player.setAccX(kMoveAccX * ((float) controller.getAxisValue(0) /
					    (float) kJoyAxisMax));

	//if (controller.ifButtonReleased(BUTTON_B))
		//player.setNextState(STATE_FALLING);

	if (player.isBesideLeftWall() &&
	    controller.ifButtonPressed(BUTTON_Y)) {
		player.setVelX(50.f);
		player.setVelY(-2.f);
	}

	if (player.isBesideRightWall() &&
	    controller.ifButtonPressed(BUTTON_A)) {
		player.setVelX(-50.f);
		player.setVelY(-2.f);
	}
}
