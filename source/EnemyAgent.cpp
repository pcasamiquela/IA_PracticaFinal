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
		SetCollidesFlag(true);
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
		if (TargetDetected())
		{
			amusedCounter = 0;
			ChangeState(PURSUE_STATE);
		}
		if (amusedCounter > K_AMUSED_COUNTER)
		{
			amusedCounter = 0;
			currentSegment = 0;
			ChangeState(SEARCH_LASTPOS_STATE);
		}
	}break;

	case  SEARCH_LASTPOS_STATE:
	{
		if (TargetDetected())
		{
			ChangeState(PURSUE_STATE);
		}
		if (position.x / 32 <= targetpathPosition.x / 32 + 1 && position.x / 32 >= targetpathPosition.x / 32 - 1 &&
			position.y / 32 <= targetpathPosition.y / 32 + 1 && position.y / 32 >= targetpathPosition.y / 32 - 1) {
			int cellX = targetpathPosition.x / 32;
			int cellY = targetpathPosition.y / 32;
			position.x = cellX * 32 + 16;
			position.y = cellY * 32 + 16;
			ChangeState(WANDER_SEARCH_STATE);
		}

	}break;

	case WANDER_SEARCH_STATE:
	{
		if (TargetDetected())
		{
			wanderCounter = 0;
			ChangeState(PURSUE_STATE);

		}
		wanderCounter++;
		if (wanderCounter > K_WANDER_COUNTER) {
			wanderCounter = 0;
			ChangeState(PATROL_PATH_FINDING_STATE);
		}


	}break;

	case PATROL_PATH_FINDING_STATE:
	{
		if (TargetDetected())
		{
			ChangeState(PURSUE_STATE);
		}
		if (position.x/32 <= targetpathPosition.x/32 + 1 && position.x/32 >= targetpathPosition.x/32 - 1 &&
			position.y/32 <= targetpathPosition.y/32 + 1 && position.y/32 >= targetpathPosition.y/32 - 1) {
			simplePath = patrolPath;
			currentSegment = 0;
			ChangeState(PATROL_STATE);
		}
	}break;
	
	default:
	{
		// Do nothing
	}break;
	}

	// Update base class
	AnimatedBoid::Update(deltaTime);
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
		SetBehavior(Behavior::SIMPLE_PATH_FOLLOWING);
		simplePathStart = false;
		SetCollidesFlag(false);
	} break;
	case AMUSED_STATE:
	{
		SDL_Log("StateAmused_Enter");
		targetLost = true;
		SetBehavior(Behavior::NONE);
		simplePathStart = false;
	} break;
	case PURSUE_STATE:
	{
		SDL_Log("StatePursue_Enter");
		SetBehavior(Behavior::ARRIVE);
		targetDetected = true;
		simplePathStart = false;
		SetCollidesFlag(true);
	} break;
	case SEARCH_LASTPOS_STATE:
	{
		SDL_Log("State_Search_LastPos_Enter");
		SetBehavior(Behavior::SIMPLE_PATH_FOLLOWING);
		targetpathPosition = *targetPosition;
		targetDetected = false;
		simplePathStart = true;
		SetCollidesFlag(false);
	}break;
	case PATROL_PATH_FINDING_STATE:
	{
		SetBehavior(Behavior::SIMPLE_PATH_FOLLOWING);
		targetpathPosition = patrolPath->pathArray[0];
		targetDetected = false;
		simplePathStart = true;
		SetCollidesFlag(false);
	}break;
	case WANDER_SEARCH_STATE:
	{
		SetBehavior(Behavior::WANDER);
		targetDetected = false;
		simplePathStart = false;
		SetCollidesFlag(true);
	}break;
	default:
	{
		// Do nothing
	}break;
	}
}

void EnemyAgent::Render()
{
	// Render base class
	AnimatedBoid::Render();
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
