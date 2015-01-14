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

	void update(const Controller& controller,
		    const MapTileLayer& tileLayer);
	void render(SDL_Renderer* renderer, const Camera& camera);

	SDL_Rect posRectOnMap() const;
private:
	float posX_, posY_;
	float velX_, velY_;
	float accX_, accY_;

	void updateInput_(const Controller& controller);
	void updateX_(const MapTileLayer& tileLayer);
	void updateY_(const MapTileLayer& tileLayer);

	void testCollideWithTiles_(const MapTileLayer& tileLayer);
};

#endif /* PLAYER_H */
