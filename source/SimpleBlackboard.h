/* ========================================================================
   File: SimpleBlackboard.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

// Hard-coded Singleton Blackboard
class SimpleBlackboard
{
public:
	// Singleton Pattern - Accessor
	static SimpleBlackboard& Instance()
	{
		static SimpleBlackboard instance;	// Guaranteed to be destroyed.
											// Instantiated on first use.
		return instance;
	}
	// Singleton Pattern - Delete Copy constructor and operator=
	SimpleBlackboard(SimpleBlackboard const&) = delete;
	void operator=(SimpleBlackboard const&) = delete;

	// Derived Data function - Blackboard Logic
	bool IsAnyAgentFollowingPlayer()
	{
		for (int i = 0; i < 3; ++i)
		{
			if (playerDetected[i] == true)
			{
				return true;
			}
		}
		return false;
	}

	void SetPlayerDetected(int index, bool detectedValue)
	{
		if (index < 3)
		{
			playerDetected[index] = detectedValue;
		}
	}

	void ResetBlackboard()
	{
		for (int i = 0; i < 3; ++i)
		{
			playerDetected[i] = false;
		}
	}

private:
	// Blackboard data
	bool playerDetected[3];

	// Singleton Pattern - Constructor is private
	SimpleBlackboard() 
	{}
};