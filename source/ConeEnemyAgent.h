/* ========================================================================
   File: ConeEnemyAgent.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"
#include "EnemyAgent.h"

class ConeEnemyAgent : public EnemyAgent
{
public:
	// Constructor
	ConeEnemyAgent(int x, int y, int width, int height, Texture* questionTexture) :
		EnemyAgent(x, y, width, height, questionTexture)
	{}

	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual bool TargetDetected();
	void Setup() override;
	SimplePath *tempPath;
private:
	// Perception fields
	// Vision
	float visionConeHeight = 200.0f;
	//float visionConeHeight = 300.0f;
	float visionConeHalfAngle = 40.0f;

	void RenderVisionCone(Uint32 coneColor);
	void RenderLOS(Uint32 color);
};
