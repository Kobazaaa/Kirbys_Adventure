#pragma once
#include <vector>

class Entity;

class Collision final
{
public:
	Collision();
	~Collision();

	static bool WallCollision	(Entity* entity, const std::vector<std::vector<Point2f>>& world);
	static bool FloorCollision	(Entity* entity, const std::vector<std::vector<Point2f>>& world);
	static bool EntityCollision	(Entity* entity, const std::vector<std::vector<Point2f>>& world);

private:

};

