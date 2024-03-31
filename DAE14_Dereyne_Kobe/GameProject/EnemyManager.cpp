#include "pch.h"
#include "EnemyManager.h"
#include "Kirby.h"
#include <iostream>

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
	for (Enemy*& enemyPtr : m_vEnemies)
	{
		delete enemyPtr;
		enemyPtr = nullptr;
	}

	for (size_t i = 0; i < m_vEnemies.size(); i++)
	{
		if (m_vEnemies[i] == nullptr) std::cout << "NULLPTR DETECTED " << i << std::endl;
	}
	
}

void EnemyManager::Draw() const
{
	for (Enemy* enemyPtr : m_vEnemies)
	{
		if (enemyPtr != nullptr)
		{
			enemyPtr->Draw();
			utils::DrawRect(enemyPtr->GetHitBox());
		}
	}
}

void EnemyManager::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	for (Enemy* enemyPtr : m_vEnemies)
	{
		if (enemyPtr != nullptr)
		{
			enemyPtr->Update(elapsedSec, world);
			
			if (Collision::WallCollision(enemyPtr, world))
			{
				enemyPtr->InverseDirection();
			}
		}
	}
}

void EnemyManager::Add(const std::string& filePath, Point2f center)
{
	m_vEnemies.push_back(new Enemy(filePath, center));
	std::cout << "ADDED\n";
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

void EnemyManager::Eliminate(int index)
{
	delete m_vEnemies.at(index);
	m_vEnemies.at(index) = nullptr;
}

bool EnemyManager::KirbyHitDetection(Kirby* pKirby)
{
	int index{-1};
	for (Enemy* enemyPtr : m_vEnemies)
	{
		++index;
		if (enemyPtr != nullptr)
		{
			if (Collision::EntityCollision(enemyPtr, pKirby))
			{
				Eliminate(index);
				//if (enemyPtr->IsActivated()) return true;
				return true;
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
		if (enemyPtr != nullptr)
		{
			if (utils::IsPointInRect(enemyPtr->GetPosition(), pKirby->GetInhaleRect()))
			{
				if (enemyPtr->IsActivated()) isEnemyInRect = true;
				enemyPtr->IsActivated(false);

				float xInc{ elapsedSec   * 10.f  * (pKirby->GetPosition().x - enemyPtr->GetPosition().x)};
				float yInc{ - elapsedSec * 10.f  * ((enemyPtr->GetPosition().y - (pKirby->GetInhaleRect().bottom + (pKirby->GetInhaleRect().height / 2)))) };
				enemyPtr->SetPosition(Point2f(enemyPtr->GetPosition().x + xInc, enemyPtr->GetPosition().y + yInc));
			}
		}

	}
	return isEnemyInRect;
}