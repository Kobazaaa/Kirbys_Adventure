#include "pch.h"
#include "Camera.h"
#include "Kirby.h"
#include "ParticleSystem.h"
#include "LevelManager.h"
#include "HUD.h"
#include "Gameplay.h"

Gameplay::Gameplay(float scale, Rectf viewport)
	: BaseState(viewport)
	, m_Scale{scale}
{
	// Camera
	m_pCamera = new Camera(m_VIEWPORT.width, m_VIEWPORT.height, m_Scale);

	// Kirby
	m_pKirby = new Kirby(Point2f(100, 100));

	// LevelManager
	m_VegetableValleyManager = new LevelManager("Levels/World.xml", m_pKirby, m_pCamera);

	//HUD
	m_pHUD = new HUD(m_pKirby, m_Scale);

}

Gameplay::~Gameplay()
{
	delete m_pKirby;
	m_pKirby = nullptr;
	delete m_pCamera;
	m_pCamera = nullptr;
	delete m_VegetableValleyManager;
	m_VegetableValleyManager = nullptr;
	delete m_pHUD;
	m_pHUD = nullptr;
}

void Gameplay::Enter()
{

}

void Gameplay::Exit()
{
}

void Gameplay::Update(float elapsedSec, bool freeze)
{
	if (!freeze)
	{
		ParticleSystem::Update(elapsedSec);
		m_VegetableValleyManager->GetCurrentLevel()->GetEnemyMngr()->KirbyInhaleCollision(m_pKirby, elapsedSec);
		if (Collision::KirbyHitDetection(m_pKirby, m_VegetableValleyManager->GetCurrentLevel()->GetEnemyMngr()->GetAllEnemies(), Projectile::GetAllProjectiles()))
		{
			m_pCamera->Shake(0.1f, 0.1f);
		}

		m_pKirby->Update(elapsedSec, m_VegetableValleyManager->GetCurrentLevel()->GetWorld());
		m_pHUD->Update(elapsedSec);
	}
	m_VegetableValleyManager->Update(elapsedSec);
	m_pCamera->Update(elapsedSec);


}

void Gameplay::Draw() const
{
	m_pCamera->Aim(m_VegetableValleyManager->GetCurrentLevel()->GetWidth(), m_VegetableValleyManager->GetCurrentLevel()->GetSubLevelHeight(), m_VegetableValleyManager->GetCurrentLevel()->GetCurrentSubLevel() * m_VegetableValleyManager->GetCurrentLevel()->GetSubLevelHeight(), m_pKirby->GetPosition(), m_pHUD->GetHeight());
	{
		m_VegetableValleyManager->Draw();
		utils::SetColor(Color4f(1, 1, 1, 1));

		ParticleSystem::Draw();
		m_pKirby->Draw();


		if (utils::DEBUG_MODE)
		{
			for (size_t idx{}; idx < m_VegetableValleyManager->GetCurrentLevel()->GetWorld().size(); idx++)
			{
				if (idx == 1) utils::SetColor(Color4f(1, 0, 0, 1));
				else utils::SetColor(Color4f(1, 1, 1, 1));
				utils::DrawPolygon(m_VegetableValleyManager->GetCurrentLevel()->GetWorld()[idx]);
			}
			utils::SetColor(Color4f(1, 1, 1, 1));
			utils::FillEllipse(m_pKirby->GetPosition(), 2, 2);
			utils::DrawRect(m_pKirby->GetHitBox());
			utils::DrawRect(m_pKirby->GetInhaleRect());

		}

	}
	m_pCamera->Reset();
	m_pHUD->Draw();

}
