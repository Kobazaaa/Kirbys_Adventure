#include "pch.h"
#include "PauseScreen.h"

PauseScreen::PauseScreen(float scale)
	: m_Scale {scale}
	, m_pTexture {TextureManager::GetTexture("PauseScreen")}
{
}

void PauseScreen::Enter()
{
	ViewFade::StartFade(0.5f);
	SoundManager::PlayEffect("Pause");
}

void PauseScreen::Exit()
{
	ViewFade::StartFade(0.5f);
	SoundManager::PlayEffect("Unpause");
}

void PauseScreen::Update(float elapsedSec)
{
	if (utils::KeyPress(SDL_SCANCODE_DOWN))
	{

	}
	else if (utils::KeyPress(SDL_SCANCODE_UP))
	{

	}
}

void PauseScreen::Draw() const
{
	m_pTexture->Draw(Rectf(0, 0, m_Scale * 248, m_Scale * 224), Rectf(0, -262, 248, 224));
	//if (m_pKirby->GetAbilityType() == Entity::AbilityType::None)  m_pTexture->Draw(Rectf(m_SCALE * 24, m_SCALE * 88, m_SCALE * 192, m_SCALE * 128), Rectf(0, -131, 192, 128));
	//if (m_pKirby->GetAbilityType() == Entity::AbilityType::Spark) m_pTexture->Draw(Rectf(m_SCALE * 24, m_SCALE * 88, m_SCALE * 192, m_SCALE * 128), Rectf(0, 0, 192, 128));
	//if (m_pKirby->GetAbilityType() == Entity::AbilityType::Beam)  m_pTexture->Draw(Rectf(m_SCALE * 24, m_SCALE * 88, m_SCALE * 192, m_SCALE * 128), Rectf(195, -131, 192, 128));
	//if (m_pKirby->GetAbilityType() == Entity::AbilityType::Fire)  m_pTexture->Draw(Rectf(m_SCALE * 24, m_SCALE * 88, m_SCALE * 192, m_SCALE * 128), Rectf(195, 0, 192, 128));

	TextureManager::GetTexture("Kirby2")->Draw(Rectf(m_Scale * 70, m_Scale * 37, m_Scale * 16, m_Scale * 16), Rectf(0, 0, 16, 16));
}
