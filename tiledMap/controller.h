#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>

#include <SDL2/SDL.h>

enum Buttons {
	BUTTON_UP = 0x00,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,

	BUTTON_START,

	BUTTON_COUNT
};

enum ButtonStates {
	BUTTON_STATE_PRESSED = 0x00,
	BUTTON_STATE_RELEASED,

	BUTTON_STATE_COUNT
};

class Controller
{
public:
	Controller();
	~Controller();

	void eventHandler(const SDL_Event* event);
	void stateClear();

	enum ButtonStates getButtonState(enum Buttons which) const;
	bool ifButtonPressed(enum Buttons which) const;
	bool ifButtonReleased(enum Buttons which) const;
private:
	enum ButtonStates buttonState_[BUTTON_COUNT] = {BUTTON_STATE_RELEASED};
	bool buttonPressed_[BUTTON_COUNT] = {false};
	bool buttonReleased_[BUTTON_COUNT] = {false};

	std::map<int, SDL_Joystick*> joysticks_;

	void addJoystick_(int joystickId);
	void removeJoystick_(int joystickId);
};

#endif /* CONTROLLER_H */
