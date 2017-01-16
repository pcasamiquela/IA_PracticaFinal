/* ========================================================================
   File: EnemyAgent.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

#include "Boid.h"
#include "LOS_Obstacle.h"

enum EnemyState
{
	PATROL_STATE,
	PURSUE_STATE,
	AMUSED_STATE,
	PATROL_PATH_FINDING_STATE,
	SEARCH_LASTPOS_STATE
};

class EnemyAgent : public Boid
{
public:

	// Constructor
	EnemyAgent(int x, int y, int width, int height, Texture* questionTexture) :
		Boid(x, y, width, height),
		questionTexture {questionTexture}
	{}

	virtual void Update(float deltaTime) override;
	virtual void Render() override;

	virtual bool TargetDetected() = 0;
	virtual bool HasLOSWithTarget();
	bool simplePathStart = false;

	// Line of Sight
	LOS_Obstacle* losObstacleArray;
	int* losObstacleArraySize;
	Vector2D losVector;

protected:
	EnemyState currentState = PATROL_STATE;

	// Perception fields (Could go inside a blackboard)
	// Vision
	bool targetDetected = false;
	bool targetLost = false;

	// Graphics
	Texture* questionTexture;

private:
	// Constants
	static constexpr int K_PATROL_FREQUENCY_COUNTER = 5;
	static constexpr int K_LOS_FREQUENCY_COUNTER = 30;
	static constexpr int K_AMUSED_COUNTER = 90;

	// State Time counters
	int patrolCounter = 0;
	int losCounter = 0;
	int amusedCounter = 0;

	// Private functions
	void ChangeState(EnemyState state);
};
