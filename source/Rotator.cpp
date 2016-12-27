/* ========================================================================
   File: Rotator.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Rotator.h"

void Rotator::Update(float deltaTime)
{
	HandleInput();
	Rotate(deltaTime);
}

void Rotator::Render()
{
	DibSprite::Render();
}

void Rotator::HandleInput()
{
	// Keyboard movement
	Uint8 keyLeft = Input::Instance().GetKey(KeyCode::LeftArrow);
	Uint8 keyRight = Input::Instance().GetKey(KeyCode::RightArrow);

	moveHorizontal = keyRight - keyLeft;
}

void Rotator::Rotate(float deltaTime)
{
	const float K_ANGULAR_ACCEL = 80.0f * 60.0f;
	const float K_ANGULAR_FRIC = 0.5f * 60.0f;
	const float K_MAX_ANGULAR_SPEED = 1.0f;

	float fric = K_ANGULAR_FRIC * deltaTime;
	float accel = K_ANGULAR_ACCEL * deltaTime;

	if (moveHorizontal > 0)
	{
		// Moving right
		// First add friction if currently going left
		if (angularVelocity < 0)
		{
			angularVelocity = FloatUtils::Approach(angularVelocity, 0, fric);
		}
		angularVelocity = FloatUtils::Approach(angularVelocity, K_MAX_ANGULAR_SPEED, accel);
	}
	else if (moveHorizontal < 0)
	{
		// Moving left
		// First add friction if currently going right
		if (angularVelocity > 0)
		{
			angularVelocity = FloatUtils::Approach(angularVelocity, 0, fric);
		}
		angularVelocity = FloatUtils::Approach(angularVelocity, -K_MAX_ANGULAR_SPEED, accel);
	}
	else
	{
		// Stopping by friction
		angularVelocity = FloatUtils::Approach(angularVelocity, 0, fric);
	}

	angle += angularVelocity;
	if (angle > 180.0f)
	{
		angle -= 360.0f;
	}
	else if (angle < -180.0f)
	{
		angle += 360.0f;
	}
}
