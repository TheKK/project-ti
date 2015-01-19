#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <SDL2/SDL.h>

#include "state.h"
#include "standingState.h"
#include "jumpingState.h"
#include "fallingState.h"
#include "sprite.h"

class Controller;
class Camera;
class MapTileLayer;
class Graphics;

enum PlayerState
{
	STATE_STANDING = 0x00,
	STATE_JUMPING,
	STATE_FALLING
};

class Player
{
public:
	Player();
	Player(Graphics& graphics);
	~Player();

	void update(const Controller& controller,
		    const MapTileLayer& tileLayer);
	void render(const Graphics& graphics, const Camera& camera);

	/* TODO Use DTO or something */
	void setX(int x);
	void setY(int y);
	void setAccX(float accX);
	void setAccY(float accY);
	void setVelX(float velX);
	void setVelY(float velY);

	int getVelX() const;
	int getVelY() const;

	void setNextState(enum PlayerState nextState);

	bool isOnGround() const;
	bool isBesideRightWall() const;
	bool isBesideLeftWall() const;

	SDL_Rect posRectOnMap() const;
private:
	/* TODO Maybe use queue to save nextState */
	State* currentState_;
	State* nextState_;
	StandingState standingState_;
	JumpingState jumpingState_;
	FallingState fallingState_;

	Sprite standSprite_;

	float posX_, posY_;
	float velX_, velY_;
	float accX_, accY_;

	bool isBesideRightWall_ = false;
	bool isBesideLeftWall_ = false;
	bool isOnGround_ = false;

	void updateX_(const MapTileLayer& tileLayer);
	void updateY_(const MapTileLayer& tileLayer);

	void checkWallAround_(const MapTileLayer& tileLayer);
};

#endif /* PLAYER_H */
