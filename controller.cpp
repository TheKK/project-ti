#include <iostream>

#include "controller.h"

namespace
{
	const int16_t kJoyAxisThreshold = 1000;
}

Controller::Controller()
{
	for (auto& e : buttonState_)
		e = BUTTON_STATE_RELEASED;

	if (SDL_NumJoysticks() < 1) {
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "No joystick found");
		return;
	}

	for (int i = 0; i < SDL_NumJoysticks(); ++i)
		addJoystick_(i);
}

Controller::~Controller()
{
	for (int i = 0; i < SDL_NumJoysticks(); ++i)
		removeJoystick_(i);
}

void
Controller::eventHandler(const SDL_Event* event)
{
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.repeat)
			break;

		switch (event->key.keysym.scancode) {
		case SDL_SCANCODE_UP:
			buttonState_[BUTTON_UP] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_UP] = true;
			break;
		case SDL_SCANCODE_DOWN:
			buttonState_[BUTTON_DOWN] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_DOWN] = true;
			break;
		case SDL_SCANCODE_LEFT:
			buttonState_[BUTTON_LEFT] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_LEFT] = true;
			break;
		case SDL_SCANCODE_RIGHT:
			buttonState_[BUTTON_RIGHT] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_RIGHT] = true;
			break;
		case SDL_SCANCODE_Z:
			buttonState_[BUTTON_B] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_B] = true;
			break;
		case SDL_SCANCODE_X:
			buttonState_[BUTTON_A] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_A] = true;
			break;
		case SDL_SCANCODE_A:
			buttonState_[BUTTON_Y] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_Y] = true;
			break;
		case SDL_SCANCODE_S:
			buttonState_[BUTTON_X] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_X] = true;
			break;
		default:
			break;
		}
		break;

	case SDL_KEYUP:
		if (event->key.repeat)
			break;

		switch (event->key.keysym.scancode) {
		case SDL_SCANCODE_UP:
			buttonState_[BUTTON_UP] = BUTTON_STATE_RELEASED;
			buttonReleased_[BUTTON_UP] = true;
			break;
		case SDL_SCANCODE_DOWN:
			buttonState_[BUTTON_DOWN] = BUTTON_STATE_RELEASED;
			buttonReleased_[BUTTON_DOWN] = true;
			break;
		case SDL_SCANCODE_LEFT:
			buttonState_[BUTTON_LEFT] = BUTTON_STATE_RELEASED;
			buttonReleased_[BUTTON_LEFT] = true;
			break;
		case SDL_SCANCODE_RIGHT:
			buttonState_[BUTTON_RIGHT] = BUTTON_STATE_RELEASED;
			buttonReleased_[BUTTON_RIGHT] = true;
			break;
		case SDL_SCANCODE_Z:
			buttonState_[BUTTON_B] = BUTTON_STATE_RELEASED;
			buttonReleased_[BUTTON_B] = true;
			break;
		case SDL_SCANCODE_X:
			buttonState_[BUTTON_A] = BUTTON_STATE_RELEASED;
			buttonReleased_[BUTTON_A] = true;
			break;
		case SDL_SCANCODE_A:
			buttonState_[BUTTON_Y] = BUTTON_STATE_RELEASED;
			buttonReleased_[BUTTON_Y] = true;
			break;
		case SDL_SCANCODE_S:
			buttonState_[BUTTON_X] = BUTTON_STATE_RELEASED;
			buttonReleased_[BUTTON_X] = true;
			break;
		default:
			break;
		}
		break;

	case SDL_JOYBUTTONDOWN:
		switch (event->jbutton.button) {
		case 0:
			buttonState_[BUTTON_X] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_X] = true;
			break;
		case 1:
			buttonState_[BUTTON_A] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_A] = true;
			break;
		case 2:
			buttonState_[BUTTON_B] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_B] = true;
			break;
		case 3:
			buttonState_[BUTTON_Y] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_Y] = true;
			break;
		case 8:
			buttonState_[BUTTON_SELECT] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_SELECT] = true;
			break;
		case 9:
			buttonState_[BUTTON_START] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_START] = true;
			break;
		default:
			break;
		}
		break;

	case SDL_JOYBUTTONUP:
		switch (event->jbutton.button) {
		case 0:
			buttonState_[BUTTON_X] = BUTTON_STATE_RELEASED;
			buttonReleased_[BUTTON_X] = true;
			break;
		case 1:
			buttonState_[BUTTON_A] = BUTTON_STATE_RELEASED;
			buttonReleased_[BUTTON_A] = true;
			break;
		case 2:
			buttonState_[BUTTON_B] = BUTTON_STATE_RELEASED;
			buttonReleased_[BUTTON_B] = true;
			break;
		case 3:
			buttonState_[BUTTON_Y] = BUTTON_STATE_RELEASED;
			buttonReleased_[BUTTON_Y] = true;
			break;
		case 8:
			buttonState_[BUTTON_SELECT] = BUTTON_STATE_RELEASED;
			buttonReleased_[BUTTON_SELECT] = true;
			break;
		case 9:
			buttonState_[BUTTON_START] = BUTTON_STATE_RELEASED;
			buttonReleased_[BUTTON_START] = true;
			break;
		default:
			break;
		}
		break;

	case SDL_JOYHATMOTION:
		switch (event->jhat.value) {
		case SDL_HAT_UP:
			buttonState_[BUTTON_UP] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_UP] = true;
			break;
		case SDL_HAT_DOWN:
			buttonState_[BUTTON_DOWN] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_DOWN] = true;
			break;
		case SDL_HAT_LEFT:
			buttonState_[BUTTON_LEFT] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_LEFT] = true;
			break;
		case SDL_HAT_RIGHT:
			buttonState_[BUTTON_RIGHT] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_RIGHT] = true;
			break;
		case SDL_HAT_RIGHTUP:
			buttonState_[BUTTON_RIGHT] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_RIGHT] = true;
			buttonState_[BUTTON_UP] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_UP] = true;
			break;
		case SDL_HAT_RIGHTDOWN:
			buttonState_[BUTTON_RIGHT] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_RIGHT] = true;
			buttonState_[BUTTON_DOWN] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_DOWN] = true;
			break;
		case SDL_HAT_LEFTUP:
			buttonState_[BUTTON_LEFT] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_LEFT] = true;
			buttonState_[BUTTON_UP] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_UP] = true;
			break;
		case SDL_HAT_LEFTDOWN:
			buttonState_[BUTTON_LEFT] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_LEFT] = true;
			buttonState_[BUTTON_DOWN] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_DOWN] = true;
			break;
		case SDL_HAT_CENTERED:
			buttonState_[BUTTON_UP] = BUTTON_STATE_RELEASED;
			buttonState_[BUTTON_DOWN] = BUTTON_STATE_RELEASED;
			buttonState_[BUTTON_LEFT] = BUTTON_STATE_RELEASED;
			buttonState_[BUTTON_RIGHT] = BUTTON_STATE_RELEASED;
			buttonReleased_[BUTTON_UP] = true;
			buttonReleased_[BUTTON_DOWN] = true;
			buttonReleased_[BUTTON_LEFT] = true;
			buttonReleased_[BUTTON_RIGHT] = true;
			break;
		}
		break;

	case SDL_JOYAXISMOTION:
		if (event->jaxis.axis == 0) {
			axis0_ = event->jaxis.value;
		}
		if (event->jaxis.axis == 1) {
			axis1_ = event->jaxis.value;
		}
		break;

	case SDL_JOYDEVICEADDED:
		addJoystick_(event->jdevice.which);
		break;

	case SDL_JOYDEVICEREMOVED:
		removeJoystick_(event->jdevice.which);
		break;
	}
}

void
Controller::stateClear()
{
	for (bool& e : buttonPressed_)
		e = false;

	for (bool& e : buttonReleased_)
		e = false;
}

enum ButtonStates
Controller::getButtonState(enum Buttons which) const
{
	return buttonState_[which];
}

bool
Controller::ifButtonPressed(enum Buttons which) const
{
	return buttonPressed_[which];
}

bool
Controller::ifButtonReleased(enum Buttons which) const
{
	return buttonReleased_[which];
}

int16_t
Controller::getAxisValue(int which) const
{
	switch (which) {
	case 0:
		return axis0_;
	case 1:
		return axis1_;
	default:
		return 0;
	}
}

void
Controller::addJoystick_(int joystickId)
{
	SDL_Joystick* joystick = SDL_JoystickOpen(joystickId);

	if (!joystick) {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
			    "Unable to open joystick: %s",
			    SDL_GetError());
		return;
	}

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		    "Joystick %s inserted", SDL_JoystickName(joystick));

	joysticks_[joystickId] = joystick;

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		    "Joystick info:\n");
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		    "Name: %s\n", SDL_JoystickName(joystick));
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		    "Buttons: %d\n", SDL_JoystickNumButtons(joystick));
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		    "Hats: %d\n", SDL_JoystickNumHats(joystick));
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		    "Axises: %d\n", SDL_JoystickNumAxes(joystick));
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		    "Balls: %d", SDL_JoystickNumBalls(joystick));
}

void
Controller::removeJoystick_(int joystickId)
{
	SDL_Joystick* joystick = joysticks_[joystickId];

	SDL_assert(joystick);

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		    "Joystick %s removed", SDL_JoystickName(joystick));

	SDL_JoystickClose(joystick);

	joysticks_.erase(joystickId);

}
