#pragma once
#include <vector>

class Entity;
class Projectile;
class Ability;

class Collision final
{
public:
	explicit Collision();
	~Collision() = default;

	// World Collsion Detection AND Handling
	static bool WallCollision		(Entity* entity,  const std::vector<std::vector<Point2f>>& world);
	static bool FloorCollision		(Entity* entity,  const std::vector<std::vector<Point2f>>& world);

	static bool WallCollision		(Projectile* projectile, const std::vector<std::vector<Point2f>>& world);
	static bool FloorCollision		(Projectile* projectile, const std::vector<std::vector<Point2f>>& world);

	// World Object Collision Detection
	static bool EntityCollision		(Entity* entity1, Entity* entity2);
	static bool ProjectileCollision	(Entity* entity,  Projectile* projectile);
private:

};

