/* ========================================================================
   File: DibAnimation.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "DibAnimation.h"

DibAnimation::DibAnimation(DibAnimationController* controller, std::string name,
	int row, std::vector<int> frames, int frameRate, bool looped) :
	name{ name },
	row{ row },
	frames{ frames },
	frameRate{ frameRate },
	looped{ looped }
{
	if (frameRate > 0)
	{
		delay = 1.0f / frameRate;
	}
	else
	{
		delay = 0.0f;
	}
	numFrames = (int)frames.size();
	currentIndex = 0;
}

void DibAnimation::Update(float deltaTime)
{
	if (delay == 0.0f || finished || paused)
		return;

	frameTimer += deltaTime;

	while (frameTimer > delay && !finished)
	{
		frameTimer -= delay;
		if (reversed)
		{
			if (looped && currentFrame == 0)
			{
				currentFrame = numFrames - 1;
				UpdateFrameIndex();
			}
			else
			{
				--currentFrame;
				UpdateFrameIndex();
			}
		}
		else
		{
			if (looped && currentFrame == numFrames - 1)
			{
				currentFrame = 0;
				UpdateFrameIndex();
			}
			else
			{
				++currentFrame;
				UpdateFrameIndex();
			}
		}
	}
}

void DibAnimation::Play(bool force_, bool reversed_, int frame)
{
	if (!force_ && !finished && reversed == reversed_)
	{
		paused = false;
		finished = false;
		return;
	}

	reversed = reversed_;
	paused = false;
	frameTimer = 0.0f;
	finished = (delay == 0.0f);

	int maxFrameIndex = numFrames - 1;
	if (frame < 0)
	{
		return;
	}
	else
	{
		if (frame > maxFrameIndex)
		{
			frame = maxFrameIndex;
		}
		if (reversed)
		{
			frame = (maxFrameIndex - frame);
		}
		currentFrame = frame;
		UpdateFrameIndex();
	}
}

void DibAnimation::Restart()
{
	Play(true, reversed);
}

void DibAnimation::Reset()
{
	Stop();
	currentFrame = reversed ? (numFrames - 1) : 0;
	UpdateFrameIndex();
}

void DibAnimation::Finish()
{
	Stop();
	currentFrame = reversed ? (numFrames - 1) : 0;
	UpdateFrameIndex();
}

void DibAnimation::Stop()
{
	finished = true;
	paused = true;
}

void DibAnimation::Pause()
{
	paused = true;
}

void DibAnimation::Resume()
{
	paused = false;
}

void DibAnimation::Reverse()
{
	reversed = !reversed;
	if (finished)
	{
		Play(false, reversed);
	}
}

void DibAnimation::UpdateFrameIndex()
{
	int maxFrameIndex = numFrames - 1;
	int updatedFrame = currentFrame;

	if (reversed)
		updatedFrame = (maxFrameIndex - updatedFrame);

	if (updatedFrame >= 0)
	{
		if (!looped && updatedFrame > maxFrameIndex)
		{
			finished = true;
			currentFrame = reversed ? 0 : maxFrameIndex;
		}
		else
		{
			currentFrame = updatedFrame;
		}

		currentIndex = frames[currentFrame];
	}
}
