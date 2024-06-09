#include "pch.h"
#include "GameOverScreen.h"

GameOverScreen::GameOverScreen(float scale, Rectf viewport)
	: BaseState(viewport)
	, m_Scale{ scale }
	, m_pTexture{ TextureManager::GetTexture("GameOverScreen") }
	, m_Option{ Option::Continue }
	, m_AccumSec{0}
	, m_SrcRect{0, 0, 256, 240}
	, m_ShowOptions{false}
{
}

void GameOverScreen::Enter()
{
	SoundManager::PlayEffect("GameOver");
	m_ShowOptions = false;
	m_AccumSec = 0;
	ViewFade::StartFade(0.5f);
	m_Option = Option::Continue;
	m_SrcRect.left = 0;
}

void GameOverScreen::Exit()
{
	ViewFade::StartFade(0.5f);
}

void GameOverScreen::Update(StateMachine& stateMachine, float elapsedSec, bool freeze)
{
	m_AccumSec += elapsedSec;
	if (m_AccumSec >= 3.f and !m_ShowOptions)
	{
		ViewFade::StartFade(0.3f);
		if (ViewFade::IsFadingIn())
		{
			m_ShowOptions = true;
			m_SrcRect.left = 256;
		}
	}

	if (m_ShowOptions)
	{
		if (utils::KeyPress(SDL_SCANCODE_DOWN))
		{
			m_Option = Option::Exit;
		}
		else if (utils::KeyPress(SDL_SCANCODE_UP))
		{
			m_Option = Option::Continue;
		}
		else if (utils::KeyPress(SDL_SCANCODE_RETURN))
		{
			switch (m_Option)
			{
			case GameOverScreen::Option::Continue:
				stateMachine.ChangeState(StateMachine::State::Gameplay);
				break;
			case GameOverScreen::Option::Exit:

				break;
			default:
				break;
			}
		}

	}
}

void GameOverScreen::Draw() const
{
	m_pTexture->Draw(m_VIEWPORT, m_SrcRect);
	if (m_ShowOptions)
	{
		const Point2f optionPos
		{
			m_Scale * 60,
			m_Scale * (150 - static_cast<int>(m_Option) * 34)
		};
		m_pTexture->Draw(Rectf(optionPos.x, optionPos.y, m_Scale * 16, m_Scale * 16), Rectf(512, 0, 16, 16));
	}
}
