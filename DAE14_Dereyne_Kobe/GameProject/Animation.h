#pragma once
#include "Texture.h"
#include <vector>


struct Frame
{
	explicit Frame(const Rectf frameRect, float frameDur);
	~Frame() = default;

	Rectf frameRect;
	float frameDuration;
};

class Animation final
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit Animation(std::vector<Frame>&& frames, bool loop);
	virtual ~Animation() noexcept = default;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void Update(float elapsedSec);
	void Draw(Texture* spritesheet, const Point2f& position) const;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			ACCESSORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool IsActive() const;
	bool IsDone() const;
	int GetCurrentFrame() const;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			MUTATORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void Activate();
	void Deactivate();

private:
	// Private Variables
	std::vector<Frame> m_vFrames;

	float m_AccumSec;
	bool m_Loop;
	int m_CurrentFrame;

	bool m_IsActivated;
	bool m_IsDone;
};

