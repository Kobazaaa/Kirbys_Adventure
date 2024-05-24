#pragma once
#include "Texture.h"
#include "BaseGame.h"
#include "HUD.h"
#include "Camera.h"
#include "LevelManager.h"
#include "EnemyManager.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include <vector>

class Kirby;
class Enemy;

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	// ENUM
	enum class GameState
	{
		Hub, Play, Pause
	};

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void LoadTextures();
	void LoadSounds();

	// VARIABLES
	bool m_PlayerEnteredDoor{false};

	Kirby* m_pKirby;
	LevelManager* m_VegetableValleyManager;

	Camera* m_pCamera;

	HUD* m_pHUD;
	
	const float m_SCALE{2.f};
	GameState m_GameState;
};