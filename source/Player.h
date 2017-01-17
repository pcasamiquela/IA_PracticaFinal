/* ========================================================================
   File: Player.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"
#include "Locker.h"
#include <vector>
using namespace std;

class Player : public DibSprite
{
public:
	Player(int x, int y, int width, int height) :
		DibSprite(x, y, width, height)
	{
		moves = true;
		isActive = true;
	};

	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Clean() override;
	void EnterToLocker(Locker &currentLocker);

private:
	// Input flags
	int moveHorizontal;
	int moveVertical;

	void HandleInput();
	void UpdateSpeed(float deltaTime);
	void UpdateAngle();
	void ManageAnimations();
};