#pragma once
#include <vector>

class Entity;
class Kirby;
class Enemy;
class Projectile;
class Ability;

class Collision final
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~		WORLD COLLISION			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static bool WallCollision		(Entity* entity,  const std::vector<std::vector<Point2f>>& world);
	static bool FloorCollision		(Entity* entity,  const std::vector<std::vector<Point2f>>& world);

	static bool WallCollision		(Projectile* projectile, const std::vector<std::vector<Point2f>>& world);
	static bool FloorCollision		(Projectile* projectile, const std::vector<std::vector<Point2f>>& world);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~  ENTITY/PROJECTILE COLLISION	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static bool KirbyHitDetection(Kirby* pKirby, std::vector<Enemy*>& vEnemies, std::vector<Projectile*>& vProjectiles);

	static bool EntityCollision		(Entity* entity1, Entity* entity2);
	static bool ProjectileCollision	(Entity* entity,  Projectile* projectile);
private:

};

