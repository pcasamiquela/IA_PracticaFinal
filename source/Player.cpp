/* ========================================================================
   File: Player.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Player.h"

void Player::Update(float deltaTime)
{
	// Update player
	HandleInput();
	UpdateSpeed(deltaTime);

	// Call DibSprite Update
	DibSprite::Update(deltaTime);

	//UpdateAngle();

	ManageAnimations();
	// Wrap player
	Utils::DoWrap(position);
}

void Player::Render()
{
	if (!isActive)
		return;

	DibSprite::Render();
}

void Player::Clean()
{
	DibSprite::Clean();
}

void Player::HandleInput()
{
	// Keyboard movement
	Uint8 keyUp = Input::Instance().GetKey(KeyCode::UpArrow);
	Uint8 keyDown = Input::Instance().GetKey(KeyCode::DownArrow);
	Uint8 keyLeft = Input::Instance().GetKey(KeyCode::LeftArrow);
	Uint8 keyRight = Input::Instance().GetKey(KeyCode::RightArrow);

	// Update input flags
	moveHorizontal = keyRight - keyLeft;
	moveVertical = keyDown - keyUp;
}

void Player::UpdateSpeed(float deltaTime)
{
	static const float ACCELERATION = 60.0f * 60.0f;
	static const float FRICTION = 25.5f * 60.0f;
	static const float MAX_SPEED = 4.0f * 60.0f;

	float deltaFriction = FRICTION * deltaTime;
	float deltaAcceleration = ACCELERATION * deltaTime;

	// Horizontal movement
	if (moveHorizontal != 0)
	{
		// First add friction if currently going right
		if (speed.x != 0.0f)
		{
			speed.x = Utils::ApplyFriction(speed.x, deltaFriction);
		}
		speed.x += moveHorizontal * deltaAcceleration;
	}
	else
	{
		speed.x = Utils::ApplyFriction(speed.x, deltaFriction);
	}

	// Vertical movement
	if (moveVertical != 0)
	{
		// Moving right
		// First add friction if currently going left
		if (speed.y != 0.0f)
		{
			speed.y = Utils::ApplyFriction(speed.y, deltaFriction);
		}
		speed.y += moveVertical * deltaAcceleration;
	}
	else
	{
		speed.y = Utils::ApplyFriction(speed.y, deltaFriction);
	}

	// Clamp speed
	if (speed.Length() > MAX_SPEED)
	{
		speed.Normalize();
		speed *= MAX_SPEED;
	}
}

void Player::UpdateAngle()
{
	if (speed.Length() > 0.0f)
	{
		angle = FloatUtils::CalculateOrientation(speed);
	}
}

void Player::ManageAnimations()
{
	if (Input::Instance().GetKey(KeyCode::UpArrow) && Input::Instance().GetKey(KeyCode::RightArrow))	animationController->Play("RunUpDiagonalRight");
	else if (Input::Instance().GetKey(KeyCode::UpArrow) && Input::Instance().GetKey(KeyCode::LeftArrow))	animationController->Play("RunUpDiagonalLeft");
	else if (Input::Instance().GetKey(KeyCode::DownArrow)&& Input::Instance().GetKey(KeyCode::RightArrow))	animationController->Play("RunDownDiagonalRight");
	else if (Input::Instance().GetKey(KeyCode::DownArrow) && Input::Instance().GetKey(KeyCode::LeftArrow))	animationController->Play("RunDownDiagonalLeft");
	else if (Input::Instance().GetKey(KeyCode::UpArrow))	animationController->Play("RunUp");
	else if (Input::Instance().GetKey(KeyCode::DownArrow))	animationController->Play("RunDown");
	else if (Input::Instance().GetKey(KeyCode::LeftArrow))	animationController->Play("RunLeft");
	else if (Input::Instance().GetKey(KeyCode::RightArrow))	animationController->Play("RunRight");
	else animationController->Play("Idle");
}
