#include "pch.h"
#include "SVGParser.h"
#include "BaseState.h"
#include "Game.h"
#include "utils.h"
#include "ParticleSystem.h"
#include "ViewFade.h"
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
	ParticleSystem::InitializeParticleSystem();

	// Sounds
	LoadSounds();
	// Textures
	LoadTextures();

	m_pStateMachine = new StateMachine(GetViewPort());
}

void Game::Cleanup( )
{
	delete m_pStateMachine;
	m_pStateMachine = nullptr;

	SoundManager::DeleteAllSound();
	TextureManager::DeleteTextures();
}

void Game::Update( float elapsedSec )
{
	m_pStateMachine->Update(elapsedSec);

	if (ViewFade::IsFading() and ViewFade::CauseFreeze())
	{
		m_pStateMachine->Freeze();
	}
	else m_pStateMachine->Unfreeze();

	if (utils::KeyPress(SDL_SCANCODE_RETURN) and m_pStateMachine->GetState() == StateMachine::State::Titlescreen)
	{
		m_pStateMachine->ChangeState(StateMachine::State::Gameplay);
	}
	if (utils::KeyPress(SDL_SCANCODE_ESCAPE) and !ViewFade::IsFading())
	{
		if		(m_pStateMachine->GetState() == StateMachine::State::Gameplay)	m_pStateMachine->ChangeState(StateMachine::State::Pause);
		else if (m_pStateMachine->GetState() == StateMachine::State::Pause)		m_pStateMachine->ChangeState(StateMachine::State::Gameplay);
	}

	ViewFade::Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground();

	m_pStateMachine->Draw();
	ViewFade::Draw(GetViewPort());
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
}
void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
}
void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
}
void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
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
	TextureManager::LoadTexture("TitleScreen",			"HUD/TitleScreen.png");
	TextureManager::LoadTexture("GameOverScreen",		"HUD/GameOverScreen.png");
	TextureManager::LoadTexture("LevelTiles",			"Levels/LevelTiles.png");
}
void Game::LoadSounds()
{
	SoundManager::LoadSoundEffect("GameOver", "Sound/SoundEffects/GameOver.mp3");
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
	SoundManager::LoadSoundEffect("Pause", "Sound/SoundEffects/Pause.wav");
	SoundManager::LoadSoundEffect("Unpause", "Sound/SoundEffects/Unpause.wav");
	SoundManager::LoadSoundEffect("LowHealth", "Sound/SoundEffects/LowHealth.wav");
	SoundManager::LoadSoundEffect("Dead", "Sound/SoundEffects/Dead.mp3");


	SoundManager::LoadSoundStream("VegetableValleyLevel", "Sound/Music/Vegetable_Valley_Level.mp3");
	SoundManager::LoadSoundStream("VegetableValleyHub", "Sound/Music/Vegetable_Valley_Hub.mp3");
	SoundManager::LoadSoundStream("TitleMusic", "Sound/Music/TitleMusic.mp3");
}