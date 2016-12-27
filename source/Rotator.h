/* ========================================================================
   File: Rotator.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

class Rotator : public DibSprite
{
public:
	// Constructor
	Rotator(int x, int y, int width, int height) :
		DibSprite(x, y, width, height)
	{
		moves = false;
		isActive = true;
	}

	virtual void Update(float deltaTime) override;
	virtual void Render() override;

private:
	// Input flags
	int moveHorizontal = 0;

	void HandleInput();
	void Rotate(float deltaTime);
};