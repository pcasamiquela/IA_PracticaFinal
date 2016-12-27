/* ========================================================================
   File: StateMachine.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

class StateMachine
{
public:
	enum StateMachineEvent
	{
		EVENT_Enter,
		EVENT_Update,
		EVENT_Exit
	};

	void InitStateMachine();
	void Process(StateMachineEvent event);
	void ChangeState(int newState);
	virtual bool States(StateMachineEvent event, int state) = 0;

private:
	int currentState;
	int nextState;
	bool stateChange;
};

