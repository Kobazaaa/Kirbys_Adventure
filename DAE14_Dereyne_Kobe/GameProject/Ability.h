#pragma once
#include "Texture.h"
#include "Matrix2x3.h"

class Ability final
{
public:
	enum class Type
	{
		None, Beam, Fire, Spark
	};

	// Constructor & Destructor
	explicit Ability(Ability::Type abilityType);
	Ability(const Ability& other) = delete;
	~Ability();
	Ability& operator=(const Ability& rhs) = delete;

	// Behavioral
	void Update(float elapsedSec, const Point2f& position, int direction);
	void Draw() const;

	void Use();

	// Accessors
	Ability::Type GetAbilityType() const;
	bool IsActivated() const;
	const std::vector<Rectf>& GetHitboxes() const;

	// Mutators
	void SetAbility(Ability::Type abilityType);
	void Deactivate();
private:
	// BEAM
	void UseBeam(float elapsedSec, const Point2f& position, int direction);
	void DrawBeam() const;

	const int	m_BEAM_SEGMENTS{ 5 };
	const float m_ANGLE_RANGE{ 120.f };
	const float m_BEAM_UPTIME{ 1.f };


	// FIRE
	void UseFire(float elapsedSec, const Point2f& position, int direction);
	void DrawFire() const;

	const int m_FIREBALLS{ 2 };
	const float m_FIRE_RANGE{ 32.f };
	const float m_FIRE_UPTIME{ 2.f };
	const Vector2f m_FIRE_VELOCITY{ 100.f, 1.f };


	// GENERAL
	std::vector<Rectf> m_vHitBoxes{ 5 };

	Matrix2x3 m_TranslationMatrix;
	Matrix2x3 m_RotationMatrix;
	Matrix2x3 m_ScalingMatrix;
	Matrix2x3 m_TransformationMatrix;

	Texture* m_pSpritesheet;
	Point2f m_Position;

	const float m_SPRITE_WIDTH{16};
	const float m_SPRITE_HEIGHT{16};

	Type m_Type;
	bool m_IsActive;
	float m_AccumSec;

	// DRAWING
	Rectf m_DstRect;
	Rectf m_SrcRect;

};

