/* ========================================================================
   File: StateManager.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "StateManager.h"
#include <cassert>

void StateManager::Update(float deltaTime)
{
	assert(currentState != nullptr && "Update currentState == nullptr");
	currentState->Update(deltaTime);
}

void StateManager::Render()
{
	assert(currentState != nullptr && "Render currentState == nullptr");
	currentState->Render();
}

void StateManager::Clean()
{
	if (currentState != nullptr)
	{
		currentState->Clean();
		delete currentState;
		currentState = nullptr;
	}
}

void StateManager::ChangeState(BaseState* newState)
{
	if (currentState != nullptr)
	{
		currentState->Clean();
		delete currentState;
	}
	currentState = newState;
	currentState->Init();
}
