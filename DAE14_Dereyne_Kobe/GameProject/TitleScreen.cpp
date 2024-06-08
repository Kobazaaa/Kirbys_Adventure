#include "pch.h"
#include "TitleScreen.h"

TitleScreen::TitleScreen(float scale, Rectf viewport)
	: BaseState(viewport)
	, m_Scale{ scale }
	, m_pTexture{ TextureManager::GetTexture("TitleScreen") }
	, m_Animation{ Animation(std::vector<Frame>{}, true) }
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

	m_Animation = Animation{ std::move(frames), true };
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

void TitleScreen::Update(StateMachine& stateMachine, float elapsedSec, bool freeze)
{
	m_Animation.Update(elapsedSec);
}

void TitleScreen::Draw() const
{
	m_pTexture->Draw(Rectf(0, 0, m_VIEWPORT.width, m_VIEWPORT.height), Rectf(0, -58, 248, 224));
	m_pTexture->Draw(Rectf(0, 0, m_VIEWPORT.width, m_VIEWPORT.height), Rectf(0, -58, 248, 224));

	glPushMatrix();
	glTranslatef(m_VIEWPORT.width / 2, m_VIEWPORT.height / 2, 0);
	glScalef(m_Scale, m_Scale, 0);
	glTranslatef(-m_VIEWPORT.width / 2, -m_VIEWPORT.height / 2, 0);
	m_Animation.Draw(m_pTexture, Point2f(248, 215));
	glPopMatrix();
}
