#include "pch.h"
#include "Gameplay.h"
#include "Entity.h"
#include "PauseScreen.h"

PauseScreen::PauseScreen(float scale, Rectf viewport, Gameplay* gameplayState)
	: BaseState(viewport)
	, m_Scale {scale}
	, m_pTexture {TextureManager::GetTexture("PauseScreen")}
	, m_Option{ Option::Continue }
	, m_pGameplay{gameplayState}
{
}

void PauseScreen::Enter()
{
	ViewFade::StartFade(0.5f);
	SoundManager::PlayEffect("Pause");
	m_Option = Option::Continue;
}

void PauseScreen::Exit()
{
	ViewFade::StartFade(0.5f);
	SoundManager::PlayEffect("Unpause");
}

void PauseScreen::Update(StateMachine& stateMachine, float elapsedSec, bool freeze)
{
	if (utils::KeyPress(SDL_SCANCODE_LEFT))
	{
		SoundManager::AdjustVolume(-5);
		SoundManager::PlayEffect("Land");
	}
	if (utils::KeyPress(SDL_SCANCODE_RIGHT))
	{
		SoundManager::AdjustVolume(5);
		SoundManager::PlayEffect("Jump");

	}

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
		case PauseScreen::Option::Continue:
			stateMachine.ChangeState(StateMachine::State::Gameplay);
			break;
		case PauseScreen::Option::Exit:
			m_pGameplay->Reset();
			SoundManager::StopAll();
			stateMachine.ChangeState(StateMachine::State::Titlescreen);
			break;
		default:
			break;
		}
	}
}

void PauseScreen::Draw() const
{
	m_pTexture->Draw(Rectf(0, 0, m_VIEWPORT.width, m_VIEWPORT.height), Rectf(0, -262, 248, 224));
	
	Entity::AbilityType kirbyAbil{ m_pGameplay->GetKirbyAbility() };
	if (kirbyAbil == Entity::AbilityType::None)  m_pTexture->Draw(Rectf(m_Scale * 24, m_Scale * 88, m_Scale * 192, m_Scale * 128), Rectf(0, -131, 192, 128));
	if (kirbyAbil == Entity::AbilityType::Spark) m_pTexture->Draw(Rectf(m_Scale * 24, m_Scale * 88, m_Scale * 192, m_Scale * 128), Rectf(0, 0, 192, 128));
	if (kirbyAbil == Entity::AbilityType::Beam)  m_pTexture->Draw(Rectf(m_Scale * 24, m_Scale * 88, m_Scale * 192, m_Scale * 128), Rectf(195, 0, 192, 128));
	if (kirbyAbil == Entity::AbilityType::Fire)  m_pTexture->Draw(Rectf(m_Scale * 24, m_Scale * 88, m_Scale * 192, m_Scale * 128), Rectf(195, -131, 192, 128));

	const Point2f optionPos
	{
		m_Scale * 70,
		m_Scale * (37 - static_cast<int>(m_Option) * 15)
	};
	TextureManager::GetTexture("Kirby2")->Draw(Rectf(optionPos.x, optionPos.y, m_Scale * 16, m_Scale * 16), Rectf(0, 0, 16, 16));
}
