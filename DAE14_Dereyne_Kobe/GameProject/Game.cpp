#include "pch.h"
#include "SVGParser.h"
#include "Matrix2x3.h"
#include "Game.h"
#include "utils.h"
#include "ParticleSystem.h"
#include "ViewFade.h"
#include "Kirby.h"
#include "StateMachine.h"
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
	ParticleSystem::InitializeParticleSystem();
	// Sounds
	LoadSounds();
	// Textures
	LoadTextures();

	
	// Camera
	m_pCamera = new Camera(GetViewPort().width, GetViewPort().height, m_SCALE);

	// Kirby
	m_pKirby = new Kirby(Point2f(100, 100));

	// LevelManager
	m_VegetableValleyManager = new LevelManager("Levels/World.xml", m_pKirby, m_pCamera);

	//HUD
	m_pHUD = new HUD(m_pKirby, m_SCALE);
}

void Game::Cleanup( )
{
	delete m_pKirby;
	m_pKirby = nullptr;
	delete m_pCamera;
	m_pCamera = nullptr;
	delete m_VegetableValleyManager;
	m_VegetableValleyManager = nullptr;
	delete m_pHUD;
	m_pHUD = nullptr;

	SoundManager::DeleteAllSound();
	TextureManager::DeleteTextures();
}

void Game::Update( float elapsedSec )
{
	if (ViewFade::IsFading())
	{
	//	StateMachine::SetState(StateMachine::State::Freeze);
	}
	else if (StateMachine::GetState() != StateMachine::State::Pause)
	{
		StateMachine::SetState(StateMachine::State::Gameplay);
	}
// TODO fix UwU
	if (utils::KeyPress(SDL_SCANCODE_ESCAPE))
	{
		ViewFade::StartFade(1.f);
	}
	else if (ViewFade::IsFadingIn())
	{
		if (StateMachine::GetState() == StateMachine::State::Gameplay) StateMachine::SetState(StateMachine::State::Pause);
		else if (StateMachine::GetState() == StateMachine::State::Pause) StateMachine::SetState(StateMachine::State::Gameplay);

	}

	ViewFade::Update(elapsedSec);
	ParticleSystem::Update(elapsedSec);

	if (StateMachine::GetState() == StateMachine::State::Gameplay)
	{
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

void Game::Draw( ) const
{
	ClearBackground();

	if (StateMachine::GetState() == StateMachine::State::Pause)
	{
		TextureManager::GetTexture("PauseScreen")->Draw(Rectf(0,0, m_SCALE * 248, m_SCALE * 224), Rectf(0, -262, 248, 224));
		if (m_pKirby->GetAbilityType() == Entity::AbilityType::None) TextureManager::GetTexture("PauseScreen")->Draw	(Rectf(m_SCALE * 24, m_SCALE * 88, m_SCALE * 192, m_SCALE * 128), Rectf(0, -131, 192, 128));
		if (m_pKirby->GetAbilityType() == Entity::AbilityType::Spark) TextureManager::GetTexture("PauseScreen")->Draw	(Rectf(m_SCALE * 24, m_SCALE * 88, m_SCALE * 192, m_SCALE * 128), Rectf(0, 0, 192, 128));
		if (m_pKirby->GetAbilityType() == Entity::AbilityType::Beam) TextureManager::GetTexture("PauseScreen")->Draw	(Rectf(m_SCALE * 24, m_SCALE * 88, m_SCALE * 192, m_SCALE * 128), Rectf(195, -131, 192, 128));
		if (m_pKirby->GetAbilityType() == Entity::AbilityType::Fire) TextureManager::GetTexture("PauseScreen")->Draw	(Rectf(m_SCALE * 24, m_SCALE * 88, m_SCALE * 192, m_SCALE * 128), Rectf(195, 0, 192, 128));
	}
	if (StateMachine::GetState() == StateMachine::State::Gameplay or StateMachine::GetState() == StateMachine::State::Freeze)
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
		ViewFade::Draw(GetViewPort());
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	if (utils::DEBUG_MODE)
	{
		if (e.keysym.sym == SDLK_i)
		{
			std::cout << *m_pKirby;
		}
		if (e.keysym.sym == SDLK_s)
		{
			m_pCamera->Shake(0.1f, 0.1f);
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
		if (e.keysym.sym == SDLK_r)
		{
			m_pKirby->Reset();
		}	
		if (e.keysym.sym == SDLK_m)
		{
			SoundManager::SetVolume(0);
		}	
		if (e.keysym.sym == SDLK_u)
		{
			SoundManager::SetVolume(50);
		}	
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	if (utils::DEBUG_MODE)
	{
		Point2f clickPos{ (float(e.x) / m_SCALE + m_pCamera->GetCameraView().left) , float(e.y) / m_SCALE - 64 };
		clickPos.y += m_VegetableValleyManager->GetCurrentLevel()->GetCurrentSubLevel() * m_VegetableValleyManager->GetCurrentLevel()->GetSubLevelHeight();

		if (e.button == SDL_BUTTON_LEFT)
		{
			m_pKirby->SetPosition(clickPos);
		}
	}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::LoadTextures()
{
	TextureManager::LoadTexture("Kirby2",				"Kirby/Kirby2.png");
	TextureManager::LoadTexture("Puff",					"Abilities/Puff.png");
	TextureManager::LoadTexture("StarProjectile",		"Abilities/Star.png");
	TextureManager::LoadTexture("Beam",					"Abilities/Beam.png");
	TextureManager::LoadTexture("Fire",					"Abilities/Fire.png");
	TextureManager::LoadTexture("Spark",				"Abilities/Spark.png");
	TextureManager::LoadTexture("BrontoBurt",			"Enemies/BrontoBurt.png");
	TextureManager::LoadTexture("HotHead",				"Enemies/HotHead.png");
	TextureManager::LoadTexture("Sparky",				"Enemies/Sparky.png");
	TextureManager::LoadTexture("WaddleDee",			"Enemies/WaddleDee.png");
	TextureManager::LoadTexture("PoppyBrosJr",			"Enemies/PoppyBrosJr.png");
	TextureManager::LoadTexture("WaddleDoo",			"Enemies/WaddleDoo.png");
	TextureManager::LoadTexture("HUD",					"HUD/HUD.png");
	TextureManager::LoadTexture("Kirby",				"Kirby/Kirby.png");
	TextureManager::LoadTexture("VegetableValleyLevel",	"Levels/VegetableValleyLevel.png");
	TextureManager::LoadTexture("VegetableValleyHub",	"Levels/VegetableValleyHub.png");
	TextureManager::LoadTexture("PowerUps",				"PowerUps/Items.png");
	TextureManager::LoadTexture("PauseScreen",			"HUD/PauseScreen.png");
}
void Game::LoadSounds()
{
	SoundManager::LoadSoundEffect("Turn", "Sound/SoundEffects/23.wav");
	SoundManager::LoadSoundEffect("Jump", "Sound/SoundEffects/Jump.wav");
	SoundManager::LoadSoundEffect("InhalingStart", "Sound/SoundEffects/InhalingStart.wav");
	SoundManager::LoadSoundEffect("Inhaling", "Sound/SoundEffects/Inhaling.wav");
	SoundManager::LoadSoundEffect("StarSpit", "Sound/SoundEffects/StarSpit.ogg");
	SoundManager::LoadSoundEffect("Beam", "Sound/SoundEffects/Beam.ogg");
	SoundManager::LoadSoundEffect("Land", "Sound/SoundEffects/Land.wav");
	SoundManager::LoadSoundEffect("Slide", "Sound/SoundEffects/Slide.wav");
	SoundManager::LoadSoundEffect("Duck", "Sound/SoundEffects/Duck.wav");
	SoundManager::LoadSoundEffect("Falling", "Sound/SoundEffects/Falling.wav");
	SoundManager::LoadSoundEffect("PowerUpReceived", "Sound/SoundEffects/PowerUpReceived.wav");
	SoundManager::LoadSoundEffect("Hit", "Sound/SoundEffects/Hit.wav");
	SoundManager::LoadSoundEffect("StartFlight", "Sound/SoundEffects/StartFlight.wav");
	SoundManager::LoadSoundEffect("Spark", "Sound/SoundEffects/Spark.wav");
	SoundManager::LoadSoundEffect("Puff", "Sound/SoundEffects/Puff.wav");
	SoundManager::LoadSoundEffect("Fire", "Sound/SoundEffects/Fire.wav");
	SoundManager::LoadSoundEffect("EnemyJump", "Sound/SoundEffects/EnemyJump.wav");
	SoundManager::LoadSoundEffect("DoorEnter", "Sound/SoundEffects/Door.wav");
	SoundManager::LoadSoundEffect("EnemyDeath", "Sound/SoundEffects/EnemyDeath.wav");
	SoundManager::LoadSoundEffect("NoPowerUpReceived", "Sound/SoundEffects/NoPowerUpReceived.wav");


	SoundManager::LoadSoundStream("VegetableValleyLevel", "Sound/Music/Vegetable_Valley_Level.mp3");
	SoundManager::LoadSoundStream("VegetableValleyHub", "Sound/Music/Vegetable_Valley_Hub.mp3");
	SoundManager::LoadSoundStream("Dead", "Sound/Music/Dead.mp3");
}