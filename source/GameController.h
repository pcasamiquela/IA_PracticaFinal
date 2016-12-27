/* ========================================================================
   File: GameController.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"
#include "Area.h"
#include "WarriorPlayer.h"

class GameController
{
public:
	// Singleton Pattern - Accessor
	static GameController& Instance()
	{
		static GameController instance;	// Guaranteed to be destroyed.
								// Instantiated on first use.
		return instance;
	}
	// Singleton Pattern - Delete Copy constructor and operator=
	GameController(GameController const&) = delete;
	void operator=(GameController const&) = delete;

	// Singleton Data
	WarriorPlayer* player;
	Area* healthArea;
	Area* coverArea;

private:
	// Singleton Pattern - Constructor is private
	GameController() {}
};