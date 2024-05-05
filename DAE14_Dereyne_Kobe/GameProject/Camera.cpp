#include "pch.h"
#include "Camera.h"
#include "utils.h"
#include <iostream>

Camera::Camera(float screenWidth, float screenHeight, float scale)
	: m_CameraWidth				{screenWidth / scale }
	, m_CameraHeight			{screenHeight / scale}
	, m_CameraPos				{0.f, 0.f}
	, m_SCALE					{scale}
	, m_ShakeTimer				{ 0.f }
	, m_IsShaking				{ false }
	, m_ShakeDuration			{ 0.f }
	, m_ShakeMagnitude			{ 0.f }
	, m_CameraPosBeforeShake	{ 0.f, 0.f }
{
}

void Camera::Update(float elapsedSec)
{
	if (!m_IsShaking) m_CameraPosBeforeShake = m_CameraPos;

	if (m_IsShaking)
	{
		m_ShakeTimer += elapsedSec;
		m_CameraPos.x += utils::GetRandomFloat(-10 * m_ShakeMagnitude, 10 * m_ShakeMagnitude, 3);
		m_CameraPos.y += utils::GetRandomFloat(-10 * m_ShakeMagnitude, 10 * m_ShakeMagnitude, 3);
	}
	
	if (m_ShakeTimer >= m_ShakeDuration)
	{
		m_ShakeTimer = 0;
		m_IsShaking = false;
		m_ShakeDuration = 0;
		m_ShakeMagnitude = 0;

		m_CameraPos = m_CameraPosBeforeShake;
	}
}

void Camera::Aim(float sublevelW, float sublevelH, float sublevelBottom, const Point2f& trackCenter, float hudHeight)
{
	glPushMatrix();
	Point2f cameraCenter	{ m_CameraPos.x    + m_CameraWidth / 2, m_CameraPos.y   + m_CameraHeight / 2};

	float moveBoxW			{ m_CameraWidth  / 6};
	float moveBoxH			{ m_CameraHeight };
	Rectf movementBox
	{
		cameraCenter.x - moveBoxW,
		cameraCenter.y - moveBoxH / 2,
		moveBoxW, moveBoxH
	};

	if (!m_IsShaking)
	{
		if (trackCenter.x < movementBox.left)					m_CameraPos.x += (trackCenter.x - (movementBox.left));
		else if (trackCenter.x > movementBox.left + moveBoxW)		m_CameraPos.x += (trackCenter.x - (movementBox.left + moveBoxW));
		if (trackCenter.y < movementBox.bottom)				m_CameraPos.y += (trackCenter.y - (movementBox.bottom));
		else if (trackCenter.y > movementBox.bottom + moveBoxH)		m_CameraPos.y += (trackCenter.y - (movementBox.bottom + moveBoxH));
	}


	if (m_CameraPos.x <= 0.f)														m_CameraPos.x = 0.f;
	else if (m_CameraPos.x > sublevelW - m_CameraWidth)									m_CameraPos.x = sublevelW - m_CameraWidth;
	if (m_CameraPos.y <= sublevelBottom)											m_CameraPos.y = sublevelBottom;
	else if (m_CameraPos.y > sublevelBottom + sublevelH - m_CameraHeight + hudHeight)	m_CameraPos.y = sublevelBottom + sublevelH - m_CameraHeight + hudHeight;
	glTranslatef(0,m_SCALE * hudHeight, 0);
	glScalef(m_SCALE, m_SCALE, 0);
	glTranslatef(-m_CameraPos.x, -m_CameraPos.y, 0);
}

void Camera::Reset()
{
	glPopMatrix();
}

void Camera::Shake(float duration, float magnitude)
{
	m_IsShaking = true;
	m_ShakeDuration = duration;
	m_ShakeMagnitude = magnitude;
}

#pragma region Accessors
Rectf Camera::GetCameraView() const
{
	return Rectf(m_CameraPos.x, m_CameraPos.y, m_CameraWidth, m_CameraHeight);
}
#pragma endregion

#pragma region Mutators
void Camera::SetPosition(const Point2f& pos)
{
	m_CameraPos = pos;
}

void Camera::SetPosition(float x, float y)
{
	SetPosition(Point2f(x, y));
}
#pragma endregion