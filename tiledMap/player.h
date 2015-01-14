#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

class Controller;
class Camera;
class MapTileLayer;

class Player
{
public:
	Player();
	~Player();

	void update(const Controller& controller, const MapTileLayer& tiles);
	void render(SDL_Renderer* renderer, const Camera& camera);

	void setPos(int x, int y);

	SDL_Rect rect() const;
private:
	SDL_Rect posRect_;
};

#endif /* PLAYER_H */
