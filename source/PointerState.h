/* ========================================================================
   File: PointerState.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

// CState Class
class PointerState
{
public:
	// Destructor
	virtual ~PointerState() {}

	// State Functions
	virtual void ExecuteEnter() = 0;
	virtual void ExecuteUpdate() = 0;
	virtual void ExecuteExit() = 0;
};

// CStateTemplate Class
template <class T>
class PointerStateTemplate : public PointerState
{
protected:
	typedef void (T::*StateFunction)();
	T* instance; // Instance Pointer
	StateFunction functionEnterState; // State Function Pointer
	StateFunction functionUpdateState; // State Function Pointer
	StateFunction functionExitState; // State Function Pointer

public:
	// Constructor
	PointerStateTemplate() : instance(0), 
		functionEnterState(0), 
		functionUpdateState(0), 
		functionExitState(0) 
	{}

	// Initialize Functions
	void Set(T *instance,
		StateFunction functionEnterState,
		StateFunction functionUpdateState,
		StateFunction functionExitState)
	{
		// Set Instance
		this->instance = instance;
		// Set Function Pointers
		this->functionEnterState = functionEnterState;
		this->functionUpdateState = functionUpdateState;
		this->functionExitState = functionExitState;
	}

	// State Functions
	virtual void ExecuteEnter()
	{
		// Begin State
		(instance->*functionEnterState)();
	}
	virtual void ExecuteUpdate()
	{
		// State
		(instance->*functionUpdateState)();
	}
	virtual void ExecuteExit()
	{
		// End State
		(instance->*functionExitState)();
	}
};

