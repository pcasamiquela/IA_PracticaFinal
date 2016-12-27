/* ========================================================================
   File: Main.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Engine/EngineFramework.h"
#include "Constants.h"

// Point of Entry
int main(int argc, char *argv[])
{
	// Initalize Game
	// Set Name of Window
	// Set Window Size
	// Set Background Color
	// Set Initial State
	Game::Instance().Init("Location-Based Knowledge", 
		FIXED_WIDTH, FIXED_HEIGHT, { 0, 184, 167, 255 },
		MENU_STATE);
	
	// Game Loop
	while (Game::Instance().isRunning)
	{
		Game::Instance().HandleEvents();
		Game::Instance().Update();
		Game::Instance().Render();

		// Allow the OS to sleep
		SDL_Delay(1);
	}

	// Cleanup
	Game::Instance().Clean();
	return 0;
}

