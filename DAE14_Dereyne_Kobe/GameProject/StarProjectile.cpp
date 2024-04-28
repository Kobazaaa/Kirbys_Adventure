#include "pch.h"
#include "StarProjectile.h"

StarProjectile::StarProjectile(bool isFriendly)
	: Projectile("StarProjectile", Vector2f(200.f, 0.f), 99999.f, isFriendly)
	, m_AnimationAccumSec{ 0 }
	, m_Type{ StarProjectile::Type::Single }
{
	m_CurrentFrameRow = 0;
}

void StarProjectile::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index)
{
	Projectile::Update(elapsedSec, world);

	if (m_IsActive)
	{
		m_AnimationAccumSec += elapsedSec;
		if (m_AnimationAccumSec > 0.1f)
		{
			m_CurrentFrame = (m_CurrentFrame + 1) % 4;
			m_AnimationAccumSec = 0;
		}
	}

}
