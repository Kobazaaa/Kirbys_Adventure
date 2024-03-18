#pragma once
class Camera
{
public:
	Camera(float screenWidth, float screenHeight, float scale = 1.f);

	void Aim(float levelW, float levelH, const Point2f& trackCenter);
	void Reset();

	Rectf GetCameraView();

private:
	float m_CameraWidth;
	float m_CameraHeight;

	Point2f m_CameraPos;
	const float m_SCALE;
};

