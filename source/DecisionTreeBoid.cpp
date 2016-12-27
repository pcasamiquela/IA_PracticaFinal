/* ========================================================================
   File: DecisionTreeBoid.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "DecisionTreeBoid.h"
#include "GameController.h"

void DecisionTreeBoid::Update(float deltaTime)
{
	// Update Boid health
	if (Vector2D::Distance(position, 
		GameController::Instance().healthArea->center) > 
		GameController::Instance().healthArea->radius)
	{
		if (Vector2D::Distance(position, 
			GameController::Instance().coverArea->center) >
			GameController::Instance().coverArea->radius)
		{
			// Decrease health if not in Health Area and not inside Cover Area
			timeHealth += deltaTime;
			if (timeHealth > K_HEALTH_DECREASE_TIME)
			{
				health -= 1;
				timeHealth = 0.0f;
			}
		}
	}
	else
	{
		// Set health if inside Health Area
		health = 10;
	}

	// Run Tree
	decisionTree->RunTree();

	// Call Base Update
	AnimatedBoid::Update(deltaTime);
}

void DecisionTreeBoid::Render()
{
	// Render nearby boid area
	filledCircleColor(Game::Instance().renderer, position.x, position.y,
		K_NEARBY_THRESHOLD, Colors::SetAlpha(Colors::ASBESTOS, 128));

	AnimatedBoid::Render();
}

void DecisionTreeBoid::Clean()
{
	decisionTree->Clean();
	AnimatedBoid::Clean();
}

void DecisionTreeBoid::GenerateDecisionTree()
{
	// Set up the decision tree
	decisionTree = new RealTimeDecisionTree<DecisionTreeBoid>(3, 3);

	// Set up the decisions
	decisionTree->SetDecision(HEALTH_CHECK_DECISION,
		this, 
		&DecisionTreeBoid::DecisionHealthOk,
		"Q0. Is your health okay?",
		decisionTree->GetDecision(ENEMY_NEARBY_DECISION),
		decisionTree->GetAction(SEEK_HEALTH_ACTION));

	decisionTree->SetDecision(ENEMY_NEARBY_DECISION,
		this,
		&DecisionTreeBoid::DecisionEnemyNearby,
		"Q1.Can you see any enemies?",
		decisionTree->GetDecision(ENEMY_DANGEROUS_DECISION),
		decisionTree->GetAction(COVER_ACTION));

	decisionTree->SetDecision(ENEMY_DANGEROUS_DECISION,
		this,
		&DecisionTreeBoid::DecisionEnemyDangerous,
		"Q2. Is the enemy dangerous?",
		decisionTree->GetAction(COVER_ACTION),
		decisionTree->GetAction(ATTACK_ACTION));

	// Set up the actions
	decisionTree->SetAction(SEEK_HEALTH_ACTION,
		this,
		&DecisionTreeBoid::ActionSeekHealth,
		"A0. Seek out health packs.");

	decisionTree->SetAction(ATTACK_ACTION,
		this,
		&DecisionTreeBoid::ActionAttackEnemy,
		"A1. Attack enemy.");

	decisionTree->SetAction(COVER_ACTION,
		this,
		&DecisionTreeBoid::ActionCover,
		"A2. Head for cover.");

	// Set Root node
	decisionTree->SetRoot(decisionTree->GetDecision(HEALTH_CHECK_DECISION));

	// Initialize Current Action
	currentAction = decisionTree->GetAction(ATTACK_ACTION);
}

// Decision methods
bool DecisionTreeBoid::DecisionEnemyNearby()
{
	return (Vector2D::Distance(GameController::Instance().player->position, position)
		< K_NEARBY_THRESHOLD);
}

#define K_HEALTH_OK 3
bool DecisionTreeBoid::DecisionHealthOk()
{
	return (health > K_HEALTH_OK);
}

bool DecisionTreeBoid::DecisionEnemyDangerous()
{
	return GameController::Instance().player->isDangerous;
}

// Action methods
void DecisionTreeBoid::ActionSeekHealth()
{
	SetBehavior(Behavior::ARRIVE);
	targetPosition = &GameController::Instance().healthArea->center;

	// Set current action for debug purposes
	currentAction = decisionTree->GetAction(SEEK_HEALTH_ACTION);
}

void DecisionTreeBoid::ActionAttackEnemy()
{
	SetBehavior(Behavior::PURSUE);
	targetPosition = &GameController::Instance().player->position;
	targetSpeed = &GameController::Instance().player->speed;

	// Set current action for debug purposes
	currentAction = decisionTree->GetAction(ATTACK_ACTION);
}

void DecisionTreeBoid::ActionCover()
{
	SetBehavior(Behavior::ARRIVE);
	targetPosition = &GameController::Instance().coverArea->center;

	// Set current action for debug purposes
	currentAction = decisionTree->GetAction(COVER_ACTION);
}
