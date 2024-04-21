#include "pch.h"
#include <iostream>
#include "utils.h"
#include "Ability.h"


Ability::Ability(Ability::Type abilityType)
	: m_Type{abilityType}
	, m_AccumSec{0}
	, m_IsActive{false}
	, m_Position{Point2f(0, 0)}
	, m_DstRect{ Rectf(0, 0, 16, 16)}
	, m_SrcRect{ Rectf(0, 0, 16, 16) }
{
	m_pSpritesheet = new Texture("Abilities/Abilities.png");

	m_TranslationMatrix.SetAsTranslate(0, 0);
	m_RotationMatrix.SetAsRotate(0);
	m_ScalingMatrix.SetAsScale(1);
	m_TransformationMatrix = m_TranslationMatrix * m_RotationMatrix * m_ScalingMatrix;
}

Ability::~Ability()
{
	delete m_pSpritesheet;
	m_pSpritesheet = nullptr;
}

void Ability::Use()
{
	m_AccumSec = 0;
	m_IsActive = true;
}

Ability::Type Ability::GetAbilityType() const
{
	return m_Type;
}

bool Ability::IsActivated() const
{
	return m_IsActive;
}
const std::vector<Rectf>& Ability::GetHitboxes() const
{
	return m_vHitBoxes;
}
void Ability::Deactivate()
{
	m_IsActive = false;
}

void Ability::SetAbility(Ability::Type abilityType)
{
	m_Type = abilityType;
}

void Ability::Update(float elapsedSec, const Point2f& position, int direction)
{
	m_AccumSec += elapsedSec;
	m_Position = position;
	switch (m_Type)
	{
	case Ability::Type::None:
		break;
	case Ability::Type::Beam:
		UseBeam(elapsedSec, position, direction);
		break;
	case Ability::Type::Fire:
		UseFire(elapsedSec, position, direction);
		break;
	case Ability::Type::Spark:
		break;
	default:
		break;
	}

}
void Ability::Draw() const
{
	switch (m_Type)
	{
	case Ability::Type::None:
		break;
	case Ability::Type::Beam:
		DrawBeam();
		break;
	case Ability::Type::Fire:
		DrawFire();
		break;
	case Ability::Type::Spark:
		break;
	default:
		break;
	}
}

#pragma region Beam
void Ability::UseBeam(float elapsedSec, const Point2f& position, int direction)
{
	m_SrcRect.left = 0;
	m_SrcRect.bottom = -static_cast<int>(m_Type) * m_SrcRect.height;

	if (m_AccumSec >= m_BEAM_UPTIME)
	{
		m_AccumSec = 0;
		Deactivate();
	}
	else
	{
		for (int index{}; index < m_BEAM_SEGMENTS; ++index)
		{
			m_TranslationMatrix.SetAsTranslate(m_Position.x, m_Position.y);
			m_RotationMatrix.SetAsRotate(-direction * m_ANGLE_RANGE * m_AccumSec);
			m_TransformationMatrix = m_TranslationMatrix * m_RotationMatrix;
			
			m_TranslationMatrix.SetAsTranslate(0, 12 * (index + 1));
			m_RotationMatrix.SetAsRotate(direction * m_ANGLE_RANGE * m_AccumSec);
			m_TransformationMatrix = m_TransformationMatrix * m_TranslationMatrix * m_RotationMatrix;
			
			m_TranslationMatrix.SetAsTranslate(-m_DstRect.width / 2, -m_DstRect.height / 2);
			m_TransformationMatrix = m_TransformationMatrix * m_TranslationMatrix;

			std::vector<Point2f> transformedDstRect;
			transformedDstRect = m_TransformationMatrix.Transform(m_DstRect);
			m_vHitBoxes[index] = utils::VectorToRect(transformedDstRect);

			std::cout << "LEFT: " << m_vHitBoxes[index].left << "\tBOTTOM: " << m_vHitBoxes[index].bottom << "\tWIDTH: " << m_vHitBoxes[index].width << "\tHEIGHT: " << m_vHitBoxes[index].height << "\n";
		}
	}
}
void Ability::DrawBeam() const
{
	int randIdx{ rand() % 5 };

	for (size_t index = 0; index < m_BEAM_SEGMENTS; ++index)
	{
		if (index != randIdx)
		{
			m_pSpritesheet->Draw(m_vHitBoxes[index], m_SrcRect);

			utils::SetColor(Color4f(1, 1, 1, 1));
			utils::DrawRect(m_vHitBoxes[index]);
		}
	}
}
#pragma endregion

#pragma region Fire
void Ability::UseFire(float elapsedSec, const Point2f& position, int direction)
{
	m_SrcRect.left = 0;
	m_SrcRect.bottom = -static_cast<int>(m_Type) * m_SrcRect.height;

	if (m_AccumSec >= m_FIRE_UPTIME)
	{
		m_AccumSec = 0;
		Deactivate();
	}
	else
	{
		for (int index{}; index < m_FIREBALLS; ++index)
		{
			float xDisplacement{};
			xDisplacement = static_cast<float>(static_cast<int>(m_FIRE_VELOCITY.x * m_AccumSec) % static_cast<int>(m_FIRE_RANGE));


			m_TranslationMatrix.SetAsTranslate(m_Position.x + xDisplacement, m_Position.y );
			m_TransformationMatrix = m_TranslationMatrix;

			m_TranslationMatrix.SetAsTranslate(m_DstRect.width / 2 + index * 16, -m_DstRect.height / 2);
			m_TransformationMatrix = m_TransformationMatrix * m_TranslationMatrix;

			std::vector<Point2f> transformedDstRect;
			transformedDstRect = m_TransformationMatrix.Transform(m_DstRect);
			m_vHitBoxes[index] = utils::VectorToRect(transformedDstRect);

			std::cout << "LEFT: " << m_vHitBoxes[index].left << "\tBOTTOM: " << m_vHitBoxes[index].bottom << "\tWIDTH: " << m_vHitBoxes[index].width << "\tHEIGHT: " << m_vHitBoxes[index].height << "\n";
		}
	}
}
void Ability::DrawFire() const
{
	for (size_t index = 0; index < m_FIREBALLS; ++index)
	{
		m_pSpritesheet->Draw(m_vHitBoxes[index], m_SrcRect);

		utils::SetColor(Color4f(1, 1, 1, 1));
		utils::DrawRect(m_vHitBoxes[index]);
	}
}
#pragma endregion