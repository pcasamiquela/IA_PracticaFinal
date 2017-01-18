/* ========================================================================
   File: Game.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include <SDL.h>                   
#include <SDL_image.h>       
#include <SDL_ttf.h>          
#include <SDL_mixer.h>       
#include "SDL_FontCache/SDL_FontCache.h"

#include "Timer.h"
#include "StateManager.h"

// Add your new states here
enum StateCode
{
	MENU_STATE,
	WIN_STATE,
	LOSE_STATE,
	PRACTICAFINAL_STATE
};

class Game
{
public:
	// Member variables
	SDL_Renderer* renderer;
	SDL_Color backgroundColor;
	bool isRunning;

	// Singleton Pattern - Accessor
	static Game& Instance()
	{
		static Game instance;	// Guaranteed to be destroyed.
								// Instantiated on first use.
		return instance;
	}
	// Singleton Pattern - Delete Copy constructor and operator=
	Game(Game const&) = delete;
	void operator=(Game const&) = delete;

	// Member functions
	bool Init(const char* title, int width, int height, 
		SDL_Color backgroundColor, StateCode initialState);
	void HandleEvents();
	void Update();
	void Render();
	void Clean();
	void Quit();
	void ChangeState(StateCode nextState);

private:
	Timer stepTimer;
	SDL_Window* window;
	StateManager manager;

	// Debug Text variables
	FC_Font* debugSmallFont;
	double updateDuration;
	double renderDuration;
	double updateAverage;
	double renderAverage;
	char debugInformation[64];

	// Singleton Pattern - Constructor is private
	Game() {}
};
