#include "pch.h"
#include "Animation.h"

Animation::Animation()
	: m_Loop		{ false }
	, m_AccumSec	{ 0 }
	, m_CurrentFrame{ 0 }
	, m_IsActivated	{ false }
	, m_vFrames		{ }
	, m_IsDone		{ false }
{
}

Animation::Animation(std::vector<Frame>&& frames, bool loop)
	: m_Loop{ loop }
	, m_AccumSec{ 0 }
	, m_CurrentFrame{ 0 }
	, m_IsActivated{ false }
	, m_vFrames{ frames }
	, m_IsDone{ false }
{
}

void Animation::Update(float elapsedSec)
{
	if (m_IsActivated)
	{
		m_AccumSec += elapsedSec;
		if (m_AccumSec >= m_vFrames[m_CurrentFrame].frameDuration)
		{
			m_AccumSec = 0;

			if (m_Loop) m_CurrentFrame = (m_CurrentFrame + 1) % (m_vFrames.size());
			else
			{
				m_CurrentFrame = (m_CurrentFrame + 1);
				if (m_CurrentFrame >= m_vFrames.size())
				{
					m_CurrentFrame = m_vFrames.size() - 1;
					m_IsDone = true;
				}
				else m_IsDone = false;
			}
		}
	}
	else
	{
		m_AccumSec = 0;
		m_CurrentFrame = 0;
	}
}
void Animation::Draw(Texture* spritesheet, const Point2f& position) const
{
	if (m_IsActivated)
	{
		Point2f pos
		{
			position.x - m_vFrames[m_CurrentFrame].frameRect.width / 2,
			position.y - 8
		};
		spritesheet->Draw(pos, m_vFrames[m_CurrentFrame].frameRect);
	}
}

bool Animation::IsActive() const
{
	return m_IsActivated;
}

bool Animation::IsDone() const
{
	return m_IsDone;
}

int Animation::GetCurrentFrame() const
{
	return m_CurrentFrame;
}

void Animation::Activate()
{
	m_IsActivated = true;
}
void Animation::Deactivate()
{
	m_AccumSec = 0;
	m_CurrentFrame = 0;
	m_IsActivated = false;
	m_IsDone = false;
}

Frame::Frame(const Rectf frameRect, float frameDur)
	: frameRect{ frameRect }
	, frameDuration{ frameDur }
{
}
