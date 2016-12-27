/* ========================================================================
   File: EnemyAgent.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "EnemyAgent.h"

void EnemyAgent::Update(float deltaTime)
{
	// UPDATE of each state
	switch (currentState)
	{
	case PATROL_STATE:
	{
		++patrolCounter;

		if (patrolCounter > K_PATROL_FREQUENCY_COUNTER)
		{
			patrolCounter = 0;

			// Check if player inside vision ellipse
			if (TargetDetected())
			{
				ChangeState(PURSUE_STATE);
			}
		}
	}break;
	case PURSUE_STATE:
	{
		++losCounter;

		if (losCounter > K_LOS_FREQUENCY_COUNTER)
		{
			losCounter = 0;
			// Check if line of sight lost
			if (HasLOSWithTarget() == false)
			{
				ChangeState(AMUSED_STATE);
			}
		}

	}break;
	case AMUSED_STATE:
	{
		++amusedCounter;

		if (amusedCounter > K_AMUSED_COUNTER)
		{
			amusedCounter = 0;

			ChangeState(PATROL_STATE);
		}
	}break;
	default:
	{
		// Do nothing
	}break;
	}

	// Update base class
	Boid::Update(deltaTime);
}

void EnemyAgent::ChangeState(EnemyState state)
{
	// EXIT for OLD state
	switch (currentState)
	{
	case PATROL_STATE:
	{
		SDL_Log("StatePatrol_Exit");
	} break;
	case PURSUE_STATE:
	{
		SDL_Log("StatePursue_Exit");
		targetDetected = false;
	} break;
	case AMUSED_STATE:
	{
		SDL_Log("StateAmused_Exit");
		targetLost = false;
	} break;
	default:
	{
		// Do nothing
	}break;
	}

	// Change state
	currentState = state;

	// ENTER for NEW State
	switch (currentState)
	{
	case PATROL_STATE:
	{
		SDL_Log("StatePatrol_Enter");
		// Reset path following to 
		// pick up path at nearest point
		pathFollowingStarted = false;
		SetBehavior(Behavior::PATH_FOLLOWING);
	} break;
	case AMUSED_STATE:
	{
		SDL_Log("StateAmused_Enter");
		targetLost = true;
		SetBehavior(Behavior::NONE);
	} break;
	case PURSUE_STATE:
	{
		SDL_Log("StatePursue_Enter");
		SetBehavior(Behavior::ARRIVE);
		targetDetected = true;
	} break;
	default:
	{
		// Do nothing
	}break;
	}
}

void EnemyAgent::Render()
{
	// Render base class
	Boid::Render();
}

bool EnemyAgent::HasLOSWithTarget()
{
	// Check if there is line of sight between enemy and target (Player)
	losVector = *targetPosition;
	int totalObstacles = *losObstacleArraySize;
	for (int i = 0; i < totalObstacles; ++i)
	{
		// Check intersetcion for each obstacle
		if (LOS_Obstacle_IntersectsSegment(&losObstacleArray[i], position, losVector))
		{
			return false;
		}
	}
	return true;
}
