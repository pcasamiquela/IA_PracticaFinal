/* ========================================================================
   File: BlackboardAgent.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

class StaticBlackboard;
class DynamicBlackboard;

class BlackboardAgent : public DibSprite
{
public:
	BlackboardAgent(int x = 0, int y = 0, int width = 0, int height = 0) :
		DibSprite(x, y, width, height)
	{
		moves = false;
		isActive = true;

		// Create Blackboards
		InitBlackboards();
	};

	virtual void Clean() override;

	// Blackboards
	StaticBlackboard* staticBlackboard;
	DynamicBlackboard* dynamicBlackboard;

private:

	// Initialize / Delete Blackboards
	void InitBlackboards();
	void FreeBlackboards();
};