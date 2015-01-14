#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <SDL2/SDL.h>

#include "state.h"
#include "standingState.h"
#include "jumpingState.h"

class Controller;
class Camera;
class MapTileLayer;

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
	~Player();

	void update(const Controller& controller,
		    const MapTileLayer& tileLayer);
	void render(SDL_Renderer* renderer, const Camera& camera);

	void setX(int x);
	void setY(int y);
	void setAccX(float accX);
	void setAccY(float accY);
	void setVelX(float velX);
	void setVelY(float velY);

	void setNextState(enum PlayerState nextState);

	bool isOnGround() const;

	SDL_Rect posRectOnMap() const;
private:
	class FallingState : public State
	{
	public:
		virtual void onEnter(Player& player) override;
		virtual void onExit(Player& player) override;
		virtual void update(Player& player,
				    const Controller& controller) override;
	private:
		void handleInput_(Player& player,
				  const Controller& controller);
	};

	State* currentState_;
	State* nextState_;
	StandingState standingState_;
	JumpingState jumpingState_;

	float posX_, posY_;
	float velX_, velY_;
	float accX_, accY_;

	bool collideWithRightWall_ = false;
	bool collideWithLeftWall_ = false;
	bool isOnGround_ = false;

	void updateX_(const MapTileLayer& tileLayer);
	void updateY_(const MapTileLayer& tileLayer);
};

#endif /* PLAYER_H */
