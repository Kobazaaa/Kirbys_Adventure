#pragma once
#include <vector>

class Entity;

class Collision final
{
public:
	explicit Collision();
	~Collision() = default;

	static bool WallCollision	(Entity* entity,  const std::vector<std::vector<Point2f>>& world);
	static bool FloorCollision	(Entity* entity,  const std::vector<std::vector<Point2f>>& world);
	static bool EntityCollision	(Entity* entity1, Entity* entity2);
	//static bool ObjectCollision (Entity* entity,  Object* object);

private:

};

