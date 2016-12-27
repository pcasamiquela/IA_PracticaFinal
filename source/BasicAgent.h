/* ========================================================================
   File: BasicAgent.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

#include "Boid.h"
#include "LOS_Obstacle.h"

class BasicAgent : public Boid
{
public:

	// Constructor
	BasicAgent(int x, int y, int width, int height, Texture* questionTexture) :
		Boid(x, y, width, height),
		questionTexture{ questionTexture }
	{}

	virtual void Render() override;

	// Line of Sight
	LOS_Obstacle* losObstacleArray;
	int* losObstacleArraySize;
	Vector2D losVector;

	// Graphics
	Texture* questionTexture;

protected:
	// Perception fields (Could go inside a blackboard)
	// Vision
	bool targetDetected = false;
	bool targetLost = false;

	// Perception fields
	// Vision
	float visionConeHeight = 160.0f;
	//float visionConeHeight = 300.0f;
	float visionConeHalfAngle = 40.0f;

	// Constants
	static constexpr int K_PATROL_FREQUENCY_COUNTER = 5;
	static constexpr int K_LOS_FREQUENCY_COUNTER = 30;
	static constexpr int K_AMUSED_COUNTER = 90;

	// State Time counters
	int patrolCounter = 0;
	int losCounter = 0;
	int amusedCounter = 0;

	// Perception functions
	bool TargetDetected();
	bool HasLOSWithTarget();

private:
	void RenderVisionCone(Uint32 coneColor);
	void RenderLOS(Uint32 color);
};