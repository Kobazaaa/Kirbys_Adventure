#include "pch.h"
#include "utils.h"
#include "TextureManager.h"
#include "ParticleSystem.h"

std::vector<Particle> ParticleSystem::m_vParticles;
Texture* ParticleSystem::m_pTexture;
float ParticleSystem::m_AccumSec{ 0 };

bool ParticleSystem::InitializeParticleSystem()
{
	m_pTexture = TextureManager::GetTexture("Particles");
	return m_pTexture->IsCreationOk();
}

void ParticleSystem::Update(float elapsedSec)
{
	m_AccumSec += elapsedSec;
	for (int index{}; index < m_vParticles.size(); ++index)
	{
		m_vParticles[index].m_AccumSec += elapsedSec;
		if (m_vParticles[index].m_AccumSec <= m_vParticles[index].m_LifeTime)
		{
			m_vParticles[index].m_Position.x += m_vParticles[index].m_Velocity.x * elapsedSec;
			m_vParticles[index].m_Position.y += m_vParticles[index].m_Velocity.y * elapsedSec;
			
			m_vParticles[index].m_Animation.Activate();
			m_vParticles[index].m_Animation.Update(elapsedSec);
		}
		else
		{
			m_vParticles[index].m_Animation.Deactivate();
			m_vParticles[index] = m_vParticles.back();
			m_vParticles.pop_back();
		}
	}
}
void ParticleSystem::Draw()
{
	for (int index{}; index < m_vParticles.size(); ++index)
	{
		m_vParticles[index].m_Animation.Draw(m_pTexture, m_vParticles[index].m_Position);
	}
}

void ParticleSystem::AddParticle(const Particle& particle)
{
	m_vParticles.push_back(particle);
}
void ParticleSystem::AddParticle(const Point2f& pos, const Vector2f& vel, float lifetime, const Animation& animation)
{
	m_vParticles.push_back(Particle(pos, vel, lifetime, animation));
}

void ParticleSystem::AddEnemyDeathParticles(const Point2f& pos, Direction direction)
{
	int randomInt = rand() % 3;

	if (randomInt == 0) AddArc(pos, direction);
	if (randomInt == 1) AddCircle(pos);
	if (randomInt == 2) AddCross(pos, rand() % 2 == 0 ? true : false);
}
void ParticleSystem::AddKirbyDeathParticles(const Point2f& pos)
{
	std::vector<Frame> frame{
		Frame(Rectf(0, -48, 16, 16), 0.1f),
		Frame(Rectf(16, -48, 16, 16), 0.1f),
		Frame(Rectf(32, -48, 16, 16), 0.1f),
		Frame(Rectf(48, -48, 16, 16), 0.1f)
	};
	Animation anim{ std::move(frame), true };
	Particle particle
	{
		pos, Vector2f(0, 0), 3.0f, anim
	};


	const float SPEED{ 40.f };
	for (int index{}; index < 8; ++index)
	{
		const float ANGLE{ float(M_PI) / 4.f * index };
		particle.m_Velocity.x = SPEED * cosf(ANGLE);
		particle.m_Velocity.y = SPEED * sinf(ANGLE);
		AddParticle(particle);
	}

}
void ParticleSystem::AddRunParticles(const Point2f& pos, const Vector2f& vel)
{
	std::vector<Frame> frame{
		Frame(Rectf(48, 0, 16, 16), 0.15f),
		Frame(Rectf(48, -16, 16, 16), 0.05f),
	};
	Animation anim{ std::move(frame), false };
	Particle particle
	{
		pos, vel, 0.2f, anim
	};

	AddParticle(particle);
}
void ParticleSystem::AddAirBubbles(const Point2f& pos)
{
	std::vector<Frame> frame{
		Frame(Rectf(48, -32, 16, 16), 1.0f),
	};
	Animation anim{ std::move(frame), false };
	Particle particle
	{
		pos, Vector2f(0, 20), 1.0f, anim
	};

	AddParticle(particle);
}
void ParticleSystem::AddLandParticles(const Point2f& pos)
{
	std::vector<Frame> frame{
		Frame(Rectf(16, 0, 16, 16), 0.15f),
	};
	Animation anim{ std::move(frame), false };

	float speed{ 100 };
	float angle{ rand() % 8 * float(M_PI) / 4.f};
	Vector2f vel{};
	vel.x = cosf(angle) * speed;
	vel.y = sinf(angle) * speed;

	Particle particle
	{
		pos, vel, 0.20f, anim
	};

	AddParticle(particle);
}
void ParticleSystem::AddInhaleParticles(const Point2f& pos, const Rectf& inhaleRect)
{
	if (m_AccumSec > 0.03f)
	{
		m_AccumSec = 0;

		std::vector<Frame> frame {
			Frame(Rectf(0, 0, 16, 16), 0.15f),
		};
		Animation anim{ std::move(frame), false };

		const float SPEED{2};
		Point2f partPos{ utils::GetRandomPosInRect(inhaleRect) };
		Particle particle
		{
			partPos, Vector2f(SPEED * (pos.x - partPos.x), SPEED * (pos.y - partPos.y)), 0.1f, anim
		};

		AddParticle(particle);
	}
}
void ParticleSystem::AddAbilityActivation(const Point2f& pos)
{
	if (m_AccumSec >= 0.05f)
	{
		m_AccumSec = 0;

		std::vector<Frame> frame{
			Frame(Rectf(0, -16, 16, 16), 0.03f),
			Frame(Rectf(16, -16, 16, 16), 0.03f),
			Frame(Rectf(32, -16, 16, 16), 0.03f),
		};
		Animation anim{ std::move(frame), false };
		const Point2f position
		{
			pos.x + utils::GetRandomFloat(-32, 32, 2),
			pos.y + utils::GetRandomFloat(-32, 32, 2)
		};

		Particle particle
		{
			position, Vector2f(0, 0), 0.1f, anim
		};

		AddParticle(particle);
	}
}
void ParticleSystem::AddImpactParticles(const Point2f& pos)
{
	std::vector<Frame> frame{
		Frame(Rectf(0, -32, 16, 16), 0.05f),
		Frame(Rectf(16, -32, 16, 16), 0.05f),
		Frame(Rectf(32, -32, 16, 16), 0.1f),
	};
	Animation anim{ std::move(frame), false };
	Particle particle
	{
		pos, Vector2f(0, 0), 0.2f, anim
	};

	AddParticle(particle);
}

void ParticleSystem::AddArc(const Point2f& pos, Direction direction)
{
	// Default Particle
	std::vector<Frame> frame{
		Frame(Rectf(16, 0, 16, 16), 0.1f),
	};
	Animation anim{ std::move(frame), true };
	float speed{ 150 };
	Particle particle
	{
		pos, Vector2f(speed, 0), 0.15f, anim
	};

	// Particle 1
	particle.m_Velocity.x = static_cast<int>(direction) * speed / 2;
	particle.m_Velocity.y = speed;
	AddParticle(particle);
	// Particle 2
	particle.m_Velocity.x = static_cast<int>(direction) * speed;
	particle.m_Velocity.y = speed / 2;
	AddParticle(particle);
	// Particle 3
	particle.m_Velocity.x = static_cast<int>(direction) * speed / 2;
	particle.m_Velocity.y = -speed;
	AddParticle(particle);
	// Particle 4
	particle.m_Velocity.x = static_cast<int>(direction) * speed;
	particle.m_Velocity.y = -speed / 2;
	AddParticle(particle);
}
void ParticleSystem::AddCross(const Point2f& pos, bool diagonal)
{
	// Default Particle
	std::vector<Frame> frame{
		Frame(Rectf(16, 0, 16, 16), 0.1f),
	};
	Animation anim{ std::move(frame), true };
	float speed{ 150 };
	Particle particle
	{
		pos, Vector2f(speed, 0), 0.15f, anim
	};

	// Particle 1
	particle.m_Velocity.x = diagonal * speed;
	particle.m_Velocity.y = speed;
	AddParticle(particle);
	// Particle 2
	particle.m_Velocity.x = -diagonal * speed;
	particle.m_Velocity.y = -speed;
	AddParticle(particle);
	// Particle 3
	particle.m_Velocity.x = speed;
	particle.m_Velocity.y = -diagonal * speed;
	AddParticle(particle);
	// Particle 4
	particle.m_Velocity.x = -speed;
	particle.m_Velocity.y = diagonal * speed;
	AddParticle(particle);

}
void ParticleSystem::AddStar(const Point2f& pos)
{
	// Default Particle
	std::vector<Frame> frame{
		Frame(Rectf(0, 48, 16, 16), 0.1f),
		Frame(Rectf(16, 48, 16, 16), 0.1f),
		Frame(Rectf(32, 48, 16, 16), 0.1f),
		Frame(Rectf(48, 48, 16, 16), 0.1f),
	};
	Animation anim{ std::move(frame), true };
	float speed{ 150 };
	Particle particle
	{
		pos, Vector2f(speed, 0), 0.15f, anim
	};

	// Particle 1
	particle.m_Velocity.x = speed;
	particle.m_Velocity.y = speed;
	AddParticle(particle);
	// Particle 2
	particle.m_Velocity.x = -speed;
	particle.m_Velocity.y = -speed;
	AddParticle(particle);
	// Particle 3
	particle.m_Velocity.x = 0;
	particle.m_Velocity.y = 0;
	// Particle 4
	AddParticle(particle);
	// Particle 5
	AddParticle(particle);
	// Particle 6
	AddParticle(particle);
	// Particle 7
	AddParticle(particle);


}
void ParticleSystem::AddCircle(const Point2f& pos)
{
	// Default Particle
	std::vector<Frame> frame{
	Frame(Rectf(16, 0, 16, 16), 0.1f)
	};
	Animation anim{ std::move(frame), true };
	float speed{ 150 };
	Particle particle
	{
		pos, Vector2f(speed, 0), 0.15f, anim
	};

	// Particle 1
	particle.m_Velocity.x = 0;
	particle.m_Velocity.y = speed;
	AddParticle(particle);
	// Particle 2
	particle.m_Velocity.x = 0;
	particle.m_Velocity.y = -speed;
	AddParticle(particle);
	// Particle 3
	particle.m_Velocity.x = speed;
	particle.m_Velocity.y = 0;
	AddParticle(particle);
	// Particle 4
	particle.m_Velocity.x = -speed;
	particle.m_Velocity.y = 0;
	AddParticle(particle);

	// Particle 5
	particle.m_Velocity.x = -speed * sqrtf(2) / 2;
	particle.m_Velocity.y = speed * sqrtf(2) / 2;
	AddParticle(particle);
	// Particle 6
	particle.m_Velocity.x = -speed * sqrtf(2) / 2;
	particle.m_Velocity.y = -speed * sqrtf(2) / 2;
	AddParticle(particle);
	// Particle 7
	particle.m_Velocity.x = speed * sqrtf(2) / 2;
	particle.m_Velocity.y = -speed * sqrtf(2) / 2;
	AddParticle(particle);
	// Particle 8
	particle.m_Velocity.x = speed * sqrtf(2) / 2;
	particle.m_Velocity.y = speed * sqrtf(2) / 2;
	AddParticle(particle);

}
