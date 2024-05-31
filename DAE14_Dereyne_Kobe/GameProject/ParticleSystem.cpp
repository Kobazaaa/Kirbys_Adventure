#include "pch.h"
#include "ParticleSystem.h"

std::vector<Particle> ParticleSystem::m_vParticles;
Texture ParticleSystem::m_Texture;

bool ParticleSystem::InitializeParticleSystem()
{
	m_Texture = Texture("Particles/Particle.png");
	return m_Texture.IsCreationOk();
}

void ParticleSystem::Update(float elapsedSec)
{
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
		m_vParticles[index].m_Animation.Draw(&m_Texture, m_vParticles[index].m_Position);
	}
}

void ParticleSystem::AddParticle(const std::string& particleName, const Point2f& pos)
{
	if (particleName == "Inhale")
	{
		std::vector<Frame> frame{
			Frame(Rectf(0, 0, 16, 16), 0.1f)
		};
		Animation anim{ std::move(frame), true };
		AddParticle(pos, Vector2f(-10, -10), 1.f, anim);
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

	// Particle 1
	AddParticle(particle);
}
void ParticleSystem::AddAirBubbles(const Point2f& pos)
{
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

	// Particle 1
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
