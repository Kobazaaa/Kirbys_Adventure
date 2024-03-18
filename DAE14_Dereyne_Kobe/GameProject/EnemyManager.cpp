#include "pch.h"
#include "EnemyManager.h"
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

bool EnemyManager::KirbyCollision(const Rectf& kirbyHitbox)
{
	int index{-1};
	for (Enemy* enemyPtr : m_vEnemies)
	{
		++index;
		if (enemyPtr != nullptr)
		{
			if (utils::IsPointInRect(enemyPtr->GetPosition(), kirbyHitbox))
			{
				Eliminate(index);
				return true;
			}
		}
	}
	return false;
}

bool EnemyManager::KirbyInhaleCollision(const Rectf& inhaleRect)
{
	return true;
}
