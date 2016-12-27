/* ========================================================================
   File: MinerSimplePointer.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "MinerSimplePointer.h"
#include "MinerOwnedStates.h"

void MinerSimplePointer::InitMiner(char* name)
{
	Miner::InitMiner(name);
	currentState = GoHomeAndSleepTilRested::Instance();
}

void MinerSimplePointer::Update()
{
	Miner::Update();
	if (currentState)
	{
		currentState->Update(this);
	}
}

void MinerSimplePointer::ChangeState(State* newState)
{
	// Call the exit method of the existing state
	currentState->Exit(this);
	// Change state to the new state
	currentState = newState;
	// Call the entry method of the new state
	currentState->Enter(this);
}


