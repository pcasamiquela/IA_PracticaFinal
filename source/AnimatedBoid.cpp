/* ========================================================================
   File: AnimatedBoid.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "AnimatedBoid.h"

AnimatedBoid::AnimatedBoid(int x, int y, int width, int height) :
	Boid(x, y, width, height)
{
	// Set Flags
	moves = true;
	isActive = true;
	// Set Size
	SetScale(2.0f, 2.0f);
	SetWidth(64);
	SetHeight(64);
	// Set Graphic
	std::string assetPath = PathUtils::GetResourcesPath("images/DragonSpritesheet.png");
	LoadGraphic(assetPath, "DragonSpritesheet", 64, 64, true);
	// Animations
	int frameSpeed = 12;
	animationController->Add("MoveUp", 0, { 0, 1, 2 }, frameSpeed, true);
	animationController->Add("IdleUp", 0, { 1 }, frameSpeed, true);
	animationController->Add("MoveUpRight", 1, { 0, 1, 2 }, frameSpeed, true);
	animationController->Add("IdleUpRight", 1, { 1 }, frameSpeed, true);
	animationController->Add("MoveRight", 2, { 0, 1, 2 }, frameSpeed, true);
	animationController->Add("IdleRight", 2, { 1 }, frameSpeed, true);
	animationController->Add("MoveDownRight", 3, { 0, 1, 2 }, frameSpeed, true);
	animationController->Add("IdleDownRight", 3, { 1 }, frameSpeed, true);
	animationController->Add("MoveDown", 4, { 0, 1, 2 }, frameSpeed, true);
	animationController->Add("IdleDown", 4, { 1 }, frameSpeed, true);
	animationController->Play("IdleRight", true);
	// Debugging
	//ShowCollisionBox(true);
	//ShowSpriteBox(true);
}

void AnimatedBoid::Update(float deltaTime)
{
	// Store current angle
	overridenAngle = angle;

	// Call DecisionTreeBoid Update
	Boid::Update(deltaTime);

	// Avoid angle override from base class
	angle = overridenAngle;

	// Update Graphic
	UpdateSprite();
}

void AnimatedBoid::Render()
{
	if (isActive)
	{
		Boid::Render();
	}
}

void AnimatedBoid::Clean()
{
	Boid::Clean();
}

void AnimatedBoid::UpdateSprite()
{
	bool movingFlag = false;

	if (speed.Length() > 0.0f)
	{
		movingFlag = true;
		movementAngle = FloatUtils::CalculateOrientation(speed);
	}

	std::string currentAnimationName = "";

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

