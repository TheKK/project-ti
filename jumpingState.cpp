#include "player.h"
#include "controller.h"

#include "jumpingState.h"

namespace
{
	const float kMoveAccX = 0.7f;

	const float kJumpSpeed = -3.f;

	const int16_t kJoyAxisMax = 32767;
}

void
JumpingState::onEnter(Player& player)
{
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
		player.setAccX(0.6f * ((float) controller.getAxisValue(0) /
				       (float) kJoyAxisMax));

	//if (controller.ifButtonReleased(BUTTON_B))
		//player.nextState_ = &player.fallingState_;
}
