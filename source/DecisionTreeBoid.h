/* ========================================================================
   File: DecisionTreeBoid.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "EngineFramework.h"
#include "AnimatedBoid.h"
#include "DecisionTreeRealTime.h"

class DecisionTreeBoid : public AnimatedBoid
{
public:
	DecisionTreeBoid(int x = 0, int y = 0, int width = 0, int height = 0) :
		AnimatedBoid(x, y, width, height)
	{
		moves = true;
		isActive = true;

		GenerateDecisionTree();
	};

	// Overriden functions
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Clean() override;

	// Entity Data (Public to allow debugging)
	int health = 5;
	RealTimeAction<DecisionTreeBoid>* currentAction;

private:
	// Entity Data
	static constexpr float K_HEALTH_DECREASE_TIME = 1.0f;
	static constexpr float K_NEARBY_THRESHOLD = 200.0f;

	// Entity Data
	float timeHealth = 0.0f;

	// Decision Indexes
	static constexpr int HEALTH_CHECK_DECISION = 0;
	static constexpr int ENEMY_NEARBY_DECISION = 1;
	static constexpr int ENEMY_DANGEROUS_DECISION = 2;

	// Action indexes
	static constexpr int SEEK_HEALTH_ACTION = 0;
	static constexpr int ATTACK_ACTION = 1;
	static constexpr int COVER_ACTION = 2;

	// Decision Tree
	RealTimeDecisionTree<DecisionTreeBoid>* decisionTree;

	// Private functions
	void GenerateDecisionTree();

	// Decision methods
	bool DecisionEnemyNearby();
	bool DecisionHealthOk();
	bool DecisionEnemyDangerous();

	// Action methods
	void ActionSeekHealth();
	void ActionAttackEnemy();
	void ActionCover();
};