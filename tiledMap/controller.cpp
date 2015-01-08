#include "controller.h"

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
		default:
			break;
		}
		break;

	case SDL_JOYBUTTONDOWN:
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
			    "Joystick button %d pressed",
			    event->jbutton.button);
		if (event->jbutton.button == 9) {
			buttonState_[BUTTON_START] = BUTTON_STATE_PRESSED;
			buttonPressed_[BUTTON_START] = true;
		}
		break;

	case SDL_JOYBUTTONUP:
		if (event->jbutton.button == 9) {
			buttonState_[BUTTON_START] = BUTTON_STATE_RELEASED;
			buttonReleased_[BUTTON_START] = true;
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

void
Controller::addJoystick_(int joystickId)
{
	SDL_Joystick* joystick;

	joystick = SDL_JoystickOpen(joystickId);
	if (!joystick) {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
			    "Unable to open joystick: %s",
			    SDL_GetError());
		return;
	}

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
