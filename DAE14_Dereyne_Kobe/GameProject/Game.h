#pragma once
#include "Texture.h"
#include "BaseGame.h"
#include "HUD.h"
#include "Camera.h"
#include "Level.h"
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

	void Fade(float duration);
	void FadeUpdate(float elapsedSec);
	bool IsFadingOut() const;
	bool IsFadingIn() const;

	// VARIABLES
	const bool m_DEBUG_MODE{true};
	bool m_PlayerEnteredDoor{false};

	float m_FadeTimer;
	float m_FadeDuration;
	bool m_IsFadingIn;
	bool m_IsFadingOut;

	Kirby* m_pKirby;
	EnemyManager* m_pEnemyMngr;
	
	std::vector<std::vector<Point2f>> m_World;
	Camera* m_pCamera;

	Level* m_pLevel;
	Level* m_pHub;

	HUD* m_pHUD;
	
	const float m_SCALE{2.f};
	GameState m_GameState;
};