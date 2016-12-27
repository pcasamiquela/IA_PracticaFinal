/* ========================================================================
   File: SimpleInfluenceMapState.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "SimpleInfluenceMapState.h"
#include "StrategyUtils.h"

void SimpleInfluenceMapState::Init()
{
	// Initialize Fonts of different sizes
	std::string assetPath = PathUtils::GetResourcesPath("Fonts/nokiafc22.ttf");
	fontNormal = FC_CreateFont();
	FC_LoadFont(fontNormal, Game::Instance().renderer, assetPath.c_str(), 20, 
				FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
	fontSmall = FC_CreateFont();
	FC_LoadFont(fontSmall, Game::Instance().renderer, assetPath.c_str(), 22,
				FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
	fontSmallColor = FC_CreateFont();
	FC_LoadFont(fontSmallColor, Game::Instance().renderer, assetPath.c_str(), 22,
				FC_MakeColor(60, 60, 60, 255), TTF_STYLE_NORMAL);

	// Initialize InfluenceMap Texture
	influenceMapTexture = {};
	StaticTexture_Init(&influenceMapTexture, FIXED_WIDTH, FIXED_HEIGHT, Game::Instance().renderer);
	StaticTexture_StartDrawing(&influenceMapTexture);

	// Render Grid only once
	InfluenceMap_RenderMap(&influenceMap, Game::Instance().renderer, mapOrigin, Colors::MIDNIGHT_BLUE);
	StaticTexture_EndDrawing(&influenceMapTexture);

	// Initialize Entities
	player = new InfluencePlayer(FIXED_WIDTH / 2, FIXED_HEIGHT / 2, 32, 24);
	assetPath = PathUtils::GetResourcesPath("images/Boid.png");
	player->LoadGraphic(assetPath, "Boid", 32, 24, false);
	player->SetScale(GLOBAL_SCALE, GLOBAL_SCALE);
	player->Init(ALLIED_SOLDIER_TYPE);

	// Initialize Influence Map
	InfluenceMap_Init(&influenceMap, mapOrigin);
	momentumToUse = influenceMap.momentum;
	decayToUse = influenceMap.decay;
	InfluenceMap_AddInfluenceSource(&influenceMap, (InfluenceAgent*)player);
}

void SimpleInfluenceMapState::Clean()
{
	StaticTexture_Free(&influenceMapTexture);
	FC_FreeFont(fontNormal);
	FC_FreeFont(fontSmall);
	FC_FreeFont(fontSmallColor);
}

void SimpleInfluenceMapState::Update(float deltaTime)
{
	if (Input::Instance().GetKeyDown(KeyCode::Escape))
	{
		Game::Instance().ChangeState(MENU_STATE);
		return;
	}

	HandleInput();

	player->Update(deltaTime);

	// Update UI Text
	Vector2D currentPosition = Input::Instance().mousePosition;
	if (currentPosition.x > mapOrigin.x && currentPosition.x < mapOrigin.x + influenceMap.width * influenceMap.cellSize
		&& currentPosition.y > mapOrigin.y && currentPosition.y < mapOrigin.y + influenceMap.height * influenceMap.cellSize)
	{
		int pressX = (currentPosition.x - mapOrigin.x) / influenceMap.cellSize;
		int pressY = (currentPosition.y - mapOrigin.y) / influenceMap.cellSize;
		sprintf(cellInformation, "Influence = %.2f", influenceMap.frontBuffer[pressX + pressY * influenceMap.width]);
	}

	if (updateFrequency == 0)
	{
		sprintf(updateFrequencyInformation, "Each Frame [U-J]");
	}
	else
	{
		sprintf(updateFrequencyInformation, "%d Hz [U-J]", updateFrequency);
	}
	sprintf(momentumInformation, "%.1f [Y-H]", momentumToUse);
	sprintf(decayInformation, "%.3f [TRE-GFD]", decayToUse);

	//Pause management
	if (isPaused == true && runOneFrame == false)
	{
		return;
	}
	runOneFrame = false;

	// Update Influence if necessary
	Uint32 currentTime = SDL_GetTicks();
	if (updateFrequency == 0
		|| lastInfluenceUpdate < currentTime - (1000 / updateFrequency))
	{
		// Performance Code
		Uint64 start, now;
		double durationInMs;
		start = SDL_GetPerformanceCounter();

		InfluenceMap_UpdateInfluence(&influenceMap);
		InfluenceMap_PropagateInfluence(&influenceMap);

		// Performance Code
		now = SDL_GetPerformanceCounter();
		durationInMs = (double)((now - start) * 1000.0) / SDL_GetPerformanceFrequency();
		sprintf(performanceInformation, "%.6f ms", durationInMs);

		lastInfluenceUpdate = SDL_GetTicks();
	}
}

void SimpleInfluenceMapState::Render()
{
	StaticTexture_Render(&influenceMapTexture, Game::Instance().renderer, 0, 0);
	InfluenceMap_RenderInfluence(&influenceMap, Game::Instance().renderer, mapOrigin);

	if (isPaused == true)
	{
		FC_DrawAlign(fontSmallColor, Game::Instance().renderer,
					 FIXED_WIDTH,
					 0,
					 FC_ALIGN_RIGHT,
					 (char*)"Pause [SPACE]");
	}
	else
	{
		FC_DrawAlign(fontSmallColor, Game::Instance().renderer,
					 FIXED_WIDTH,
					 0,
					 FC_ALIGN_RIGHT,
					 (char*)"Play [SPACE]");
	}

	FC_DrawAlign(fontSmallColor, Game::Instance().renderer,
				 FIXED_WIDTH,
				 50.0f,
				 FC_ALIGN_RIGHT,
				 (char*)"Momentum:");

	FC_DrawAlign(fontSmall, Game::Instance().renderer,
				 FIXED_WIDTH,
				 50.0f + LINE_HEIGHT,
				 FC_ALIGN_RIGHT,
				 momentumInformation);

	FC_DrawAlign(fontSmallColor, Game::Instance().renderer,
				 FIXED_WIDTH,
				 150.0f,
				 FC_ALIGN_RIGHT,
				 (char*)"Decay:");

	FC_DrawAlign(fontSmall, Game::Instance().renderer,
				 FIXED_WIDTH,
				 150.0f + LINE_HEIGHT,
				 FC_ALIGN_RIGHT,
				 decayInformation);

	FC_DrawAlign(fontSmallColor, Game::Instance().renderer,
				 FIXED_WIDTH,
				 250.0f,
				 FC_ALIGN_RIGHT,
				 (char*)"Update Frequency:");

	FC_DrawAlign(fontSmall, Game::Instance().renderer,
				 FIXED_WIDTH,
				 250.0f + LINE_HEIGHT,
				 FC_ALIGN_RIGHT,
				 updateFrequencyInformation);

	FC_DrawAlign(fontSmallColor, Game::Instance().renderer,
				 FIXED_WIDTH,
				 550.0f,
				 FC_ALIGN_RIGHT,
				 (char*)"Performance =");

	FC_DrawAlign(fontSmall, Game::Instance().renderer,
				 FIXED_WIDTH,
				 550.0f + LINE_HEIGHT,
				 FC_ALIGN_RIGHT,
				 performanceInformation);

	FC_DrawAlign(fontSmall, Game::Instance().renderer,
				 20,
				 10,
				 FC_ALIGN_LEFT,
				 cellInformation);

	player->Render();
}

void SimpleInfluenceMapState::HandleInput()
{
	// keyboard Input Handling
	if (Input::Instance().GetKeyDown(KeyCode::Space))
	{
		isPaused = !isPaused;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::N))
	{
		if (isPaused == false)
		{
			isPaused = true;
		}
		else
		{
			runOneFrame = true;
		}
	}
	else if (Input::Instance().GetKeyDown(KeyCode::B))
	{
		InfluenceMap_ChangeBuffer(&influenceMap);
	}
	else if (Input::Instance().GetKeyDown(KeyCode::U))
	{
		updateFrequency -= 1;
		if (updateFrequency < 0)
		{
			updateFrequency = K_COUNT_UPDDATE_FREQUENCY - 1;
		}
	}
	else if (Input::Instance().GetKeyDown(KeyCode::J))
	{
		updateFrequency += 1;
		updateFrequency %= K_COUNT_UPDDATE_FREQUENCY;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::Y))
	{
		momentumToUse -= 0.1f;
		if (momentumToUse < 0.0f)
		{
			momentumToUse = 0.0f;
		}
		influenceMap.momentum = momentumToUse;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::H))
	{
		momentumToUse += 0.1f;
		if (momentumToUse > 0.9f)
		{
			momentumToUse = 0.9f;
		}
		influenceMap.momentum = momentumToUse;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::T))
	{
		decayToUse -= 0.1f;
		if (decayToUse < 0.0f)
		{
			decayToUse = 0.0f;
		}
		influenceMap.decay = decayToUse;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::G))
	{
		decayToUse += 0.1f;
		influenceMap.decay = decayToUse;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::R))
	{
		decayToUse -= 0.01f;
		if (decayToUse < 0.0f)
		{
			decayToUse = 0.0f;
		}
		influenceMap.decay = decayToUse;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::F))
	{
		decayToUse += 0.01f;
		influenceMap.decay = decayToUse;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::E))
	{
		decayToUse -= 0.001f;
		if (decayToUse < 0.0f)
		{
			decayToUse = 0.0f;
		}
		influenceMap.decay = decayToUse;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::D))
	{
		decayToUse += 0.001f;
		influenceMap.decay = decayToUse;
	}

	// Mouse Input Handling
	if (Input::Instance().GetMouseButton(0))
	{
		InsertAlliedUnit(Input::Instance().mousePosition);
	}

	if (Input::Instance().GetMouseButton(1))
	{
		InsertEnemyUnit(Input::Instance().mousePosition);
	}
}

void SimpleInfluenceMapState::InsertAlliedUnit(Vector2D position)
{
	int unitX = (position.x - mapOrigin.x) / influenceMap.cellSize;
	int unitY = (position.y - mapOrigin.y) / influenceMap.cellSize;
	StrategyUtils::AddAlliedSoldier(&influenceMap, unitX, unitY);
}

void SimpleInfluenceMapState::InsertEnemyUnit(Vector2D position)
{
	int unitX = (position.x - mapOrigin.x) / influenceMap.cellSize;
	int unitY = (position.y - mapOrigin.y) / influenceMap.cellSize;
	StrategyUtils::AddEnemySoldier(&influenceMap, unitX, unitY);
}
