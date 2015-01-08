#include "controller.h"
Controller::Controller()
{
	for (auto& e : buttonState_)
		e = BUTTON_STATE_RELEASED;
}

Controller::~Controller()
{
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

