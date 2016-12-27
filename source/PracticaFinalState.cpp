/* ========================================================================
   File: AdvancedInfluenceMapState.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "PracticaFinalState.h"
#include "StrategyUtils.h"

void AdvancedInfluenceMapState::Init()
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
	InfluenceMap_RenderMap(&alliesMap, Game::Instance().renderer, mapOrigin, Colors::MIDNIGHT_BLUE);
	StaticTexture_EndDrawing(&influenceMapTexture);

	// Initialize Entities
	player = new InfluencePlayer(FIXED_WIDTH / 2, FIXED_HEIGHT / 2, 32, 24);
	assetPath = PathUtils::GetResourcesPath("images/Boid.png");
	player->LoadGraphic(assetPath, "Boid", 32, 24, false);
	player->SetScale(GLOBAL_SCALE, GLOBAL_SCALE);
	player->Init(ALLIED_SOLDIER_TYPE);

	// Initialize Influence Maps
	InfluenceMap_Init(&alliesMap, mapOrigin);
	InfluenceMap_Init(&enemiesMap, mapOrigin);
	momentumToUse = alliesMap.momentum;
	decayToUse = alliesMap.decay;
	InfluenceMap_AddInfluenceSource(&alliesMap, (InfluenceAgent*)player);
}

void AdvancedInfluenceMapState::Clean()
{
	StaticTexture_Free(&influenceMapTexture);
	FC_FreeFont(fontNormal);
	FC_FreeFont(fontSmall);
	FC_FreeFont(fontSmallColor);
}

void AdvancedInfluenceMapState::Update(float deltaTime)
{
	if (Input::Instance().GetKeyDown(KeyCode::Escape))
	{
		Game::Instance().ChangeState(MENU_STATE);
		return;
	}

	HandleInput();

	player->Update(deltaTime);

	// Update UI Text
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

		InfluenceMap_UpdateInfluence(&alliesMap);
		InfluenceMap_PropagateInfluence(&alliesMap);
		InfluenceMap_UpdateInfluence(&enemiesMap);
		InfluenceMap_PropagateInfluence(&enemiesMap);

		Vector2D currentPosition = Input::Instance().mousePosition;

		switch (currentMap)
		{
			case MapToShow::BASIC_INFLUENCE:
				CalculatedMap_UpdateBasicInfluenceMap(&basicInfluenceMap, &alliesMap, &enemiesMap);
				if (currentPosition.x > mapOrigin.x && currentPosition.x < mapOrigin.x + alliesMap.width * alliesMap.cellSize
					&& currentPosition.y > mapOrigin.y && currentPosition.y < mapOrigin.y + alliesMap.height * alliesMap.cellSize)
				{
					int pressX = (currentPosition.x - mapOrigin.x) / alliesMap.cellSize;
					int pressY = (currentPosition.y - mapOrigin.y) / alliesMap.cellSize;
					sprintf(cellInformation, "Influence = %.2f", basicInfluenceMap.mapArray[pressX + pressY * alliesMap.width]);
				}
				break;
			case MapToShow::TENSION:
				CalculatedMap_UpdateTensionMap(&tensionMap, &alliesMap, &enemiesMap);
				if (currentPosition.x > mapOrigin.x && currentPosition.x < mapOrigin.x + alliesMap.width * alliesMap.cellSize
					&& currentPosition.y > mapOrigin.y && currentPosition.y < mapOrigin.y + alliesMap.height * alliesMap.cellSize)
				{
					int pressX = (currentPosition.x - mapOrigin.x) / alliesMap.cellSize;
					int pressY = (currentPosition.y - mapOrigin.y) / alliesMap.cellSize;
					sprintf(cellInformation, "Tension = %.2f", tensionMap.mapArray[pressX + pressY * alliesMap.width]);
				}
				break;
			case MapToShow::TENSION_VARIATION:
				CalculatedMap_UpdateTensionVariationMap(&tensionVariationMap, &alliesMap, &enemiesMap);
				if (currentPosition.x > mapOrigin.x && currentPosition.x < mapOrigin.x + alliesMap.width * alliesMap.cellSize
					&& currentPosition.y > mapOrigin.y && currentPosition.y < mapOrigin.y + alliesMap.height * alliesMap.cellSize)
				{
					int pressX = (currentPosition.x - mapOrigin.x) / alliesMap.cellSize;
					int pressY = (currentPosition.y - mapOrigin.y) / alliesMap.cellSize;
					sprintf(cellInformation, "Tension Variation = %.2f", tensionVariationMap.mapArray[pressX + pressY * alliesMap.width]);
				}
				break;
			case MapToShow::VULNERABILITY:
				CalculatedMap_UpdateVulnerabilityMap(&vulnerabilityMap, &alliesMap, &enemiesMap);
				if (currentPosition.x > mapOrigin.x && currentPosition.x < mapOrigin.x + alliesMap.width * alliesMap.cellSize
					&& currentPosition.y > mapOrigin.y && currentPosition.y < mapOrigin.y + alliesMap.height * alliesMap.cellSize)
				{
					int pressX = (currentPosition.x - mapOrigin.x) / alliesMap.cellSize;
					int pressY = (currentPosition.y - mapOrigin.y) / alliesMap.cellSize;
					sprintf(cellInformation, "Vulnerability = %.2f", vulnerabilityMap.mapArray[pressX + pressY * alliesMap.width]);
				}
				break;
			case MapToShow::VULNERABILITY_VARIATION:
				CalculatedMap_UpdateVulnerabilityVariationMap(&vulnerabilityVariationMap, &alliesMap, &enemiesMap);
				if (currentPosition.x > mapOrigin.x && currentPosition.x < mapOrigin.x + alliesMap.width * alliesMap.cellSize
					&& currentPosition.y > mapOrigin.y && currentPosition.y < mapOrigin.y + alliesMap.height * alliesMap.cellSize)
				{
					int pressX = (currentPosition.x - mapOrigin.x) / alliesMap.cellSize;
					int pressY = (currentPosition.y - mapOrigin.y) / alliesMap.cellSize;
					sprintf(cellInformation, "Vulnerability Variation = %.2f", vulnerabilityVariationMap.mapArray[pressX + pressY * alliesMap.width]);
				}
				break;
			case MapToShow::VULNERABILITY_VARIATION_2:
				CalculatedMap_UpdateVulnerabilityVariation2Map(&vulnerabilityVariation2Map, &alliesMap, &enemiesMap);
				if (currentPosition.x > mapOrigin.x && currentPosition.x < mapOrigin.x + alliesMap.width * alliesMap.cellSize
					&& currentPosition.y > mapOrigin.y && currentPosition.y < mapOrigin.y + alliesMap.height * alliesMap.cellSize)
				{
					int pressX = (currentPosition.x - mapOrigin.x) / alliesMap.cellSize;
					int pressY = (currentPosition.y - mapOrigin.y) / alliesMap.cellSize;
					sprintf(cellInformation, "Vulnerability Variation 2 = %.2f", vulnerabilityVariation2Map.mapArray[pressX + pressY * alliesMap.width]);
				}
				break;
		}

		// Performance Code
		now = SDL_GetPerformanceCounter();
		durationInMs = (double)((now - start) * 1000.0) / SDL_GetPerformanceFrequency();
		sprintf(performanceInformation, "%.6f ms", durationInMs);

		lastInfluenceUpdate = SDL_GetTicks();
	}
}

void AdvancedInfluenceMapState::Render()
{
	StaticTexture_Render(&influenceMapTexture, Game::Instance().renderer, 0, 0);

	switch (currentMap)
	{
		case MapToShow::BASIC_INFLUENCE:
			CalculatedMap_RenderInfluence(&basicInfluenceMap, Game::Instance().renderer, mapOrigin);
			break;
		case MapToShow::TENSION:
			CalculatedMap_RenderInfluence(&tensionMap, Game::Instance().renderer, mapOrigin);
			break;
		case MapToShow::TENSION_VARIATION:
			CalculatedMap_RenderInfluence(&tensionVariationMap, Game::Instance().renderer, mapOrigin);
			break;
		case MapToShow::VULNERABILITY:
			CalculatedMap_RenderInfluence(&vulnerabilityMap, Game::Instance().renderer, mapOrigin);
			break;
		case MapToShow::VULNERABILITY_VARIATION:
			CalculatedMap_RenderInfluence(&vulnerabilityVariationMap, Game::Instance().renderer, mapOrigin);
			break;
		case MapToShow::VULNERABILITY_VARIATION_2:
			CalculatedMap_RenderInfluence(&vulnerabilityVariation2Map, Game::Instance().renderer, mapOrigin);
			break;
	}

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
				 FIXED_HEIGHT - 2 * LINE_HEIGHT,
				 FC_ALIGN_RIGHT,
				 (char*)"Press [1-5] for Templates");

	FC_DrawAlign(fontSmallColor, Game::Instance().renderer,
				 FIXED_WIDTH,
				 FIXED_HEIGHT - LINE_HEIGHT,
				 FC_ALIGN_RIGHT,
				 (char*)"Press M to Reset");

	if (buildArchers)
	{
		FC_DrawAlign(fontSmallColor, Game::Instance().renderer,
					 FIXED_WIDTH,
					 350.0f,
					 FC_ALIGN_RIGHT,
					 (char*)"Building Archers [A]");
	}
	else
	{
		FC_DrawAlign(fontSmallColor, Game::Instance().renderer,
					 FIXED_WIDTH,
					 350.0f,
					 FC_ALIGN_RIGHT,
					 (char*)"Building Soldiers [A]");
	}

	char* mapInUseText = nullptr;
	switch (currentMap)
	{
		case MapToShow::BASIC_INFLUENCE:
			mapInUseText = "Basic Influence [C]";
			break;
		case MapToShow::TENSION:
			mapInUseText = "Tension [C]";
			break;
		case MapToShow::TENSION_VARIATION:
			mapInUseText = "Tension Variation [C]";
			break;
		case MapToShow::VULNERABILITY:
			mapInUseText = "Vulnerability [C]";
			break;
		case MapToShow::VULNERABILITY_VARIATION:
			mapInUseText = "Vulnerability Variation [C]";
			break;
		case MapToShow::VULNERABILITY_VARIATION_2:
			mapInUseText = "Vulnerability Variation 2 [C]";
			break;
	}

	FC_DrawAlign(fontSmallColor, Game::Instance().renderer,
				 FIXED_WIDTH,
				 400.0f,
				 FC_ALIGN_RIGHT,
				 mapInUseText);

	FC_DrawAlign(fontSmall, Game::Instance().renderer,
				 20,
				 10,
				 FC_ALIGN_LEFT,
				 cellInformation);

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

	for (int i = 0; i < agentArrayOccupation; ++i)
	{
		InfluenceBoid* currentInfluenceBoid = agentArray[i];
		currentInfluenceBoid->Render();
	}

	player->Render();

	switch (currentMap)
	{
		case MapToShow::BASIC_INFLUENCE:
			CalculatedMap_RenderInterestPoint(&basicInfluenceMap, Game::Instance().renderer, mapOrigin);
			break;
		case MapToShow::TENSION:
			CalculatedMap_RenderInterestPoint(&tensionMap, Game::Instance().renderer, mapOrigin);
			break;
		case MapToShow::TENSION_VARIATION:
			CalculatedMap_RenderInterestPoint(&tensionVariationMap, Game::Instance().renderer, mapOrigin);
			break;
		case MapToShow::VULNERABILITY:
			CalculatedMap_RenderInterestPoint(&vulnerabilityMap, Game::Instance().renderer, mapOrigin);
			break;
		case MapToShow::VULNERABILITY_VARIATION:
			CalculatedMap_RenderInterestPoint(&vulnerabilityVariationMap, Game::Instance().renderer, mapOrigin);
			break;
		case MapToShow::VULNERABILITY_VARIATION_2:
			CalculatedMap_RenderInterestPoint(&vulnerabilityVariation2Map, Game::Instance().renderer, mapOrigin);
			break;
	}
}

void AdvancedInfluenceMapState::HandleInput()
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
		alliesMap.momentum = momentumToUse;
		enemiesMap.momentum = momentumToUse;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::H))
	{
		momentumToUse += 0.1f;
		if (momentumToUse > 0.9f)
		{
			momentumToUse = 0.9f;
		}
		alliesMap.momentum = momentumToUse;
		enemiesMap.momentum = momentumToUse;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::T))
	{
		decayToUse -= 0.1f;
		if (decayToUse < 0.0f)
		{
			decayToUse = 0.0f;
		}
		alliesMap.decay = decayToUse;
		enemiesMap.decay = decayToUse;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::G))
	{
		decayToUse += 0.1f;
		alliesMap.decay = decayToUse;
		enemiesMap.decay = decayToUse;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::R))
	{
		decayToUse -= 0.01f;
		if (decayToUse < 0.0f)
		{
			decayToUse = 0.0f;
		}
		alliesMap.decay = decayToUse;
		enemiesMap.decay = decayToUse;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::F))
	{
		decayToUse += 0.01f;
		alliesMap.decay = decayToUse;
		enemiesMap.decay = decayToUse;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::E))
	{
		decayToUse -= 0.001f;
		if (decayToUse < 0.0f)
		{
			decayToUse = 0.0f;
		}
		alliesMap.decay = decayToUse;
		enemiesMap.decay = decayToUse;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::D))
	{
		decayToUse += 0.001f;
		alliesMap.decay = decayToUse;
		enemiesMap.decay = decayToUse;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::M))
	{
		CleanInfluenceMap();
	}
	else if (Input::Instance().GetKeyDown(KeyCode::A))
	{
		buildArchers = !buildArchers;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::C))
	{
		currentMap = (MapToShow)(((int)currentMap + 1) % (int)MapToShow::MAPS_COUNT);
	}
	else if (Input::Instance().GetKeyDown(KeyCode::P))
	{
		PrintInfluenceAgents();
	}
	else if (Input::Instance().GetKeyDown(KeyCode::Alpha1))
	{
		CreateTemplate(1);
	}
	else if (Input::Instance().GetKeyDown(KeyCode::Alpha2))
	{
		CreateTemplate(2);
	}
	else if (Input::Instance().GetKeyDown(KeyCode::Alpha3))
	{
		CreateTemplate(3);
	}
	else if (Input::Instance().GetKeyDown(KeyCode::Alpha4))
	{
		CreateTemplate(4);
	}
	else if (Input::Instance().GetKeyDown(KeyCode::Alpha5))
	{
		CreateTemplate(5);
	}

	// Mouse Input Handling
	if (Input::Instance().GetMouseButtonDown(0))
	{
		if (buildArchers)
		{
			CreateAgent(Input::Instance().mousePosition, ALLIED_ARCHER_TYPE);
		}
		else
		{
			CreateAgent(Input::Instance().mousePosition, ALLIED_SOLDIER_TYPE);
		}
	}

	if (Input::Instance().GetMouseButtonDown(1))
	{
		if (buildArchers)
		{
			CreateAgent(Input::Instance().mousePosition, ENEMY_ARCHER_TYPE);
		}
		else
		{
			CreateAgent(Input::Instance().mousePosition, ENEMY_SOLDIER_TYPE);
		}
	}
}

void AdvancedInfluenceMapState::CreateAgent(Vector2D position, UnitType agentType)
{
	if (agentArrayOccupation < K_MAX_AGENTS)
	{
		SDL_Log("Agent added");
		// Initialize Agent

		agentArray[agentArrayOccupation] = new InfluenceBoid((int)position.x, (int)position.y, 32, 24);
		std::string assetPath;
		if (agentType == ALLIED_SOLDIER_TYPE || agentType == ALLIED_ARCHER_TYPE)
		{
			assetPath = PathUtils::GetResourcesPath("images/Boid.png");
			agentArray[agentArrayOccupation]->LoadGraphic(assetPath, "AlliedBoid", 32, 24, false);
			agentArray[agentArrayOccupation]->SetScale(GLOBAL_SCALE, GLOBAL_SCALE);
			agentArray[agentArrayOccupation]->Init(agentType);
			InfluenceMap_AddInfluenceSource(&alliesMap, agentArray[agentArrayOccupation]);
		}
		else
		{
			assetPath = PathUtils::GetResourcesPath("images/Player.png");
			agentArray[agentArrayOccupation]->LoadGraphic(assetPath, "EnemyBoid", 32, 24, false);
			agentArray[agentArrayOccupation]->SetScale(GLOBAL_SCALE, GLOBAL_SCALE);
			agentArray[agentArrayOccupation]->Init(agentType);
			InfluenceMap_AddInfluenceSource(&enemiesMap, agentArray[agentArrayOccupation]);
		}
		
		++agentArrayOccupation;
	}
	else
	{
		SDL_Log("Cannot create more agents!");
	}
}

void AdvancedInfluenceMapState::CleanInfluenceMap()
{
	InfluenceMap_Clear(&alliesMap);
	InfluenceMap_Clear(&enemiesMap);
	agentArrayOccupation = 0;

	InfluenceMap_AddInfluenceSource(&alliesMap, (InfluenceAgent*)player);
}

void AdvancedInfluenceMapState::PrintInfluenceAgents()
{
	for (int i = 0; i < agentArrayOccupation; ++i)
	{
		if (agentArray[i]->unitType == ALLIED_SOLDIER_TYPE)
		{
			SDL_Log("CreateAgent(Vector2D(%f, %f), ALLIED_SOLDIER_TYPE);\n",
					agentArray[i]->position.x, agentArray[i]->position.y);
		}
		else if (agentArray[i]->unitType == ALLIED_ARCHER_TYPE)
		{
			SDL_Log("CreateAgent(Vector2D(%f, %f), ALLIED_ARCHER_TYPE);\n",
					agentArray[i]->position.x, agentArray[i]->position.y);
		}
		else if (agentArray[i]->unitType == ENEMY_SOLDIER_TYPE)
		{
			SDL_Log("CreateAgent(Vector2D(%f, %f), ENEMY_SOLDIER_TYPE);\n",
					agentArray[i]->position.x, agentArray[i]->position.y);
		}
		else if (agentArray[i]->unitType == ENEMY_ARCHER_TYPE)
		{
			SDL_Log("CreateAgent(Vector2D(%f, %f), ENEMY_ARCHER_TYPE);\n",
					agentArray[i]->position.x, agentArray[i]->position.y);
		}
	}
}

void AdvancedInfluenceMapState::CreateTemplate(int i)
{
	CleanInfluenceMap();

	if (i == 1)
	{
		CreateAgent(Vector2D(140.000000, 115.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(496.000000, 136.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(152.000000, 338.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(367.000000, 433.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(415.000000, 313.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(567.000000, 656.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(234.000000, 663.000000), ENEMY_SOLDIER_TYPE);
	}
	else if (i == 2)
	{
		CreateAgent(Vector2D(91.000000, 82.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(125.000000, 110.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(132.000000, 181.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(73.000000, 194.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(74.000000, 121.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(167.000000, 158.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(192.000000, 597.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(233.000000, 537.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(306.000000, 555.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(285.000000, 615.000000), ENEMY_SOLDIER_TYPE);
	}
	else if (i == 3)
	{
		CreateAgent(Vector2D(73.000000, 86.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(80.000000, 148.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(76.000000, 242.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(253.000000, 241.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(193.000000, 153.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(222.000000, 92.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(265.000000, 637.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(377.000000, 592.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(368.000000, 654.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(598.000000, 204.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(577.000000, 249.000000), ENEMY_SOLDIER_TYPE);
	}
	else if (i == 4)
	{
		CreateAgent(Vector2D(67.000000, 496.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(138.000000, 515.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(180.000000, 553.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(212.000000, 597.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(226.000000, 647.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(94.000000, 632.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(423.000000, 70.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(433.000000, 124.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(464.000000, 167.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(514.000000, 185.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(587.000000, 202.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(585.000000, 120.000000), ALLIED_SOLDIER_TYPE);
	}
	else if (i == 5)
	{
		CreateAgent(Vector2D(91.000000, 82.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(125.000000, 110.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(132.000000, 181.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(73.000000, 194.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(74.000000, 121.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(167.000000, 158.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(192.000000, 597.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(233.000000, 537.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(306.000000, 555.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(285.000000, 615.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(445.000000, 140.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(438.000000, 204.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(493.000000, 169.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(499.000000, 106.000000), ALLIED_SOLDIER_TYPE);
		CreateAgent(Vector2D(547.000000, 90.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(553.000000, 142.000000), ENEMY_SOLDIER_TYPE);
		CreateAgent(Vector2D(513.000000, 233.000000), ENEMY_SOLDIER_TYPE);
	}
}

