#pragma once

class ViewFade
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void StartFade(float duration);
	static void Update(float elapsedSec);
	static void Draw(const Rectf& viewPort);

	static bool IsFading();
	static bool IsFadingIn();

private:
	//Private Variables
	static float m_FadeTimer;
	static float m_FadeDuration;

	static bool m_IsFading;
	static bool m_FadeIn;
	static bool m_FadeOut;
};

