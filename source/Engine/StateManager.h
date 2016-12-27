/* ========================================================================
   File: StateManager.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "BaseState.h"

class StateManager
{
public:
	void Update(float deltaTime);
	void Render();
	void Clean();
	void ChangeState(BaseState* newState);
private:
	BaseState* currentState = nullptr;
};




