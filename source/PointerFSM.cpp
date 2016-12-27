/* ========================================================================
   File: PointerFSM.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "PointerFSM.h"

// Update
void PointerFSM::UpdateFSM()
{
	if (currentState)
	{
		// Execute State
		currentState->ExecuteUpdate();
	}
}

// Is State
bool PointerFSM::IsState(PointerState& state)
{
	return (currentState == &state);
}

// Goto State
void PointerFSM::ChangeState(PointerState& newState)
{
	// Call the exit method of the existing state
	currentState->ExecuteExit();
	// Change state to the new state
	currentState = &newState;
	// Call the entry method of the new state
	currentState->ExecuteEnter();
}

