#include "pch.h"
#include "Collision.h"
#include "Entity.h"

Collision::Collision()
{
}

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
				//entity->SetVelocity(Vector2f(0, entity->GetVelocity().y));

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
	const Point2f bottomMiddle	{ entity->GetHitBox().left + entity->GetHitBox().width / 2,	entity->GetHitBox().bottom };
	utils::HitInfo hitInfo		{ entity->GetHitInfo() };

	for (size_t idx{}; idx < world.size(); idx++)
	{
		if  (utils::Raycast(world[idx], bottomRight,  topRight,	 hitInfo)
		or  (utils::Raycast(world[idx], bottomLeft,   topLeft,	 hitInfo)
		or  (utils::Raycast(world[idx], bottomMiddle, topMiddle, hitInfo))))
		{
			if (hitInfo.lambda <= 0.2f)
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
	return false;
}
bool Collision::EntityCollision(Entity* entity1, Entity* entity2)
{
	return utils::IsOverlapping(entity1->GetHitBox(), entity2->GetHitBox());
}

//bool Collision::ObjectCollision(Entity* entity, Object* object)
//{
//	return utils::IsOverlapping(entity->GetHitBox(), object->GetHitBox());
//}
