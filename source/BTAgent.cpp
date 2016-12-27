/* ========================================================================
   File: BTAgent.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "BTAgent.h"

void BTAgent::Update(float deltaTime)
{
	bt->tick();
	BasicAgent::Update(deltaTime);
}

void BTAgent::Render()
{
	BasicAgent::Render();
}

void BTAgent::Clean()
{
	DeinitBehaviorTree();
	BasicAgent::Clean();
}

void BTAgent::DeinitBehaviorTree()
{
	delete bt;
}