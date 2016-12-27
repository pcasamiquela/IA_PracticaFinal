/* ========================================================================
   File: SimpleInfluenceMapState.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"
#include "InfluenceMapUtils.h"
#include "InfluencePlayer.h"

class SimpleInfluenceMapState : public BaseState
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
	InfluenceMap influenceMap;
	Vector2D mapOrigin = {20.0f, 50.0f};

	// UI vars
	const int K_COUNT_UPDDATE_FREQUENCY = 6;
	Uint32 lastInfluenceUpdate = 0;
	Uint32 lastUpdate = 0;
	Uint32 timePerUpdate = 0;
	float momentumToUse = 0.0f;
	float decayToUse = 0.0f;
	int updateFrequency = 0;
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
	void InsertAlliedUnit(Vector2D position);
	void InsertEnemyUnit(Vector2D position);
};