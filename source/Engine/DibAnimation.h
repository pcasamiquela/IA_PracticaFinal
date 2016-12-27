/* ========================================================================
   File: DibAnimation.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include <string>
#include <vector>

// Forward declaration of DibAnimationController
class DibAnimationController;

class DibAnimation
{
public:
	// Member variables
	std::string name;
	int row;
	int currentIndex;

	// Constructor
	DibAnimation(DibAnimationController* controller, std::string name,
		int row, std::vector<int> frames, int frameRate, bool looped);

	// Member functions
	void Update(float deltaTime);
	void Play(bool force_, bool reversed_, int frame = 0);
	void Restart();
	void Reset();
	void Finish();
	void Stop();
	void Pause();
	void Resume();
	void Reverse();

private:
	// Member variables
	std::vector<int> frames;
	int frameRate;
	bool looped;

	// Update variables
	int currentFrame;
	int numFrames;
	float frameTimer;
	float delay;

	// State variables
	bool finished;
	bool paused;
	bool reversed;

	// Member functions
	void UpdateFrameIndex();
};