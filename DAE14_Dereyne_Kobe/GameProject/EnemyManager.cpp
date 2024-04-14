#include "pch.h"
#include "EnemyManager.h"
#include "Kirby.h"
#include "SVGParser.h"
#include "Camera.h"
#include <iostream>

EnemyManager::EnemyManager(Camera* camera)
	: m_pCamera{camera}
{
	std::vector<std::vector<Point2f>> m_SpawnPoints;

	SVGParser::GetVerticesFromSvgFile("Enemies/SpawnPoints.svg", m_SpawnPoints);

	for (size_t i = 0; i < 25; i++)
	{
		if ( i == 1 or i == 6 or i == 9 or i == 12 or i == 17 or i == 18 or i == 22 or i == 23  ) m_vEnemies.push_back(new WaddleDee(m_SpawnPoints[0][i]));
		if ( i == 0 or i == 3 or i == 19														) m_vEnemies.push_back(new WaddleDoo(m_SpawnPoints[0][i]));
		if ( i == 2 or i == 4 or i == 7			 or i == 14 or i == 20 or i == 24				) m_vEnemies.push_back(new BrontoBurt(m_SpawnPoints[0][i]));
		if ( i == 11																			) m_vEnemies.push_back(new BrontoBurt(m_SpawnPoints[0][i], true));
		if ( i == 5 or i == 10 or i == 15														) m_vEnemies.push_back(new Sparky(m_SpawnPoints[0][i]));
		if ( i == 8 or i == 13 or i == 16 or i == 21											) m_vEnemies.push_back(new HotHead(m_SpawnPoints[0][i]));
	}
}

EnemyManager::~EnemyManager()
{
	for (Enemy*& enemyPtr : m_vEnemies)
	{
		delete enemyPtr;
		enemyPtr = nullptr;
	}

	//for (size_t i = 0; i < m_vEnemies.size(); i++)
	//{
	//	if (m_vEnemies[i] == nullptr) std::cout << "DELETED ENEMY NR " << i << std::endl;
	//}
	
}

void EnemyManager::Draw(bool debugMode) const
{
	for (Enemy* enemyPtr : m_vEnemies)
	{
		enemyPtr->Draw();

		if (debugMode)
		{
			utils::SetColor(Color4f(1, 1, 1, 1));
			utils::DrawRect(enemyPtr->GetHitBox());
			utils::FillEllipse(enemyPtr->GetPosition(), 2, 2);
			utils::SetColor(Color4f(0, 1, 1, 1));
			utils::DrawRect(enemyPtr->GetSpawnRect());
			utils::FillEllipse(enemyPtr->GetSpawnPoint(), 2, 2);
		}
	}
}

void EnemyManager::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	for (Enemy* enemyPtr : m_vEnemies)
	{
		if (!enemyPtr->IsEliminated()) enemyPtr->Update(elapsedSec, world);
	}
	ApplyPlaySpace();
}

#pragma region Managing
void EnemyManager::Add(const std::string& filePath, Point2f center)
{
	m_vEnemies.push_back(new Enemy(filePath, center));
}
void EnemyManager::Add(Enemy* enemyPtr)
{
	m_vEnemies.push_back(enemyPtr);
}
void EnemyManager::Pop()
{
	delete m_vEnemies[m_vEnemies.size() - 1];
	m_vEnemies[m_vEnemies.size() - 1] = nullptr;
	m_vEnemies.pop_back();
}
void EnemyManager::Eliminate(Enemy* enemyPtr)
{
	enemyPtr->IsEliminated(true);
	if (enemyPtr->GetDirection() == Entity::Direction::Right) enemyPtr->InverseDirection();
}
#pragma endregion

#pragma region Update
bool EnemyManager::KirbyHitDetection(Kirby* pKirby)
{
	for (Enemy* enemyPtr : m_vEnemies)
	{
		if (!enemyPtr->IsEliminated())
		{
			if (Collision::EntityCollision(enemyPtr, pKirby))
			{
				Eliminate(enemyPtr);

				if (enemyPtr->IsActivated()) return true;
				else return false;
			}
		}
	}
	return false;
}

bool EnemyManager::KirbyInhaleCollision(Kirby* pKirby, float elapsedSec)
{
	bool isEnemyInRect{ false };
	for (Enemy* enemyPtr : m_vEnemies)
	{
		if (!enemyPtr->IsEliminated())
		{
			if (pKirby->GetCurrentState() == Kirby::State::Inhaling and
				utils::IsPointInRect(enemyPtr->GetPosition(), pKirby->GetInhaleRect()))
			{
				isEnemyInRect = true;
				enemyPtr->IsActivated(false);

				float xInc{ -elapsedSec * 10.f *  (enemyPtr->GetPosition().x - pKirby->GetPosition().x) };
				float yInc{ -elapsedSec * 10.f * ((enemyPtr->GetPosition().y - (pKirby->GetInhaleRect().bottom + (pKirby->GetInhaleRect().height / 2)))) };

				Point2f newEnemyPos
				{
					enemyPtr->GetPosition().x + xInc,
					enemyPtr->GetPosition().y + yInc
				};

				enemyPtr->SetPosition(newEnemyPos);
			}

			if (!enemyPtr->IsActivated() and Collision::EntityCollision(enemyPtr, pKirby))
			{
				Eliminate(enemyPtr);
				if (!IsEnemyInhaleRect(pKirby))
				{
					pKirby->InhaledEnemy(enemyPtr);
				}
			}

		}
	}
	return isEnemyInRect;
}

bool EnemyManager::IsEnemyInhaleRect(Kirby* pKirby) const
{
	for (Enemy* enemyPtr : m_vEnemies)
	{
		if (!enemyPtr->IsEliminated())
		{
			if (utils::IsPointInRect(enemyPtr->GetPosition(), pKirby->GetInhaleRect()))
			{
				return true;
			}
		}
	}
	return false;
}

void EnemyManager::ApplyPlaySpace()
{
	for (Enemy* enemyPtr : m_vEnemies)
	{
		// If Enemy leaves Camera View, it dies
		if (!utils::IsOverlapping(enemyPtr->GetHitBox(), m_pCamera->GetCameraView()))
		{
			if (!enemyPtr->IsEliminated())
			{
				Eliminate(enemyPtr);
			}
		}

		// Reset the Enemy
		if (!utils::IsOverlapping(enemyPtr->GetSpawnRect(), m_pCamera->GetCameraView()))
		{
			if (enemyPtr->IsEliminated())
			{
				enemyPtr->SetPosition(enemyPtr->GetSpawnPoint());
			}
		}

		// If the Enemy has been reset, revive it if it's in Camera View
		if (utils::IsRectInRect(enemyPtr->GetSpawnRect(), m_pCamera->GetCameraView()))
		{
			if (enemyPtr->GetPosition().x == enemyPtr->GetSpawnPoint().x and
				enemyPtr->GetPosition().y == enemyPtr->GetSpawnPoint().y)
			{
				enemyPtr->IsEliminated(false);
				enemyPtr->IsActivated(true);
			}
		}
	}
}
#pragma endregion