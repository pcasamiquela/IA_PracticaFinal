/* ========================================================================
   File: WarriorPlayer.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "WarriorPlayer.h"

WarriorPlayer::WarriorPlayer(int x, int y, int width, int height) :
	DibSprite(x, y, width, height)
{
	// Set Flags
	moves = true;
	isActive = true;

	// Set Size
	SetWidth(16);
	SetHeight(16);
	SetScale(2.0f, 2.0f);

	// Set Graphic

	// Note_dcollado: Warrior Spritesheet Sizes
	//std::string assetPath = PathUtils::GetResourcesPath("images/WarriorSpritesheet.png");
	//player->LoadGraphic(assetPath, "WarriorPlayerSpritesheet", 24, 33, true);

	std::string assetPath = PathUtils::GetResourcesPath("images/KidSpritesheet.png");
	LoadGraphic(assetPath, "PlayerSpritesheet", 32, 32, true);
	
	// Animations
	int frameSpeed = 14;
	animationController->Add("MoveUp", 0, { 0, 1, 2 }, frameSpeed, true);
	animationController->Add("KidMoveUp", 0, { 3, 4, 5 }, frameSpeed, true);
	animationController->Add("IdleUp", 0, { 1 }, frameSpeed, true);
	animationController->Add("KidUp", 0, { 4 }, frameSpeed, true);
	animationController->Add("MoveUpRight", 1, { 0, 1, 2 }, frameSpeed, true);
	animationController->Add("KidMoveUpRight", 1, { 3, 4, 5 }, frameSpeed, true);
	animationController->Add("IdleUpRight", 1, { 1 }, frameSpeed, true);
	animationController->Add("KidIdleUpRight", 1, { 4 }, frameSpeed, true);
	animationController->Add("MoveRight", 2, { 0, 1, 2 }, frameSpeed, true);
	animationController->Add("KidMoveRight", 2, { 3, 4, 5 }, frameSpeed, true);
	animationController->Add("IdleRight", 2, { 1 }, frameSpeed, true);
	animationController->Add("KidIdleRight", 2, { 4 }, frameSpeed, true);
	animationController->Add("MoveDownRight", 3, { 0, 1, 2 }, frameSpeed, true);
	animationController->Add("KidMoveDownRight", 3, { 3, 4, 5 }, frameSpeed, true);
	animationController->Add("IdleDownRight", 3, { 1 }, frameSpeed, true);
	animationController->Add("KidIdleDownRight", 3, { 4 }, frameSpeed, true);
	animationController->Add("MoveDown", 4, { 0, 1, 2 }, frameSpeed, true);
	animationController->Add("KidMoveDown", 4, { 3, 4, 5 }, frameSpeed, true);
	animationController->Add("IdleDown", 4, { 1 }, frameSpeed, true);
	animationController->Add("KidIdleDown", 4, { 4 }, frameSpeed, true);
	animationController->Play("KidIdleRight", true);
	//ShowCollisionBox(true);
};

void WarriorPlayer::Update(float deltaTime)
{
	// Update player
	HandleInput();
	UpdateSpeed(deltaTime);

	// Call DibSprite Update
	DibSprite::Update(deltaTime);

	// Wrap player
	Utils::DoWrap(position);

	// Update Graphic
	UpdateSprite();
}

void WarriorPlayer::Render()
{
	if (isActive)
	{
		DibSprite::Render();
	}
}

void WarriorPlayer::Clean()
{
	DibSprite::Clean();
}

void WarriorPlayer::HandleInput()
{
	// Keyboard movement
	Uint8 keyUp = Input::Instance().GetKey(KeyCode::UpArrow);
	Uint8 keyDown = Input::Instance().GetKey(KeyCode::DownArrow);
	Uint8 keyLeft = Input::Instance().GetKey(KeyCode::LeftArrow);
	Uint8 keyRight = Input::Instance().GetKey(KeyCode::RightArrow);

	if (Input::Instance().GetKeyDown(KeyCode::T))
	{
		isDangerous = !isDangerous;
	}

	// Update input flags
	moveHorizontal = keyRight - keyLeft;
	moveVertical = keyDown - keyUp;
}

void WarriorPlayer::UpdateSpeed(float deltaTime)
{
	static const float ACCELERATION = 30.0f * 60.0f;
	static const float FRICTION = 25.0f * 60.0f;
	static const float MAX_SPEED = 5.0f * 60.0f;

	float deltaFriction = FRICTION * deltaTime;
	float deltaAcceleration = ACCELERATION * deltaTime;

	// Horizontal movement
	if (moveHorizontal != 0)
	{
		speed.x += moveHorizontal * deltaAcceleration;
	}
	else
	{
		speed.x = Utils::ApplyFriction(speed.x, deltaFriction);
	}

	// Vertical movement
	if (moveVertical != 0)
	{
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

void WarriorPlayer::UpdateSprite()
{
	bool movingFlag = false;

	if (speed.Length() > 0.0f)
	{
		movingFlag = true;
		movementAngle = FloatUtils::CalculateOrientation(speed);
	}

	std::string currentAnimationName = "";
	if (isDangerous == false)
	{
		currentAnimationName = "Kid";
	}

	if (movingFlag)
	{
		currentAnimationName.append("Move");
	}
	else
	{
		currentAnimationName.append("Idle");
	}

	// Animation
	if (movementAngle >= -157.5f && movementAngle <= -112.5f)
	{
		if (currentFlip == SDL_FLIP_NONE)
		{
			currentFlip = SDL_FLIP_HORIZONTAL;
		}

		currentAnimationName.append("UpRight");
		if (animationController->currentAnimation->name != currentAnimationName)
		{
			animationController->Play(currentAnimationName, true);
		}
	}
	if (movementAngle > -112.5f && movementAngle <= -67.5f)
	{
		if (currentFlip == SDL_FLIP_HORIZONTAL)
		{
			currentFlip = SDL_FLIP_NONE;
		}

		currentAnimationName.append("Up");
		if (animationController->currentAnimation->name != currentAnimationName)
		{
			animationController->Play(currentAnimationName, true);
		}
	}
	if (movementAngle > -67.5f && movementAngle <= -22.5f)
	{
		if (currentFlip == SDL_FLIP_HORIZONTAL)
		{
			currentFlip = SDL_FLIP_NONE;
		}
		currentAnimationName.append("UpRight");
		if (animationController->currentAnimation->name != currentAnimationName)
		{
			animationController->Play(currentAnimationName, true);
		}
	}
	if (movementAngle < 22.5f && movementAngle > -22.5f)
	{
		if (currentFlip == SDL_FLIP_HORIZONTAL)
		{
			currentFlip = SDL_FLIP_NONE;
		}
		currentAnimationName.append("Right");
		if (animationController->currentAnimation->name != currentAnimationName)
		{
			animationController->Play(currentAnimationName, true);
		}
	}
	else if (movementAngle >= 22.5f && movementAngle < 67.5f)
	{
		if (currentFlip == SDL_FLIP_HORIZONTAL)
		{
			currentFlip = SDL_FLIP_NONE;
		}
		currentAnimationName.append("DownRight");
		if (animationController->currentAnimation->name != currentAnimationName)
		{
			animationController->Play(currentAnimationName, true);
		}
	}
	else if (movementAngle >= 67.5f && movementAngle < 112.5f)
	{
		if (currentFlip == SDL_FLIP_HORIZONTAL)
		{
			currentFlip = SDL_FLIP_NONE;
		}
		currentAnimationName.append("Down");
		if (animationController->currentAnimation->name != currentAnimationName)
		{
			animationController->Play(currentAnimationName, true);
		}
	}
	else if (movementAngle >= 112.5f && movementAngle < 157.5f)
	{
		if (currentFlip == SDL_FLIP_NONE)
		{
			currentFlip = SDL_FLIP_HORIZONTAL;
		}
		currentAnimationName.append("DownRight");
		if (animationController->currentAnimation->name != currentAnimationName)
		{
			animationController->Play(currentAnimationName, true);
		}
	}
	else if (movementAngle >= 157.5f || movementAngle < -157.5f)
	{
		if (currentFlip == SDL_FLIP_NONE)
		{
			currentFlip = SDL_FLIP_HORIZONTAL;
		}
		currentAnimationName.append("Right");
		if (animationController->currentAnimation->name != currentAnimationName)
		{
			animationController->Play(currentAnimationName, true);
		}
	}
}

