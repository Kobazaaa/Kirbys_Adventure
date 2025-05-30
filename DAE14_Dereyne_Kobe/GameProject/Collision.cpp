#include "pch.h"
#include "Collision.h"
#include "ParticleSystem.h"
#include "Projectile.h"
#include "Entity.h"
#include "Kirby.h"
#include "Enemy.h"
#include "utils.h"
#include <iostream>


bool Collision::WallCollision(Entity* entity, const std::vector<std::vector<Point2f>>& world)
{
	const Point2f left		{ entity->GetHitBox().left								- 1, entity->GetHitBox().bottom + 1};
	const Point2f right		{ entity->GetHitBox().left + entity->GetHitBox().width	+ 1, entity->GetHitBox().bottom + 1};
	utils::HitInfo hitInfo	{ entity->GetHitInfo() };


	for (size_t idx{}; idx < world.size(); idx++)
	{
		if (utils::Raycast(world[idx], left, right, hitInfo))
		{
			if (hitInfo.normal.DotProduct(Vector2f(0, 1)) < 0.01f)
			{
				Point2f newPosition
				{
					hitInfo.intersectPoint.x + utils::GetSign(hitInfo.normal.x) * ((entity->GetHitBox().width / 2.f) + 1),
					entity->GetPosition().y
				};
				entity->SetPosition(newPosition);

				return true;
			}
		}
	}
	return false;
}
bool Collision::FloorCollision(Entity* entity, const std::vector<std::vector<Point2f>>& world)
{
	const Point2f topLeft		{ entity->GetHitBox().left,									entity->GetHitBox().bottom + (entity->GetHitBox().height) };
	const Point2f topRight		{ entity->GetHitBox().left + entity->GetHitBox().width,		entity->GetHitBox().bottom + (entity->GetHitBox().height) };
	const Point2f topMiddle		{ entity->GetHitBox().left + entity->GetHitBox().width / 2, entity->GetHitBox().bottom + (entity->GetHitBox().height) };
	const Point2f bottomLeft	{ entity->GetHitBox().left,									entity->GetHitBox().bottom };
	const Point2f bottomRight	{ entity->GetHitBox().left + entity->GetHitBox().width,		entity->GetHitBox().bottom };
	const Point2f bottomMiddle	{ entity->GetHitBox().left + entity->GetHitBox().width / 2,	entity->GetHitBox().bottom - 1};

	utils::HitInfo& hitInfo		{ entity->GetHitInfo() };
	bool hasCollision{false};

	for (size_t idx{}; idx < world.size(); idx++)
	{
		// Is the entity on a slope?
		bool isOnSlope	{false};
		if (utils::Raycast(world[idx], bottomLeft, topLeft, hitInfo))
		{
			if (1.f - abs(hitInfo.normal.y) > 0.001f) isOnSlope = true;
		}
		if (utils::Raycast(world[idx], bottomRight, topRight, hitInfo))
		{
			if (1.f - abs(hitInfo.normal.y) > 0.001f) isOnSlope = true;
		}
		if (utils::Raycast(world[idx], bottomMiddle, topMiddle, hitInfo))
		{
			if (1.f - abs(hitInfo.normal.y) > 0.001f) isOnSlope = true;
		}

		// If the entity is on a slope, use the middle ray for terrain collision checking/handling
		if (isOnSlope)
		{
			if (utils::Raycast(world[idx], bottomMiddle, topMiddle, hitInfo))
			{
				hasCollision = true;
			}
		}
		// Otherwise, use the left or right ray for terrain collision checking/handling
		else if (world[idx].size() > 2)
		{
			if ((utils::Raycast(world[idx], bottomRight, topRight, hitInfo)
				or utils::Raycast(world[idx], bottomLeft, topRight, hitInfo)
				or utils::Raycast(world[idx], bottomMiddle, topMiddle, hitInfo)))
			{
				hasCollision = true;
			}
		}
		else if (entity->GetVelocity().y <= 0.f)
		{
			if ((utils::Raycast(world[idx], bottomRight, topRight, hitInfo)
				or utils::Raycast(world[idx], bottomLeft, topRight, hitInfo)
				or utils::Raycast(world[idx], bottomMiddle, topMiddle, hitInfo)))
			{
				if (hitInfo.lambda <= 0.3f)
				{
					Vector2f newVelocity{ entity->GetVelocity().x, 0 };
					entity->SetVelocity(newVelocity);

					Point2f newPosition
					{
						entity->GetPosition().x,
						hitInfo.intersectPoint.y + utils::GetSign(hitInfo.normal.y) * (entity->GetHitBox().height / 2)
					};
					entity->SetPosition(newPosition);
					return true;
				}
			}
		}
	}

	if (hasCollision)
	{
		Vector2f newVelocity{ entity->GetVelocity().x, 0 };
		entity->SetVelocity(newVelocity);

		Point2f newPosition
		{
			entity->GetPosition().x,
			hitInfo.intersectPoint.y + utils::GetSign(hitInfo.normal.y) * (entity->GetHitBox().height / 2)
		};
		entity->SetPosition(newPosition);
	}

	return hasCollision;
}

bool Collision::WallCollision(Projectile* projectile, const std::vector<std::vector<Point2f>>& world)
{
	const Point2f left			{ projectile->GetHitBox().left,										projectile->GetHitBox().bottom + projectile->GetHitBox().height / 2};
	const Point2f right			{ projectile->GetHitBox().left + projectile->GetHitBox().width,		projectile->GetHitBox().bottom + projectile->GetHitBox().height / 2};
	utils::HitInfo hitInfo		{ projectile->GetHitInfo() };
	
	for (size_t idx{}; idx < world.size(); idx++)
	{
		if (utils::Raycast(world[idx], left, right, hitInfo))
		{
			if (projectile->DoesParticleOnImpact()) ParticleSystem::AddImpactParticles(projectile->GetPosition());
			return true;
		}
	}
	return false;
}
bool Collision::FloorCollision(Projectile* projectile, const std::vector<std::vector<Point2f>>& world)
{
	const Point2f top			{ projectile->GetHitBox().left + projectile->GetHitBox().width / 2,		projectile->GetHitBox().bottom + projectile->GetHitBox().height };
	const Point2f bottom		{ projectile->GetHitBox().left + projectile->GetHitBox().width / 2,		projectile->GetHitBox().bottom };
	utils::HitInfo hitInfo		{ projectile->GetHitInfo() };

	for (size_t idx{}; idx < world.size(); idx++)
	{
		if (utils::Raycast(world[idx], top, bottom, hitInfo))
		{
			if (projectile->DoesParticleOnImpact()) ParticleSystem::AddImpactParticles(projectile->GetPosition());
			return true;
		}
	}
	return false;
}

bool Collision::WallCollision(PowerUp* powerUp, const std::vector<std::vector<Point2f>>& world)
{
	const Point2f left		{ powerUp->GetHitBox().left,										powerUp->GetHitBox().bottom + powerUp->GetHitBox().height / 2 };
	const Point2f right		{ powerUp->GetHitBox().left + powerUp->GetHitBox().width,		powerUp->GetHitBox().bottom + powerUp->GetHitBox().height / 2 };
	utils::HitInfo hitInfo	{ powerUp->GetHitInfo() };

	for (size_t idx{}; idx < world.size(); idx++)
	{
		if (utils::Raycast(world[idx], left, right, hitInfo))
		{
			Point2f newPosition
			{
				hitInfo.intersectPoint.x + utils::GetSign(hitInfo.normal.x) * ((powerUp->GetHitBox().width / 2.f) + 1),
				powerUp->GetPosition().y
			};
			powerUp->SetPosition(newPosition);
			return true;
		}
	}
	return false;
}
bool Collision::FloorCollision(PowerUp* powerUp, const std::vector<std::vector<Point2f>>& world)
{
	const Point2f top		{ powerUp->GetHitBox().left + powerUp->GetHitBox().width / 2,		powerUp->GetHitBox().bottom + powerUp->GetHitBox().height };
	const Point2f bottom	{ powerUp->GetHitBox().left + powerUp->GetHitBox().width / 2,		powerUp->GetHitBox().bottom };
	utils::HitInfo hitInfo	{ powerUp->GetHitInfo() };

	for (size_t idx{}; idx < world.size(); idx++)
	{
		if (utils::Raycast(world[idx], top, bottom, hitInfo))
		{
			Point2f newPosition
			{
				powerUp->GetPosition().x,
				hitInfo.intersectPoint.y + utils::GetSign(hitInfo.normal.y) * (powerUp->GetHitBox().height / 2)
			};
			powerUp->SetPosition(newPosition);
			powerUp->SetVelocity(Vector2f(0, 0));
			return true;
		}
	}
	return false;
}

bool Collision::KirbyHitDetection(Kirby* pKirby, std::vector<Enemy*>& vEnemies, std::vector<Projectile*>& vProjectiles)
{
	bool kirbyGotHitByEnemy{false};
	bool kirbyGotHitByProjectile{false};

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	KIRBY-ENEMY HIT DETECTION & HANDLING    ~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (Enemy* enemyPtr : vEnemies)
	{
		if (!enemyPtr->IsEliminated())
		{
			if (Collision::EntityCollision(enemyPtr, pKirby))
			{
				enemyPtr->Reset();
				ParticleSystem::AddEnemyDeathParticles(enemyPtr->GetPosition(), static_cast<Direction>(-utils::GetSign(pKirby->GetPosition().x - enemyPtr->GetPosition().x)));
				if (enemyPtr->IsActivated())
				{
					//SoundManager::PlayEffect("EnemyDeath");
					pKirby->AddToScore(enemyPtr->GetScore() * 2);
					pKirby->HitEnemy(enemyPtr->GetPosition(), Projectile::Element::None);
					kirbyGotHitByEnemy = true;
				}
			}
		}
	}

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	KIRBY/ENEMY - PROJECTILE HIT DETECTION & HANDLING	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (Projectile* projectilePtr : vProjectiles)
	{
		if (projectilePtr->IsActivated() and projectilePtr->DoesEntityCollision())
		{
			if (projectilePtr->IsFriendly())
			{
				for (Enemy* enemyPtr : vEnemies)
				{
					if (!enemyPtr->IsEliminated())
					{
						if (Collision::ProjectileCollision(enemyPtr, projectilePtr))
						{
							enemyPtr->Reset();
							ParticleSystem::AddEnemyDeathParticles(enemyPtr->GetPosition(), static_cast<Direction>(-utils::GetSign(projectilePtr->GetPosition().x - enemyPtr->GetPosition().x)));
							SoundManager::PlayEffect("EnemyDeath");
							pKirby->AddToScore(enemyPtr->GetScore() * 2);
							if (projectilePtr->DoesDeactivateOnImpact()) projectilePtr->Deactivate();
						}
					}
				}
			}
			else
			{
				if (Collision::ProjectileCollision(pKirby, projectilePtr))
				{
					pKirby->HitEnemy(projectilePtr->GetPosition(), projectilePtr->GetElement());
					kirbyGotHitByProjectile = true;
				}

			}
		}
	}

	return kirbyGotHitByEnemy or kirbyGotHitByProjectile;
}

bool Collision::EntityCollision(Entity* entity1, Entity* entity2)
{
	return utils::IsOverlapping(entity1->GetHitBox(), entity2->GetHitBox());
}
bool Collision::ProjectileCollision(Entity* entity, Projectile* projectile)
{
	return utils::IsOverlapping(entity->GetHitBox(), projectile->GetHitBox());
}

bool Collision::PowerUpCollision(Entity* entity, PowerUp* powerUp)
{
	return utils::IsOverlapping(entity->GetHitBox(), powerUp->GetHitBox());
}
