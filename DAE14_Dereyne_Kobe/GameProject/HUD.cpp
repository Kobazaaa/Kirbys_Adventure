#include "pch.h"
#include "HUD.h"

HUD::HUD(Kirby* kirby, float scale)
	: m_pKIRBY		 { kirby }
	, m_AccumSec	 { 0 }
	, m_Scale		 { scale }
	, m_CurrentFrame { 0 }
{
	m_pSprites = new Texture("HUD.png");
}

HUD::~HUD()
{
	delete m_pSprites;
	m_pSprites = nullptr;
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

void HUD::PrintScore() const
{
	int score{ m_pKIRBY->GetScore() };
	if (score < 0) score = 0;

	for (size_t idx{0}; idx < 7; ++idx)
	{
		const Point2f pos{ m_SCORE_POS.x + (6 - idx) * m_NUMBER_WIDTH, m_SCORE_POS.y};
		int nrToPrint{ score / int(powf(10, idx)) % 10};
		const Rectf srcRect{nrToPrint * m_NUMBER_WIDTH, 0, m_NUMBER_WIDTH, m_NUMBER_HEIGHT};
		m_pSprites->Draw(pos, srcRect);
	}
}

void HUD::DrawHealth() const
{
	int health{ m_pKIRBY->GetHealth() };
	if (health < 0) health = 0;

	for (size_t idx{ 0 }; idx < 6; ++idx)
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
		int nrToPrint{ lives / int(powf(10, idx)) % 10};
		const Rectf srcRect{ nrToPrint * m_NUMBER_WIDTH, 0, m_NUMBER_WIDTH, m_NUMBER_HEIGHT };
		m_pSprites->Draw(pos, srcRect);
	}
}

void HUD::DrawLivesAnim() const
{
	const Point2f pos{ m_KIRBY_ANIM_POS.x, m_KIRBY_ANIM_POS.y };
	const Rectf srcRect{ (m_CurrentFrame % 4) * m_KIRBY_ANIM_WIDTH, 289, m_KIRBY_ANIM_WIDTH, m_KIRBY_ANIM_HEIGHT};
	m_pSprites->Draw(pos, srcRect);
}

void HUD::DrawCard() const
{

}
