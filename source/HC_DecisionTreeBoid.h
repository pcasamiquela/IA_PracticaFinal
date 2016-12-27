/* ========================================================================
   File: HC_DecisionTreeBoid.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "EngineFramework.h"
#include "AnimatedBoid.h"

class HC_DecisionTreeBoid : public AnimatedBoid
{
public:
	HC_DecisionTreeBoid(int x = 0, int y = 0, int width = 0, int height = 0) :
		AnimatedBoid(x, y, width, height)
	{
		moves = true;
		isActive = true;

		// Set scale to differentiate it from the non-hardcoded one
		SetScale(1.0f, 1.0f);
	};

	// Overriden functions
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Clean() override;

	// Entity Data (Public to allow debugging)
	int health = 5;

private:
	// Entity Data
	static constexpr float K_HEALTH_DECREASE_TIME = 1.0f;
	static constexpr float K_NEARBY_THRESHOLD = 200.0f;

	float timeHealth = 0.0f;

	// Private functions
	void RunDecisionTree();

	// Decision methods
	bool DecisionEnemyNearby();
	bool DecisionHealthOk();
	bool DecisionEnemyDangerous();

	// Action methods
	void ActionSeekHealth();
	void ActionAttackEnemy();
	void ActionCover();
};