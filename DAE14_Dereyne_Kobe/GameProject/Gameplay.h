#pragma once
#include "BaseState.h"
#include "Entity.h"

class Kirby;
class StateMachine;
class LevelManager;
class HUD;
class Camera;

class Gameplay final : public BaseState
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit Gameplay(float scale, Rectf viewport);
	Gameplay(const Gameplay& other) = delete;
	Gameplay(Gameplay&& other) = delete;
	Gameplay& operator=(const Gameplay& rhs) = delete;
	Gameplay& operator=(Gameplay&& rhs) = delete;
	virtual ~Gameplay() noexcept override;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(StateMachine& stateMachine, float elapsedSec, bool freeze) override;
	virtual void Draw() const override;

	void Reset();


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			ACCESSORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Entity::AbilityType GetKirbyAbility();

private:
	// Private Datamembers
	Kirby* m_pKirby;
	LevelManager* m_VegetableValleyManager;

	Camera* m_pCamera;

	HUD* m_pHUD;

	float m_Scale;
};

