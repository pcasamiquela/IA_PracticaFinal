/* ========================================================================
   File: AdvancedInfluenceMapState.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"
#include "InfluenceMapUtils.h"
#include "InfluencePlayer.h"
#include "InfluenceBoid.h"
#include "InfluenceMapUtils.h"

#define K_MAX_AGENTS 30

enum MapToShow
{
	BASIC_INFLUENCE,
	TENSION,
	TENSION_VARIATION,
	VULNERABILITY,
	VULNERABILITY_VARIATION,
	VULNERABILITY_VARIATION_2,
	MAPS_COUNT
};

class AdvancedInfluenceMapState : public BaseState
{
public:
	virtual void Init() override;
	virtual void Clean() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

private:
	// State Data

	// State Data
	InfluencePlayer* player;

	// Textures
	StaticTexture influenceMapTexture;

	// Influence Map
	// Influence Map
	Vector2D mapOrigin = {20.0f, 50.0f};
	InfluenceMap alliesMap;
	InfluenceMap enemiesMap;
	InfluenceBoid* agentArray[K_MAX_AGENTS];
	int agentArrayOccupation = 0;
	CalculatedMap basicInfluenceMap;
	CalculatedMap tensionMap;
	CalculatedMap tensionVariationMap;
	CalculatedMap vulnerabilityMap;
	CalculatedMap vulnerabilityVariationMap;
	CalculatedMap vulnerabilityVariation2Map;

	// UI vars
	MapToShow currentMap = BASIC_INFLUENCE;
	bool buildArchers = false;
	const int K_COUNT_UPDDATE_FREQUENCY = 6;
	float momentumToUse = 0.0f;
	float decayToUse = 0.0f;
	int updateFrequency = 0;
	Uint32 lastInfluenceUpdate = 0;
	bool runOneFrame = false;
	bool isPaused = false;

	// UI Text
	char cellInformation[256];
	char momentumInformation[128];
	char decayInformation[128];
	char updateFrequencyInformation[128];
	char performanceInformation[128];

	// Debug Fonts
	FC_Font* fontNormal;
	FC_Font* fontSmall;
	FC_Font* fontSmallColor;

	// Private methods
	void HandleInput();
	void CreateAgent(Vector2D position, UnitType agentType);
	void CreateTemplate(int i);
	void CleanInfluenceMap();
	void PrintInfluenceAgents();
};