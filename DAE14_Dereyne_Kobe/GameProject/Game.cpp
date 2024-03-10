#include "pch.h"
#include "SVGParser.h"
#include "Game.h"
#include "utils.h"
#include "Kirby.h"
#include "Enemy.h"
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
	SVGParser::GetVerticesFromSvgFile("Level.svg", m_World);
	m_pKirby = new Kirby(Point2f(100, 300));
	m_pEnemy = new Enemy("WaddleDee.png", Point2f(300, 300));
}

void Game::Cleanup( )
{
	delete m_pKirby;
	delete m_pEnemy;
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	m_pKirby->Update(elapsedSec, m_World);
	m_pEnemy->Update(elapsedSec, m_World);
}

void Game::Draw( ) const
{
	ClearBackground( );
	utils::DrawPolygon(m_World[0]);
	m_pKirby->Draw();
	m_pEnemy->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	if (e.button == SDL_BUTTON_LEFT)
	{
		m_pKirby->SetPosition(Point2f(float(e.x), float(e.y)));
	}
	if (e.button == SDL_BUTTON_RIGHT)
	{
		m_pEnemy->SetPosition(Point2f(float(e.x), float(e.y)));
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
