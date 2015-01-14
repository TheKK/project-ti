#include "player.h"
#include "controller.h"

#include "standingState.h"

namespace
{
	const float kMoveAccX = 0.7f;

	const int16_t kJoyAxisMax = 32767;
}

void
StandingState::onEnter(Player& player)
{
	player.setVelY(0.f);
}

void
StandingState::onExit(Player& player)
{
}

void
StandingState::update(Player& player,
			      const Controller& controller)
{
	handleInput_(player, controller);
}

void
StandingState::handleInput_(Player& player,
				    const Controller& controller)
{
	if (controller.getButtonState(BUTTON_LEFT))
		player.setAccX(-kMoveAccX);
	else if (controller.getButtonState(BUTTON_RIGHT))
		player.setAccX(kMoveAccX);
	else if (std::abs(controller.getAxisValue(0)) > 4000)
		player.setAccX(0.6f * ((float) controller.getAxisValue(0) /
				       (float) kJoyAxisMax));

	if (controller.ifButtonPressed(BUTTON_B))
		player.setNextState(STATE_JUMPING);
}
