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
	// Sounds
	SoundManager::LoadSoundEffect("effect1", "Sound/SoundEffects/0C.wav");
	SoundManager::LoadSoundEffect("effect2", "Sound/SoundEffects/0C.wav");

	SoundManager::LoadSoundStream("stream1", "Sound/Music/Vegetable_Valley_Hub.mp3");
	SoundManager::LoadSoundStream("stream2", "Sound/Music/Vegetable_Valley_Hub.mp3");

	// Textures
	LoadTextures();

	// Camera + Level + HUD
	m_pLevel =	new Level("Levels/VegetableValley.png", 3);
	m_pCamera = new Camera(GetViewPort().width, GetViewPort().height, m_SCALE);
	
	// EnemyManager
	m_pEnemyMngr = new EnemyManager(m_pCamera);
	
	// Kirby
	m_pKirby = new Kirby(Point2f(100, 100), m_pLevel);
	m_pHUD = new HUD(m_pKirby, m_SCALE);
	
	// World
	SVGParser::GetVerticesFromSvgFile("Levels/VegetableValley.svg", m_World);
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

	SoundManager::DeleteAllSound();
	TextureManager::DeleteTextures();
}

void Game::Update( float elapsedSec )
{
	if (m_pKirby->DoDoorChecks())
	{
		m_pCamera->SetPosition(Point2f(0, m_pLevel->GetCurrentSubLevel() * m_pLevel->GetSubLevelHeight()));
	}
	
	m_pEnemyMngr->KirbyInhaleCollision(m_pKirby, elapsedSec);
	if (m_pEnemyMngr->EnemyHitKirbyDetection(m_pKirby))
	{
		m_pKirby->HitEnemy();
		m_pCamera->Shake(0.1f, 0.1f);
	}
	if (m_pEnemyMngr->EnemyKirbyProjectileCollision(m_pKirby))
	{
		m_pKirby->HitEnemy();
		m_pCamera->Shake(0.1f, 0.1f);
	}
	
	m_pKirby->Update(elapsedSec, m_World);
	m_pEnemyMngr->Update(elapsedSec, m_World, m_pKirby->GetPosition());
	m_pHUD->Update(elapsedSec);
	m_pCamera->Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground();

	m_pCamera->Aim(m_pLevel->GetWidth(), m_pLevel->GetSubLevelHeight(), m_pLevel->GetCurrentSubLevel() * m_pLevel->GetSubLevelHeight(), m_pKirby->GetPosition(), m_pHUD->GetHeight());
	{
		m_pLevel->Draw();
		utils::SetColor(Color4f(1, 1, 1, 1));
	
		m_pKirby->Draw();
		m_pEnemyMngr->Draw(m_DEBUG_MODE);
	
	
	
		if (m_DEBUG_MODE)
		{
			for (size_t idx{}; idx < m_World.size(); idx++)
			{
				if (idx == 1) utils::SetColor(Color4f(1, 0, 0, 1));
				else utils::SetColor(Color4f(1, 1, 1, 1));
				utils::DrawPolygon(m_World[idx]);
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

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	if (m_DEBUG_MODE)
	{
		if (e.keysym.sym == SDLK_i)
		{
			std::cout << *m_pKirby;
		}
		if (e.keysym.sym == SDLK_s)
		{
			m_pCamera->Shake(0.1f, 0.1f);
		}
		
		if (e.keysym.sym == SDLK_1)
		{
			SoundManager::PlayStream("stream1");
		}
		if (e.keysym.sym == SDLK_2)
		{
			SoundManager::PlayStream("stream2");
		}
		if (e.keysym.sym == SDLK_3)
		{
			SoundManager::PlayEffect("effect1");
		}
		if (e.keysym.sym == SDLK_4)
		{
			SoundManager::PlayEffect("effect2");
		}
		if (e.keysym.sym == SDLK_5)
		{
			SoundManager::PauseAll();
		}
		if (e.keysym.sym == SDLK_6)
		{
			SoundManager::ResumeAll();
		}
		if (e.keysym.sym == SDLK_7)
		{
			SoundManager::StopAll();
		}
	}
	if (e.keysym.sym == SDLK_r)
	{
		m_pKirby->Reset();
	}	
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
	if (m_DEBUG_MODE)
	{
		Point2f clickPos{ (float(e.x) / m_SCALE + m_pCamera->GetCameraView().left) , float(e.y) / m_SCALE - 64 };
		clickPos.y += m_pLevel->GetCurrentSubLevel() * m_pLevel->GetSubLevelHeight();
	
		if (e.button == SDL_BUTTON_RIGHT)
		{
			m_pEnemyMngr->Add(new Sparky(clickPos));
			clickPos.y += m_pLevel->GetCurrentSubLevel() * m_pLevel->GetSubLevelHeight();
		}
		if (e.button == SDL_BUTTON_LEFT)
		{
			m_pKirby->SetPosition(clickPos);
		}
		//if (e.button == SDL_BUTTON_MIDDLE)
		//{
		//	m_pEnemyMngr->Add(new BrontoBurt(clickPos));
		//	clickPos.y += m_pLevel->GetCurrentSubLevel() * m_pLevel->GetSubLevelHeight();
		//}	
		if (e.button == SDL_BUTTON_MIDDLE)
		{
			m_pEnemyMngr->Add(new HotHead(clickPos, true));
			clickPos.y += m_pLevel->GetCurrentSubLevel() * m_pLevel->GetSubLevelHeight();
		}
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

void Game::LoadTextures()
{
	TextureManager::LoadTexture("Puff",					"Abilities/Puff.png");
	TextureManager::LoadTexture("StarProjectile",		"Abilities/Star.png");
	TextureManager::LoadTexture("Beam",					"Abilities/Beam.png");
	TextureManager::LoadTexture("Fire",					"Abilities/Fire.png");
	TextureManager::LoadTexture("Spark",				"Abilities/Spark.png");
	TextureManager::LoadTexture("BrontoBurt",			"Enemies/BrontoBurt.png");
	TextureManager::LoadTexture("HotHead",				"Enemies/HotHead.png");
	TextureManager::LoadTexture("Sparky",				"Enemies/Sparky.png");
	TextureManager::LoadTexture("WaddleDee",			"Enemies/WaddleDee.png");
	TextureManager::LoadTexture("WaddleDoo",			"Enemies/WaddleDoo.png");
	TextureManager::LoadTexture("HUD",					"HUD/HUD.png");
	TextureManager::LoadTexture("Kirby",				"Kirby/Kirby.png");
	TextureManager::LoadTexture("VegetableValley",		"Levels/VegetableValley.png");
}
