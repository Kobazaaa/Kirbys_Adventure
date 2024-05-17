#pragma once

class Camera final
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit Camera(float screenWidth, float screenHeight, float scale = 1.f);
	virtual ~Camera() noexcept = default;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void Update(float elapsedSec);
	void Aim(float sublevelW, float sublevelH, float sublevelBottom, const Point2f& trackCenter, float hudHeight);
	void Reset();
	void Shake(float duration, float magnitude);


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			ACCESSORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Rectf GetCameraView() const;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			MUTATORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void SetPosition(const Point2f& pos);
	void SetPosition(float x, float y);

private:
	// Private Variables
	float		m_CameraWidth;
	float		m_CameraHeight;
	Point2f		m_CameraPos;

	float	m_ShakeTimer;
	bool	m_IsShaking;
	float	m_ShakeMagnitude;
	float	m_ShakeDuration;
	Point2f m_CameraPosBeforeShake;

	// Private Constants
	const float m_SCALE;
};

