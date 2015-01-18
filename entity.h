#ifndef ENTITY_H
#define ENTITY_H

class Player;
class Graphics;
class Camera;

class Entity
{
public:
	virtual ~Entity() {};
	virtual void update(Player& player) = 0;
	virtual void render(const Graphics& graphics, const Camera& camera) = 0;
};

#endif /* ENTITY_H */
