#include "pch.h"
#include "TitleScreen.h"

TitleScreen::TitleScreen(float scale)
	: m_Scale{ scale }
	, m_pTexture{ TextureManager::GetTexture("TitleScreen") }
	, m_Animation{}
{
	std::vector<Frame> frames
	{
		Frame(Rectf(0, 0, 64, 55), 0.3f),
		Frame(Rectf(64, 0, 64, 55), 0.1f),
		Frame(Rectf(0, 0, 64, 55), 0.1f),
		Frame(Rectf(64, 0, 64, 55), 0.1f),
		Frame(Rectf(0, 0, 64, 55), 0.3f),
		Frame(Rectf(128, 0, 64, 55), 0.1f),
		Frame(Rectf(0, 0, 64, 55), 0.1f),
		Frame(Rectf(128, 0, 64, 55), 0.1f),
		Frame(Rectf(0, 0, 64, 55), 0.1f),
		Frame(Rectf(128, 0, 64, 55), 0.1f),
		Frame(Rectf(0, 0, 64, 55), 0.1f),
		Frame(Rectf(128, 0, 64, 55), 0.1f),
		Frame(Rectf(0, 0, 64, 55), 0.1f),
		Frame(Rectf(128, 0, 64, 55), 0.1f),
		Frame(Rectf(0, 0, 64, 55), 0.1f)
	};
	m_Animation = Animation(std::move(frames), true);
}

void TitleScreen::Enter()
{
	ViewFade::StartFadeIn(1.5f);
	SoundManager::PlayStream("TitleMusic");
	m_Animation.Activate();
}

void TitleScreen::Exit()
{
	m_Animation.Deactivate();
	ViewFade::StartFade(0.5f);
}

void TitleScreen::Update(float elapsedSec)
{
	m_Animation.Update(elapsedSec);
}

void TitleScreen::Draw() const
{
	
	m_pTexture->Draw(Rectf(0, 0, m_Scale * 248, m_Scale * 224), Rectf(0, -58, 248, 224));
	m_pTexture->Draw(Rectf(0, 0, m_Scale * 248, m_Scale * 224), Rectf(0, -58, 248, 224));

	glPushMatrix();
	glTranslatef(248, 224, 0);
	glScalef(m_Scale, m_Scale, 0);
	glTranslatef(-248, -224, 0);
	m_Animation.Draw(m_pTexture, Point2f(248, 215));
	glPopMatrix();
}
