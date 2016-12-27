/* ========================================================================
   File: TopDownPlayer.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

class WarriorPlayer : public DibSprite
{
public:
	WarriorPlayer(int x = 0, int y = 0, int width = 0, int height = 0);
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Clean() override;

	bool isDangerous;

private:
	// Input flags
	int moveHorizontal;
	int moveVertical;
	// Graphic vars
	float movementAngle;

	// Private functions
	void HandleInput();
	void UpdateSpeed(float deltaTime);
	void UpdateSprite();
};