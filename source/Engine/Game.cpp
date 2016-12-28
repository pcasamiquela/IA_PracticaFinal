/* ========================================================================
   File: Game.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Game.h"
#include "Input.h"

// Add the includes of your states here
#include "../MenuState.h"
#include "SimpleInfluenceMapState.h"
#include "PracticaFInalState.h"

// Add changes to new states here
void Game::ChangeState(StateCode nextState)
{
	switch (nextState)
	{
		case MENU_STATE:
		{
			manager.ChangeState(new MenuState());
		} break;
		case SIMPLE_INFLUENCE_STATE:
		{
			manager.ChangeState(new SimpleInfluenceMapState());
		} break;
		case ADVANCED_INFLUENCE_STATE:
		{
			manager.ChangeState(new AdvancedInfluenceMapState());
		} break;
		//case TEMPLATE:
		//{
		//	manager.ChangeState(new TEMPLATE());
		//} break;
		default:
		{
			// Do nothing
		} break;
	}
}

bool Game::Init(const char* title, int width, int height, SDL_Color backgroundColor, StateCode initialState)
{ 
	// Initialization of SDL, SDL_image, SDL_ttf and SDL_mixer
	if (SDL_Init(0) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "SDL_Init: Couldn't start SDL\n");
		return false;
	}
	else
	{
		// Initializing Subsystems
		int flags = (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
		if (SDL_InitSubSystem(flags) < 0)
		{
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "SDL_Init: Couldn't start Subsystems.\n");
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "SDL_GetError = %s.\n", SDL_GetError());
			return false;
		}

		//// Initializing IMG - Image support
		int imgFlags = IMG_INIT_PNG;
		if (!IMG_Init(imgFlags) & imgFlags)
		{
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "IMG_Init: Couldn't start Image support.\n");
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "IMG_GetError = %s\n", IMG_GetError());
			return false;
		}

		// Initialize TTF support
		if (TTF_Init() == -1)
		{
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "SDL_ttf could not be initialized. TTF_Error: %s\n", TTF_GetError());
			return false;
		}

		// Initialize SDL_mixer.
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		{
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "SDL_mixer could not be initialized. Mix_Error: %s\n", Mix_GetError());
			return false;
		}
	}

	// Window Creation
	window = SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Window could not be created.\n");
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "SDL_GetError = %s.\n", SDL_GetError());
	}
	else
	{
		// Renderer Creation
		renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED
			| SDL_RENDERER_PRESENTVSYNC);
		if (renderer == NULL)
		{
			SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Renderer could not be created.\n");
			SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "SDL_GetError = %s.\n", SDL_GetError());
		}

		SDL_RenderSetLogicalSize(renderer, width, height);
		this->backgroundColor = backgroundColor;
	}

	// Initialize Debug Font
#if CHECK_PERFORMANCE
	std::string assetPath = PathUtils::GetResourcesPath("Fonts/nokiafc22.ttf");
	debugSmallFont = FC_CreateFont();
	FC_LoadFont(debugSmallFont,renderer,
		assetPath.c_str(), 18, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
#endif

	// Initialize Timer
	Timer_Start(&stepTimer);

	// Initialize Input
	Input::Instance().Init();

	// Initialize State Manager
	ChangeState(initialState);

	// Everything initiated successfully!
	isRunning = true;

	return isRunning;
}

void Game::HandleEvents() 
{ 
	Input::Instance().HandleEvents();
}

void Game::Update() 
{ 
	// Get deltaTime
	float deltaTime = Timer_GetTicks(&stepTimer) / 1000.0f;

#if CHECK_PERFORMANCE
	static Uint64 start, now, frameCounter = 0;
	static double totalDuration = 0.0;
	start = SDL_GetPerformanceCounter();
#endif

	// Update Current Game State
	manager.Update(deltaTime);

#if CHECK_PERFORMANCE
	now = SDL_GetPerformanceCounter();
	++frameCounter;
	updateDuration = (double)((now - start) * 1000.0) / SDL_GetPerformanceFrequency();
	totalDuration += updateDuration;
	updateAverage = totalDuration / (double)frameCounter;
#endif

	// Reset Timer
	Timer_Start(&stepTimer);
}

void Game::Render() 
{ 
	// Clear renderer to background color
	SDL_SetRenderDrawColor(renderer,
		backgroundColor.r,
		backgroundColor.g,
		backgroundColor.b,
		backgroundColor.a);
	SDL_RenderClear(renderer);

#if CHECK_PERFORMANCE
	static Uint64 start, now, frameCounter = 0;
	static double totalDuration = 0.0;
	start = SDL_GetPerformanceCounter();
#endif

	// Render Current Game State
	manager.Render();

#if CHECK_PERFORMANCE
	now = SDL_GetPerformanceCounter();
	++frameCounter;
	renderDuration = (double)((now - start) * 1000.0) / SDL_GetPerformanceFrequency();
	totalDuration += renderDuration;
	renderAverage = totalDuration / (double)frameCounter;

	sprintf(debugInformation, 
		"Update %.3f ms\nUpdate Av. %.3f ms\nRender %.3f ms\nRender Av. %.3f ms", 
		updateDuration, updateAverage, renderDuration, renderAverage);

	// Render Debug Text
	FC_DrawAlign(debugSmallFont, renderer,
		0,
		0,
		FC_ALIGN_LEFT,
		debugInformation);
#endif

	SDL_RenderPresent(renderer);
}

void Game::Clean() 
{ 
#if CHECK_PERFORMANCE
	FC_FreeFont(debugSmallFont);
#endif

	// Cleanup input
	Input::Instance().Clean();

	// Cleanup State Manager
	manager.Clean();

	// Cleanup Renderer
	if (renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}

	// Cleanup Window
	if (window != nullptr)
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	// Cleanup SDL Systems
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::Quit()
{
	isRunning = false;
}
