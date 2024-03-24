#include "pch.h"
#include "Collision.h"
#include "Entity.h"

Collision::Collision()
{
}

Collision::~Collision()
{
}

bool Collision::WallCollision(Entity* entity, const std::vector<std::vector<Point2f>>& world)
{
	const Point2f left	{ entity->m_Center.x - (entity->m_Width / 2), entity->m_Center.y - entity->m_Height / 2 + 5};
	const Point2f right	{ entity->m_Center.x + (entity->m_Width / 2), entity->m_Center.y - entity->m_Height / 2 + 5};

	for (size_t idx{}; idx < world.size(); idx++)
	{
		if (utils::Raycast(world[idx], left, right, entity->m_HitInfo))
		{
			entity->m_Velocity.x = 0;
			entity->m_Center.x = entity->m_HitInfo.intersectPoint.x - static_cast<int>(entity->m_Direction) * entity->m_Width / 2;
			entity->m_WorldCollision = true;
		}
	}
	return entity->m_WorldCollision;
}

bool Collision::FloorCollision(Entity* entity, const std::vector<std::vector<Point2f>>& world)
{
	const Point2f topLeft		{ entity->m_Center.x - (entity->m_Width / 2), entity->m_Center.y + (entity->m_Height / 2) };
	const Point2f topRight		{ entity->m_Center.x + (entity->m_Width / 2), entity->m_Center.y + (entity->m_Height / 2) };
	const Point2f topMiddle		{ entity->m_Center.x						, entity->m_Center.y + (entity->m_Height / 2) };
	const Point2f bottomLeft	{ entity->m_Center.x - (entity->m_Width / 2), entity->m_Center.y - (entity->m_Height / 2) };
	const Point2f bottomRight	{ entity->m_Center.x + (entity->m_Width / 2), entity->m_Center.y - (entity->m_Height / 2) };
	const Point2f bottomMiddle	{ entity->m_Center.x						, entity->m_Center.y - (entity->m_Height / 2) };

	entity->m_WorldCollision = false;

	for (size_t idx{}; idx < world.size(); idx++)
	{
		if (entity->m_Direction == Entity::Direction::Left)
		{
			if (utils::Raycast(world[idx], bottomRight, topRight, entity->m_HitInfo)) entity->m_WorldCollision = true;
			else if (utils::Raycast(world[idx], bottomLeft, topLeft, entity->m_HitInfo)) entity->m_WorldCollision = true;
			else if (utils::Raycast(world[idx], bottomMiddle, topMiddle, entity->m_HitInfo)) entity->m_WorldCollision = true;
		}
		else
		{
			if (utils::Raycast(world[idx], bottomLeft, topLeft, entity->m_HitInfo)) entity->m_WorldCollision = true;
			else if (utils::Raycast(world[idx], bottomRight, topRight, entity->m_HitInfo)) entity->m_WorldCollision = true;
			else if (utils::Raycast(world[idx], bottomMiddle, topMiddle, entity->m_HitInfo)) entity->m_WorldCollision = true;
		}

		if (entity->m_WorldCollision == true)
		{
			entity->m_Velocity.y = 0;
			if (entity->m_HitInfo.normal.y > 0) entity->m_Center.y = entity->m_HitInfo.intersectPoint.y + (entity->m_Height / 2);
			else entity->m_Center.y = entity->m_HitInfo.intersectPoint.y - (entity->m_Height / 2);

		}
	}

	return entity->m_WorldCollision;
}

bool Collision::EntityCollision(Entity* entity, const std::vector<std::vector<Point2f>>& world)
{
	return false;
}
