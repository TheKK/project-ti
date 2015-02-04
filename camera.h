#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

class Player;
class Graphics;

class Camera
{
public:
	Camera();
	~Camera();

	void setup(Graphics& graphics,
		   int worldWidth, int worldHeight,
		   int viewWidth, int viewHeight);

	void update();

	void setX(int x);
	void setY(int y);

	int getLeft() const;
	int getRight() const;
	int getTop() const;
	int getBottom() const;

	void lookAt(const Player* target);
private:
	SDL_Rect worldRect_ = {0};
	SDL_Rect viewRect_ = {0};
	SDL_Rect deadZoneRect_ = {0};

	const Player* target_ = nullptr;
};

#endif /* CAMERA_H */
