/* ========================================================================
   File: State.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

class MinerSimplePointer;

class State
{
public:
	// This will execute when the state is entered
	virtual void Enter(MinerSimplePointer*) = 0;
	// This is the state's normal update function
	virtual void Update(MinerSimplePointer*) = 0;
	// This will execute when the state is exited.
	virtual void Exit(MinerSimplePointer*) = 0;
};
