#include "pch.h"
#include "SVGParser.h"
#include "Matrix2x3.h"
#include "Game.h"
#include "utils.h"
#include "Kirby.h"
#include <iostream>

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	// EnemyManager
	m_pEnemyMngr = new EnemyManager();

	// Camera + Level + HUD
	m_pCamera = new Camera(GetViewPort().width, GetViewPort().height, m_SCALE);
	m_pLevel =	new Level("VegetableValley.png", 3);
	
	// Kirby
	m_pKirby = new Kirby(Point2f(100, 168), m_pLevel);
	m_pHUD = new HUD(m_pKirby, m_SCALE);

	// World
	SVGParser::GetVerticesFromSvgFile("VegetableValleyFull.svg", m_World);
	for (size_t idx{}; idx < m_World.size(); idx++)
	{
		for (size_t i = 0; i < m_World[idx].size(); i++)
		{
			m_World[idx][i].y += (GetViewPort().height / m_SCALE - m_pLevel->GetHeight() / 3);
		}
	}	
}

void Game::Cleanup( )
{
	delete m_pKirby;
	m_pKirby = nullptr;
	delete m_pEnemyMngr;
	m_pEnemyMngr = nullptr;
	delete m_pCamera;
	m_pCamera = nullptr;
	delete m_pLevel;
	m_pLevel = nullptr;
	delete m_pHUD;
	m_pHUD = nullptr;
}

void Game::Update( float elapsedSec )
{
	m_pKirby->Update(elapsedSec, m_World);
	if (m_pKirby->GetCurrentState() == Kirby::State::Inhaling and
		m_pEnemyMngr->KirbyInhaleCollision(m_pKirby, elapsedSec))
	{
		if (m_pEnemyMngr->KirbyHitDetection(m_pKirby))
		{
			m_pKirby->InhaledEnemy();
		}	
	}
	if (m_pEnemyMngr->KirbyHitDetection(m_pKirby))
	{
		m_pKirby->HitEnemy();
	}
	m_pEnemyMngr->Update(elapsedSec, m_World);
	//if (!utils::IsOverlapping(m_pEnemies[idx]->GetDstRect(), m_pCamera->GetCameraView()))
	//{
	//	delete m_pEnemies[idx];
	//	m_pEnemies[idx] = nullptr;
	//}

	//if (utils::IsPointInRect(Point2f(300, 200), m_pCamera->GetCameraView())) m_pEnemies.push_back(new Enemy("WaddleDee.png", Point2f(300, 200)));

	m_pHUD->Update(elapsedSec);

}

void Game::Draw( ) const
{
	ClearBackground();

	m_pCamera->Aim(m_pLevel->GetWidth(), m_pLevel->GetHeight(), m_pKirby->GetPosition());
		m_pLevel->Draw();
		utils::SetColor(Color4f(1, 1, 1, 1));
		for (size_t idx{}; idx < m_World.size(); idx++)
		{
			utils::DrawPolygon(m_World[idx]);
		}
		m_pKirby->Draw();
		m_pEnemyMngr->Draw();



		// DEBUGGING I LOVE IT <3 <3 <3
		utils::SetColor(Color4f(1, 1, 1, 1));
		utils::FillEllipse(m_pKirby->GetPosition(), 2, 2);
		utils::DrawRect(m_pKirby->GetHitBox());
		utils::DrawRect(m_pKirby->GetInhaleRect());

	m_pCamera->Reset();

	m_pHUD->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	Point2f clickPos{ (float(e.x) / m_SCALE + m_pCamera->GetCameraView().left) , float(e.y) / m_SCALE };

	if (e.button == SDL_BUTTON_RIGHT)
	{
		m_pEnemyMngr->Add(new WaddleDee(clickPos));
	}
	if (e.button == SDL_BUTTON_LEFT)
	{
		m_pKirby->SetPosition(clickPos);
	}
	if (e.button == SDL_BUTTON_MIDDLE)
	{
		m_pEnemyMngr->Add(new BrontoBurt(clickPos));
	}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
