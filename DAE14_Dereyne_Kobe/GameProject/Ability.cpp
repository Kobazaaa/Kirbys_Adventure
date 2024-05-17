#include "pch.h"
#include "TextureManager.h"
#include "Ability.h"

Ability::Ability(bool canBeCancelled)
	: m_AccumSec		{ 0.f }
	, m_IsActive		{ false }
	, m_CanBeCancelled	{ canBeCancelled }
{
}

Ability::~Ability() noexcept
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
	}
}

void Ability::Activate()
{
	m_IsActive = true;
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

bool Ability::CanBeCancelled() const
{
	return m_CanBeCancelled;
}

