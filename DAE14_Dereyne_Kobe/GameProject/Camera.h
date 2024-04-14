#pragma once

class Camera final
{
public:
	// Constructor & Destructor
	explicit Camera(float screenWidth, float screenHeight, float scale = 1.f);
	virtual ~Camera() = default;

	// Behavioral
	void Aim(float sublevelW, float sublevelH, float sublevelBottom, const Point2f& trackCenter, float hudHeight);
	void Reset();

	// Accessors
	Rectf GetCameraView() const;

	// Mutators
	void SetPosition(const Point2f& pos);
	void SetPosition(float x, float y);

private:
	// Variables
	float		m_CameraWidth;
	float		m_CameraHeight;

	Point2f		m_CameraPos;
	const float m_SCALE;
};

