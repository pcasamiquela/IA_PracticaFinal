/* ========================================================================
   File: DibAnimationController.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "DibSprite.h"
#include "DibAnimation.h"

#include <unordered_map>
#include <string>

class DibAnimationController
{
public:
	// Member variables
	DibAnimation* currentAnimation;

	// Constructor
	DibAnimationController(DibSprite* sprite_) :
		sprite{ sprite_ }, 
		currentAnimation{nullptr}
	{}

	// Member methods
	void Update(float deltaTime);
	void Clear();
	void Add(std::string name, int animationRow, std::vector<int> frames, int frameRate = 30,
		bool looped = true);
	void Play(std::string animationName, bool force = false, bool reversed = false, int frame = 0);
	void Reset();
	void Finish();
	void Stop();
	void Pause();
	void Resume();
	void Reverse();

private:
	// Member variables
	DibSprite* sprite;
	typedef std::unordered_map<std::string, DibAnimation*> AnimationMap;
	AnimationMap animations;
};