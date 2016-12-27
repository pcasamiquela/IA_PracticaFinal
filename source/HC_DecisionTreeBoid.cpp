/* ========================================================================
   File: HC_DecisionTreeBoid.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "HC_DecisionTreeBoid.h"
#include "GameController.h"

void HC_DecisionTreeBoid::Update(float deltaTime)
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
	RunDecisionTree();

	// Call Base Update
	AnimatedBoid::Update(deltaTime);
}

void HC_DecisionTreeBoid::Render()
{
	// Render nearby boid area
	filledCircleColor(Game::Instance().renderer, position.x, position.y,
		K_NEARBY_THRESHOLD, Colors::SetAlpha(Colors::ASBESTOS, 128));

	AnimatedBoid::Render();
}

void HC_DecisionTreeBoid::Clean()
{
	AnimatedBoid::Clean();
}

void HC_DecisionTreeBoid::RunDecisionTree()
{
	// Run Hard-Coded Decision Tree
	if (DecisionHealthOk())
	{
		if (DecisionEnemyNearby())
		{
			if (DecisionEnemyDangerous())
			{
				ActionCover();
			}
			else
			{
				ActionAttackEnemy();
			}
		}
		else
		{
			ActionCover();
		}
	}
	else
	{
		ActionSeekHealth();
	}
}

// Decision methods
bool HC_DecisionTreeBoid::DecisionEnemyNearby()
{
	return (Vector2D::Distance(GameController::Instance().player->position, position)
		< K_NEARBY_THRESHOLD);
}

#define K_HEALTH_OK 3
bool HC_DecisionTreeBoid::DecisionHealthOk()
{
	return (health > K_HEALTH_OK);
}

bool HC_DecisionTreeBoid::DecisionEnemyDangerous()
{
	return GameController::Instance().player->isDangerous;
}

// Action methods
void HC_DecisionTreeBoid::ActionSeekHealth()
{
	SetBehavior(Behavior::ARRIVE);
	targetPosition = &GameController::Instance().healthArea->center;
}

void HC_DecisionTreeBoid::ActionAttackEnemy()
{
	SetBehavior(Behavior::PURSUE);
	targetPosition = &GameController::Instance().player->position;
	targetSpeed = &GameController::Instance().player->speed;
}

void HC_DecisionTreeBoid::ActionCover()
{
	SetBehavior(Behavior::ARRIVE);
	targetPosition = &GameController::Instance().coverArea->center;
}
