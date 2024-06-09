#include "pch.h"
#include "utils.h"
#include "ViewFade.h"

float ViewFade::m_FadeDuration = 0;
float ViewFade::m_FadeTimer = 0;
float ViewFade::m_DarkenIntensity = 0;

bool ViewFade::m_Darkened = false;
bool ViewFade::m_IsFading = false;
bool ViewFade::m_FadeIn = false;
bool ViewFade::m_FadeOut = false;
bool ViewFade::m_Freeze = true;

void ViewFade::Darken(float intensity)
{
	if (!m_Darkened)
	{
		m_Darkened = true;
		m_DarkenIntensity = intensity;
	}
}

void ViewFade::RemoveDarken()
{
	m_Darkened = false;
}

void ViewFade::StartFade(float duration, bool freeze)
{
	if (!m_IsFading)
	{
		m_Freeze = freeze;
		m_IsFading = true;
		m_FadeOut = true;
		m_FadeIn = false;
		m_FadeTimer = 0;
		m_FadeDuration = duration;
	}
}

void ViewFade::StartFadeIn(float duration)
{
	if (!m_IsFading)
	{
		m_IsFading = true;
		m_FadeOut = false;
		m_FadeIn = true;
		m_FadeDuration = duration;
		m_FadeTimer = 0;
	}
}

void ViewFade::Update(float elapsedSec)
{
	if (m_IsFading)
	{
		m_FadeTimer += elapsedSec;

		if (m_FadeOut and m_FadeTimer >= m_FadeDuration / 2)
		{
			m_FadeTimer = 0;
			m_FadeOut = false;
			m_FadeIn = true;
		}
		else if (m_FadeIn and m_FadeTimer >= m_FadeDuration / 2)
		{
			m_FadeTimer = 0;
			m_FadeIn = false;
			m_IsFading = false;
		}
	}
}

void ViewFade::Draw(const Rectf& viewPort)
{
	if (m_IsFading)
	{
		if (m_FadeOut)
		{
			utils::SetColor(Color4f(0, 0, 0, m_FadeTimer / (m_FadeDuration / 2)));
			utils::FillRect(viewPort);
		}
		if (m_FadeIn)
		{
			utils::SetColor(Color4f(0, 0, 0, ((m_FadeDuration / 2) - m_FadeTimer) / (m_FadeDuration / 2)));
			utils::FillRect(viewPort);
		}
	}
}

void ViewFade::DrawDarken(const Rectf& viewPort)
{
	if (m_Darkened)
	{
		utils::SetColor(Color4f(0.1f, 0.1f, 0.1f, m_DarkenIntensity));
		utils::FillRect(viewPort);
	}
}

bool ViewFade::IsFading()
{
	return m_IsFading;
}

bool ViewFade::IsFadingIn()
{
	return m_FadeIn;
}

bool ViewFade::IsDarkened()
{
	return m_Darkened;
}

bool ViewFade::CauseFreeze()
{
	return m_Freeze;
}
