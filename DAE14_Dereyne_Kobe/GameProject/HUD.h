#pragma once
#include "Texture.h"
#include "Kirby.h"

class HUD final
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit HUD(Kirby* kirby, float scale = 1);


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void Draw() const;
	void Update(float elapsedSec);


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			ACCESSORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float GetHeight() const;
private:
	// Private Draw Functions
	void PrintScore()				const;
	void DrawHealth()				const;
	void DrawLives()				const;
	void DrawLivesAnim()			const;
	void DrawCard()					const;

	// Private Variables
	Texture*  m_pSprites;
	Kirby*	  m_pKIRBY;

	float	 m_AccumSec;
	float	 m_Scale;
	int		 m_CurrentFrame;

	// Dimensions for every HUD element
	const float		m_HUD_WIDTH				{ 248.f };
	const float		m_HUD_HEIGHT			{  64.f };
	const Point2f	m_HUD_POS				{ 0.f, 0.f };

	const float		m_CARD_WIDTH			{  32.f };
	const float		m_CARD_HEIGHT			{  40.f };
	const Point2f	m_CARD_POS				{ 144.f, 15.f };

	const float		m_LEVEL_NAME_WIDTH		{ 169.f };
	const float		m_LEVEL_NAME_HEIGHT		{  10.f };
	const Point2f	m_LEVEL_NAME_POS		{ 64.f, 39.f };

	const float		m_KIRBY_HEALTH_WIDTH	{   8.f };
	const float		m_KIRBY_HEALTH_HEIGHT	{  14.f };
	const Point2f	m_KIRBY_HEALTH_POS		{ 72.f, 36.f };

	const float		m_KIRBY_ANIM_WIDTH		{  13.f };
	const float		m_KIRBY_ANIM_HEIGHT		{  12.f };
	const Point2f	m_KIRBY_ANIM_POS		{ 187.f, 31.f};
	const Point2f	m_KIRBY_LIVES_POS		{ 208.f, 31.f};

	const float		m_NUMBER_WIDTH			{   8.f };
	const float		m_NUMBER_HEIGHT			{   8.f };

	const Point2f	m_SCORE_POS				{ 72.f, 23.f};
};

