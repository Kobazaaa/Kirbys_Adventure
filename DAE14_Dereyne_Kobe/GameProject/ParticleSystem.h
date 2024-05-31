#pragma once
#include <vector>
#include "AnimationManager.h"
#include "WorldUtils.h"
#include "Texture.h"
#include "Vector2f.h"

struct Particle
{
	explicit Particle(const Point2f& pos, const Vector2f& vel, float lifetime, const Animation& animation)
		: m_Position{ pos }
		, m_Velocity{ vel }
		, m_AccumSec{ 0 }
		, m_LifeTime{ lifetime }
		, m_Animation{ animation }
	{
	}

	Point2f m_Position;
	Vector2f m_Velocity;
	float m_AccumSec;
	float m_LifeTime;
	Animation m_Animation;
};

class ParticleSystem
{
public:
	static bool InitializeParticleSystem();

	static void Update(float elapsedSec);
	static void Draw();

	static void AddParticle(const std::string& particleName, const Point2f& pos);
	static void AddParticle(const Point2f& pos, const Vector2f& vel, float lifetime, const Animation& animation);
	static void AddParticle(const Particle& particle);

	static void AddEnemyDeathParticles(const Point2f& pos, Direction direction);
	static void AddRunParticles(const Point2f& pos, const Vector2f& vel);
	static void AddImpactParticles(const Point2f& pos);
	static void AddAirBubbles(const Point2f& pos);

	static void AddArc(const Point2f& pos, Direction direction);
	static void AddCross(const Point2f& pos, bool diagonal);
	static void AddStar(const Point2f& pos);
	static void AddCircle(const Point2f& pos);


private:

	static std::vector<Particle> m_vParticles;
	static Texture m_Texture;
};

