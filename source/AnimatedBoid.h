/* ========================================================================
   File: AnimatedBoid.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"
#include "Boid.h"

class AnimatedBoid : public Boid
{
public:
	AnimatedBoid(int x = 0, int y = 0, int width = 0, int height = 0);
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Clean() override;

private:
	// Graphic vars
	float overridenAngle;
	float movementAngle;

	// Private functions
	void UpdateSprite();
};