/* ========================================================================
   File: BTAgent.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "BasicAgent.h"
#include "BehaviorTree.h"

class BTAgent : public BasicAgent
{
public:
	// Constructor
	BTAgent(int x, int y, int width, int height, Texture* questionTexture) :
		BasicAgent(x, y, width, height, questionTexture)
	{}

	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Clean() override;

protected:

	// Behavior Tree
	BehaviorTree* bt;

	// BT functions
	virtual void InitBehaviorTree() = 0;
	virtual void DeinitBehaviorTree();
};