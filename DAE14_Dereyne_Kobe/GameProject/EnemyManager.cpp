#include "pch.h"
#include "EnemyManager.h"
#include "Kirby.h"
#include "SVGParser.h"
#include "Camera.h"
#include <iostream>
#include <fstream>

EnemyManager::EnemyManager(Camera* camera)
	: m_pCamera{camera}
{
	//std::ofstream file{ "spawn.txt" };
	//if (file)
	//{
	//	for (int index{}; index < m_vEnemies.size(); ++index)
	//	{
	//		std::string name;
	//		if (typeid(*m_vEnemies[index]) == typeid(WaddleDee)) name = "WaddleDee";
	//		if (typeid(*m_vEnemies[index]) == typeid(WaddleDoo)) name = "WaddleDoo";
	//		if (typeid(*m_vEnemies[index]) == typeid(BrontoBurt)) name = "BrontoBurt";
	//		if (typeid(*m_vEnemies[index]) == typeid(Sparky)) name = "Sparky";
	//		if (typeid(*m_vEnemies[index]) == typeid(HotHead)) name = "HotHead";
	//		if (typeid(*m_vEnemies[index]) == typeid(PoppyBrosJr)) name = "PoppyBrosJr";
	//
	//
	//		file << "<" << name << ">\n";
	//		file << "\tspawnpoint=\"" << std::to_string(m_vEnemies[index]->GetSpawnPoint().x) << "," << std::to_string(m_vEnemies[index]->GetSpawnPoint().y) << "\"\n";
	//
	//		if (name == "BrontoBurt")
	//		{
	//			std::string tactic;
	//			BrontoBurt::Tactic enumTactic = dynamic_cast<BrontoBurt*>(m_vEnemies[index])->GetTactic();
	//			switch (enumTactic)
	//			{
	//			case BrontoBurt::Tactic::Wave:
	//				tactic = "wave";
	//				break;
	//			case BrontoBurt::Tactic::AscendingWave:
	//				tactic = "ascendingWave";
	//				break;
	//			case BrontoBurt::Tactic::Ascend:
	//				tactic = "ascend";
	//				break;
	//			case BrontoBurt::Tactic::Straight:
	//				tactic = "straight";
	//				break;
	//			default:
	//				break;
	//			}
	//
	//			file << "\ttactic=\"" << tactic << "\"\n";
	//		}
	//
	//		file << "</" << name << ">\n\n";
	//	}
	//}

}

EnemyManager::~EnemyManager()
{
	for (Enemy*& enemyPtr : m_vEnemies)
	{
		delete enemyPtr;
		enemyPtr = nullptr;
	}
}

void EnemyManager::Draw() const
{
	for (Enemy* enemyPtr : m_vEnemies)
	{
		enemyPtr->Draw();

		if (utils::DEBUG_MODE)
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

void EnemyManager::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos)
{
	for (Enemy* enemyPtr : m_vEnemies)
	{
		enemyPtr->Update(elapsedSec, world, kirbyPos);
	}
	ApplyPlaySpace();
}

#pragma region Managing
void EnemyManager::Add(const std::string& filePath, Point2f center, int score)
{
	m_vEnemies.push_back(new Enemy(filePath, center, score));
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
	enemyPtr->Reset();
}
#pragma endregion

#pragma region Update
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

				float xDist{ enemyPtr->GetPosition().x - pKirby->GetPosition().x };
				float yDist{ (enemyPtr->GetPosition().y - (pKirby->GetInhaleRect().bottom + (pKirby->GetInhaleRect().height / 2))) };

				float xSpeed{ -utils::GetSign(xDist) * 20 * powf(0.9f, (abs(xDist) - 50)) };
				float ySpeed{ -utils::GetSign(yDist) * 20 * powf(0.9f, (abs(xDist) - 50)) };

				float xInc{ xSpeed * elapsedSec };
				float yInc{ ySpeed * elapsedSec };

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
					pKirby->InhaledEnemy(enemyPtr->GetAbilityType());
					pKirby->AddToScore(enemyPtr->GetScore());
				}
			}

		}
	}
	return isEnemyInRect;
}

std::vector<Enemy*>& EnemyManager::GetAllEnemies()
{
	return m_vEnemies;
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
		if (utils::IsRectInRect(enemyPtr->GetSpawnRect(), m_pCamera->GetCameraView()) and enemyPtr->IsEliminated())
		{
			float test1{abs(enemyPtr->GetPosition().x - enemyPtr->GetSpawnPoint().x)};
			float test2{abs(enemyPtr->GetPosition().y - enemyPtr->GetSpawnPoint().y)};

			if (abs(enemyPtr->GetPosition().x - enemyPtr->GetSpawnPoint().x) <= 0.001f  and
				abs(enemyPtr->GetPosition().y - enemyPtr->GetSpawnPoint().y) <= 0.001f )
			{
				enemyPtr->IsEliminated(false);
				enemyPtr->IsActivated(true);
			}
		}
	}
}
#pragma endregion