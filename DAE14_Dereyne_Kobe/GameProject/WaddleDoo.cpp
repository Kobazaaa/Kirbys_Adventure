#include "pch.h"
#include "WaddleDoo.h"

WaddleDoo::WaddleDoo(const Point2f& center)
	: Enemy("WaddleDoo.png", center)
{
}

void WaddleDoo::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	Collision::FloorCollision(this, world);
	if (Collision::WallCollision(this, world)) InverseDirection();

	Enemy::Update(elapsedSec, world);

	UpdateAnimation();


	if (utils::KeyPress(SDL_SCANCODE_SPACE))
	{

	}

}

void WaddleDoo::UpdateAnimation()
{
	if (m_AccumSec >= 0.2f)
	{
		m_AccumSec = 0;
		++m_CurrentFrame;
		m_CurrentFrame = m_CurrentFrame % 2;
	}
}
