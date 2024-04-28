#include "pch.h"
#include "TextureManager.h"
#include "Ability.h"

Ability::Ability()
	: m_AccumSec{0}
	, m_IsActive{false}
	, m_LifeTime{}
{
}

Ability::~Ability()
{
	for (int index{}; index < m_vProjectiles.size(); ++index)
	{
		delete m_vProjectiles[index];
		m_vProjectiles[index] = nullptr;
	}
}

void Ability::Draw() const
{
	for (int index{}; index < m_vProjectiles.size(); ++index)
	{
		m_vProjectiles[index]->Draw();

		//utils::DrawRect(m_vProjectiles[index]->GetHitBox());
		//utils::DrawEllipse(m_vProjectiles[index]->GetPosition(), 2, 2);
	}
}

void Ability::Deactivate()
{
	for (int index{}; index < m_vProjectiles.size(); ++index)
	{
		m_vProjectiles[index]->Deactivate();
	}
	
	m_IsActive = false;
	m_AccumSec = 0;
}

const std::vector<Projectile*>& Ability::GetProjectiles() const
{
	return m_vProjectiles;
}

bool Ability::IsActive() const
{
	return m_IsActive;
}

