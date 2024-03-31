#pragma once
class Camera final
{
public:
	// Constructor & Destructor
	Camera(float screenWidth, float screenHeight, float scale = 1.f);
	~Camera();

	// Behavioral
	void Aim(float levelW, float levelH, const Point2f& trackCenter);
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

