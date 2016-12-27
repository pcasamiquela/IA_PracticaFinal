/* ========================================================================
   File: InfluenceMapUtils.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"
#include <list>
#include "InfluenceAgent.h"

#define MAP_WIDTH 40 // Cell number
#define MAP_HEIGHT 40 // Cell number
#define MAP_SIZE 16 // Cell size

struct InfluenceMap
{
	// Data Storage
	float firstBufferStorage[MAP_WIDTH * MAP_HEIGHT] = { 0.0f };
	float secondBufferStorage[MAP_WIDTH * MAP_HEIGHT] = { 0.0f };

	// Double buffering
	float* frontBuffer = nullptr;
	float* backBuffer = nullptr;

	// Parameters
	Vector2D mapOrigin = { 0.0f, 0.0f };
	int width = MAP_WIDTH;
	int height = MAP_HEIGHT;
	int cellSize = MAP_SIZE;

	// Influence map parameters
	float momentum = 0.5f;
	float decay = 0.01f;

	// Influence Sources
	std::list<InfluenceAgent*> influenceAgentList;
};

// Init / Clear
void InfluenceMap_Init(InfluenceMap* map, Vector2D mapOrigin);
void InfluenceMap_Clear(InfluenceMap* map);

// Add Sources methos
void InfluenceMap_AddInfluenceSource(InfluenceMap* map, InfluenceAgent* influenceAgent);
void InfluenceMap_DeleteInfluenceSource(InfluenceMap* map, InfluenceAgent* influenceAgent);

// Add Influence methods
float InfluenceMap_CalculateNeighborsInfluence(InfluenceMap* map, int x, int y);
float InfluenceMap_GetInfluence(InfluenceMap* map, int x, int y);
void InfluenceMap_SetInfluence(InfluenceMap* map, int x, int y, float influence);
void InfluenceMap_AddInfluence(InfluenceMap* map, int x, int y, float influence);

// Update methods
void InfluenceMap_UpdateInfluence(InfluenceMap* map);
void InfluenceMap_PropagateInfluence(InfluenceMap* map);
void InfluenceMap_ChangeBuffer(InfluenceMap* map);

// Render methods
void InfluenceMap_RenderMap(InfluenceMap* map, SDL_Renderer* rendeder, Vector2D origin, Uint32 mapColor);
void InfluenceMap_RenderInfluence(InfluenceMap* map, SDL_Renderer* renderer, Vector2D gridOrigin);

// helper methods
Vector2D InfluenceMap_GetMapPositionFromWorldPosition(InfluenceMap* map, Vector2D worldPosition);

struct CalculatedMap
{
	int width = MAP_WIDTH;
	int height = MAP_HEIGHT;
	int cellSize = MAP_SIZE;
	float mapArray[MAP_WIDTH * MAP_HEIGHT] = { 0.0f };
	Vector2D maxPoint = { 0.0f, 0.0f };
	Vector2D minPoint = { 0.0f, 0.0f };
};

void CalculatedMap_UpdateBasicInfluenceMap(CalculatedMap* destination,
	InfluenceMap* firstSource, InfluenceMap* secondSource);
void CalculatedMap_UpdateTensionMap(CalculatedMap* destination,
	InfluenceMap* firstSource, InfluenceMap* secondSource);
void CalculatedMap_UpdateTensionVariationMap(CalculatedMap* destination,
	InfluenceMap* firstSource, InfluenceMap* secondSource);
void CalculatedMap_UpdateVulnerabilityMap(CalculatedMap* destination,
	InfluenceMap* firstSource, InfluenceMap* secondSource);
void CalculatedMap_UpdateVulnerabilityVariationMap(CalculatedMap* destination,
	InfluenceMap* firstSource, InfluenceMap* secondSource);
void CalculatedMap_UpdateVulnerabilityVariation2Map(CalculatedMap* destination,
	InfluenceMap* firstSource, InfluenceMap* secondSource);
void CalculatedMap_RenderInfluence(CalculatedMap* map, SDL_Renderer* renderer, Vector2D gridOrigin);
void CalculatedMap_RenderInterestPoint(CalculatedMap* map, SDL_Renderer* renderer, Vector2D gridOrigin);




