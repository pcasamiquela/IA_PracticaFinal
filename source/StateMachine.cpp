/* ========================================================================
   File: StateMachine.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "StateMachine.h"

void StateMachine::InitStateMachine()
{
	Process(EVENT_Enter);
}

void StateMachine::Process(StateMachineEvent event)
{
	// Sends the event to the State Machine
	States(event, currentState);

	// Check for a state change and send new events
	if (stateChange == true)
	{
		stateChange = false;

		// Let the last state clean-up
		States(EVENT_Exit, currentState);

		// Set the new state
		currentState = nextState;

		// Let the new state initialize
		States(EVENT_Enter, currentState);
	}
}

void StateMachine::ChangeState(int newState)
{
	stateChange = true;
	nextState = newState;
}
