/* ========================================================================
   File: DibAnimationController.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "DibAnimationController.h"

#include <cassert>
#include <SDL.h>

void DibAnimationController::Update(float deltaTime)
{
	assert(currentAnimation != nullptr && "currentAnimation == nullptr");
	currentAnimation->Update(deltaTime);
}

void DibAnimationController::Clear()
{
	// Delete all DibAnimation
	for (auto it : animations)
	{
		delete (DibAnimation*)(it.second);
	}
	animations.clear();
}

void DibAnimationController::Add(std::string name, int animationRow,
	std::vector<int> frames, int frameRate, bool looped)
{
	DibAnimation* anim = new DibAnimation(this, name, animationRow, frames, frameRate, looped);
	animations.insert(std::make_pair(name, anim));
}

void DibAnimationController::Play(std::string animationName, bool force, 
	bool reversed, int frame)
{
	if (animations.find(animationName) == animations.end())
	{
		SDL_Log("No animation called %s", animationName.c_str());
		return;
	}

	if (currentAnimation != nullptr && animationName != currentAnimation->name)
	{
		currentAnimation->Stop();
	}

	currentAnimation = animations[animationName];
	currentAnimation->Play(force, reversed, frame);
}

void DibAnimationController::Reset()
{
	if (currentAnimation != nullptr)
	{
		currentAnimation->Reset();
	}
}

void DibAnimationController::Finish()
{
	if (currentAnimation != nullptr)
	{
		currentAnimation->Finish();
	}
}

void DibAnimationController::Stop()
{
	if (currentAnimation != nullptr)
	{
		currentAnimation->Stop();
	}
}

void DibAnimationController::Pause()
{
	if (currentAnimation != nullptr)
	{
		currentAnimation->Pause();
	}
}

void DibAnimationController::Resume()
{
	if (currentAnimation != nullptr)
	{
		currentAnimation->Resume();
	}
}

void DibAnimationController::Reverse()
{
	if (currentAnimation != nullptr)
	{
		currentAnimation->Reverse();
	}
}
