#include "player.h"
#include "controller.h"

namespace
{
	const float kMoveAccX = 0.3f;

	const int16_t kJoyAxisMax = 32767;
}

void
Player::FallingState::onEnter(Player& player)
{
}

void
Player::FallingState::onExit(Player& player)
{
}

void
Player::FallingState::update(Player& player,
			     const Controller& controller)
{
	handleInput_(player, controller);
}

void
Player::FallingState::handleInput_(Player& player,
				   const Controller& controller)
{
	if (controller.getButtonState(BUTTON_LEFT))
		player.accX_ = -kMoveAccX;
	else if (controller.getButtonState(BUTTON_RIGHT))
		player.accX_ = kMoveAccX;
	else if (std::abs(controller.getAxisValue(0)) > 4000)
		player.accX_ = 0.6f * ((float) controller.getAxisValue(0) /
				       (float) kJoyAxisMax);
	else
		player.accX_ = 0.f;

}
