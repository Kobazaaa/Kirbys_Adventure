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
	explicit Animation(std::vector<Frame>&& frames, bool loop);
	virtual ~Animation() = default;

	void Update(float elapsedSec);
	void Draw(Texture* spritesheet, const Point2f& position) const;

	bool IsActive() const;
	bool IsDone() const;
	int GetCurrentFrame() const;

	void Activate();
	void Deactivate();
private:
	std::vector<Frame> m_vFrames;

	float m_AccumSec;
	bool m_Loop;
	int m_CurrentFrame;

	bool m_IsActivated;
	bool m_IsDone;
};

