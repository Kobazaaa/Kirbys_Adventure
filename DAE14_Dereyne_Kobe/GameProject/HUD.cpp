#include "pch.h"
#include "HUD.h"
#include "TextureManager.h"
#include <typeinfo>

HUD::HUD(Kirby* kirby, float scale)
	: m_pKIRBY		 { kirby }
	, m_AccumSec	 { 0 }
	, m_Scale		 { scale }
	, m_CurrentFrame { 0 }
	, m_pSprites	 { TextureManager::GetTexture("HUD")}
{
}

void HUD::Draw() const
{
	glPushMatrix();
	{
		glScalef(m_Scale, m_Scale, 0);
		m_pSprites->Draw(m_HUD_POS, Rectf(0, 248, m_HUD_WIDTH, m_HUD_HEIGHT));
		PrintScore();
		DrawHealth();
		DrawLives();
		DrawLivesAnim();
		DrawCard();
	}
	glPopMatrix();

}

void HUD::Update(float elapsedSec)
{
	m_AccumSec += elapsedSec;

	if (m_AccumSec >= 0.25f)
	{
		++m_CurrentFrame;
		m_AccumSec = 0;
	}

}

float HUD::GetHeight() const
{
	return m_HUD_HEIGHT;
}

void HUD::PrintScore() const
{
	int score{ m_pKIRBY->GetScore() };
	if (score < 0) score = 0;

	for (int idx{0}; idx < 7; ++idx)
	{
		const Point2f pos	{ m_SCORE_POS.x + (6.f - static_cast<float>(idx)) * m_NUMBER_WIDTH, m_SCORE_POS.y};
		int nrToPrint		{ score / int(powf(10.f, static_cast<float>(idx))) % 10};
		
		const Rectf srcRect
		{
			nrToPrint * m_NUMBER_WIDTH,
			0,
			m_NUMBER_WIDTH,
			m_NUMBER_HEIGHT
		};
		
		m_pSprites->Draw(pos, srcRect);
	}
}

void HUD::DrawHealth() const
{
	int health{ m_pKIRBY->GetHealth() };
	if (health < 0) health = 0;

	for (int idx{ 0 }; idx < 6; ++idx)
	{
		Point2f pos{};
		Rectf srcRect{};
		if (idx < health)
		{
			pos		= Point2f( m_KIRBY_HEALTH_POS.x + idx * m_KIRBY_HEALTH_WIDTH, m_KIRBY_HEALTH_POS.y );
			srcRect = Rectf( m_KIRBY_HEALTH_WIDTH * (m_CurrentFrame % 2), -8, m_KIRBY_HEALTH_WIDTH, m_KIRBY_HEALTH_HEIGHT );
		}
		else
		{
			pos		= Point2f( m_KIRBY_HEALTH_POS.x + idx * m_KIRBY_HEALTH_WIDTH, m_KIRBY_HEALTH_POS.y );
			srcRect = Rectf( 16, -8, m_KIRBY_HEALTH_WIDTH, m_KIRBY_HEALTH_HEIGHT );
		}
		m_pSprites->Draw(pos, srcRect);
	}

}

void HUD::DrawLives() const
{
	int lives{ m_pKIRBY->GetLives() };
	if (lives < 0) lives = 0;

	for (size_t idx{0}; idx < 2; ++idx)
	{
		const Point2f pos{ m_KIRBY_LIVES_POS.x + (1 - idx) * m_NUMBER_WIDTH, m_KIRBY_LIVES_POS.y };
		int nrToPrint{ lives / int(powf(10, static_cast<float>(idx))) % 10};
		
		const Rectf srcRect
		{
			nrToPrint * m_NUMBER_WIDTH,
			0,
			m_NUMBER_WIDTH,
			m_NUMBER_HEIGHT
		};
		
		m_pSprites->Draw(pos, srcRect);
	}
}

void HUD::DrawLivesAnim() const
{
	const Point2f pos	{ m_KIRBY_ANIM_POS.x, m_KIRBY_ANIM_POS.y };
	const Rectf srcRect
	{ 
		(m_CurrentFrame % 4) * m_KIRBY_ANIM_WIDTH,
		289,
		m_KIRBY_ANIM_WIDTH,
		m_KIRBY_ANIM_HEIGHT
	};
	
	m_pSprites->Draw(pos, srcRect);
}

void HUD::DrawCard() const
{
	const Point2f pos{ m_CARD_POS.x, m_CARD_POS.y};
	Rectf srcRect
	{
		0,
		120,
		m_CARD_WIDTH,
		m_CARD_HEIGHT
	};

	if (m_pKIRBY->GetAbilityType() != Entity::AbilityType::None)
	{
		if (m_pKIRBY->GetAbilityType() == Entity::AbilityType::Beam)	  srcRect.left = m_CARD_WIDTH;
		if (m_pKIRBY->GetAbilityType() == Entity::AbilityType::Fire)	  srcRect.left = 2 * m_CARD_WIDTH;
		if (m_pKIRBY->GetAbilityType() == Entity::AbilityType::Spark)	  srcRect.left = 3 * m_CARD_WIDTH;
	}
	else srcRect.bottom = 120;

	m_pSprites->Draw(pos, srcRect);
}
