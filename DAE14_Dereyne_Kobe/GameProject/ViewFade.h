#pragma once

class ViewFade
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void Darken(float intensity = 0.5f);
	static void RemoveDarken();

	static void StartFade(float duration);
	static void StartFadeIn(float duration);
	static void Update(float elapsedSec);
	static void Draw(const Rectf& viewPort);

	static bool IsFading();
	static bool IsFadingIn();
private:
	//Private Variables
	static float m_FadeTimer;
	static float m_FadeDuration;
	static float m_DarkenIntensity;

	static bool m_IsFading;
	static bool m_FadeIn;
	static bool m_FadeOut;
	static bool m_Darkened;
};

