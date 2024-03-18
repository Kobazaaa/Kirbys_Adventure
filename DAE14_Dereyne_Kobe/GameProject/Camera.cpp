#include "pch.h"
#include "Camera.h"
#include "utils.h"
#include <iostream>

Camera::Camera(float screenWidth, float screenHeight, float scale)
	: m_CameraWidth		{screenWidth / scale }
	, m_CameraHeight	{screenHeight / scale}
	, m_CameraPos		{0.f, 0.f}
	, m_SCALE			{scale}
{
}


void Camera::Aim(float levelW, float levelH, const Point2f& trackCenter)
{
	glPushMatrix();
	Point2f cameraCenter	{ m_CameraPos.x    + m_CameraWidth / 2, m_CameraPos.y   + m_CameraHeight / 2};

	float moveBoxW			{ m_CameraWidth  / 6};
	float moveBoxH			{ 10};
	Rectf movementBox
	{
		cameraCenter.x - moveBoxW,
		cameraCenter.y + m_CameraHeight / 2 - 10,
		moveBoxW, moveBoxH
	};

    if		(trackCenter.x < movementBox.left)					m_CameraPos.x += (trackCenter.x - (movementBox.left));
	else if (trackCenter.x > movementBox.left   + moveBoxW)		m_CameraPos.x += (trackCenter.x - (movementBox.left   + moveBoxW));
	if		(trackCenter.y < movementBox.bottom)				m_CameraPos.y += (trackCenter.y - (movementBox.bottom));
	else if (trackCenter.y > movementBox.bottom + moveBoxH)		m_CameraPos.y += (trackCenter.y - (movementBox.bottom + moveBoxH));


	if		(m_CameraPos.x <= 0.f)							m_CameraPos.x = 0.f;
	else if (m_CameraPos.x > levelW - m_CameraWidth)		m_CameraPos.x = levelW - m_CameraWidth;
	if		(m_CameraPos.y <= 0.f)							m_CameraPos.y = 0.f;
	//else if (m_CameraPos.y > levelH - m_CameraHeight)		m_CameraPos.y = levelH - m_CameraHeight;

	glScalef(m_SCALE, m_SCALE, 0);
	glTranslatef(-m_CameraPos.x, -m_CameraPos.y, 0);
}

void Camera::Reset()
{
	glPopMatrix();
}

Rectf Camera::GetCameraView()
{
	return Rectf(m_CameraPos.x, m_CameraPos.y, m_CameraWidth, m_CameraHeight);
}
