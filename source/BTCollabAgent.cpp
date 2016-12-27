/* ========================================================================
   File: BTCollabAgent.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "BTCollabAgent.h"
#include "SimpleBlackboard.h"

// BT Setup
void BTCollabAgent::InitBehaviorTree()
{
	LeafNode<BTCollabAgent>* patrolAction = new LeafNode<BTCollabAgent>(this,
		&BTCollabAgent::InitPatrolAction,
		&BTCollabAgent::UpdatePatrolAction,
		&BTCollabAgent::TerminatePatrolAction);
	LeafNode<BTCollabAgent>* followAction = new LeafNode<BTCollabAgent>(this,
		&BTCollabAgent::InitFollowAction,
		&BTCollabAgent::UpdateFollowAction,
		&BTCollabAgent::TerminateFollowAction);
	LeafNode<BTCollabAgent>* continuousFollowAction = new LeafNode<BTCollabAgent>(this,
		&BTCollabAgent::InitContinuousFollowAction,
		&BTCollabAgent::UpdateContinuousFollowAction,
		&BTCollabAgent::TerminateContinuousFollowAction);
	LeafNode<BTCollabAgent>* questionAction = new LeafNode<BTCollabAgent>(this,
		&BTCollabAgent::InitQuestionAction,
		&BTCollabAgent::UpdateQuestionAction,
		&BTCollabAgent::TerminateQuestionAction);
	LeafNode<BTCollabAgent>* isFollowingCondition = new LeafNode<BTCollabAgent>(this,
		nullptr,
		&BTCollabAgent::UpdateIsFollowingCondition,
		nullptr);
	LeafNode<BTCollabAgent>* isNotLOSCondition = new LeafNode<BTCollabAgent>(this,
		nullptr,
		&BTCollabAgent::UpdateIsNotLOSCondition,
		nullptr);
	LeafNode<BTCollabAgent>* isVisibleCondition = new LeafNode<BTCollabAgent>(this,
		nullptr,
		&BTCollabAgent::UpdateIsVisibleCondition,
		nullptr);
	LeafNode<BTCollabAgent>* isAlertCondition = new LeafNode<BTCollabAgent>(this,
		nullptr,
		&BTCollabAgent::UpdateIsAlertCondition,
		nullptr);

	Selector* selShouldFollow = new Selector;
	selShouldFollow->addChild(isAlertCondition);
	selShouldFollow->addChild(isVisibleCondition);

	Sequence* seqNotFollowing = new Sequence;
	seqNotFollowing->addChild(isNotLOSCondition);
	seqNotFollowing->addChild(questionAction);

	Selector* selectorFollow = new Selector;
	selectorFollow->addChild(seqNotFollowing);
	selectorFollow->addChild(followAction);

	Sequence* variantSeq2 = new Sequence;
	variantSeq2->addChild(isFollowingCondition);
	variantSeq2->addChild(selectorFollow);

	Monitor* monitorInCone = new Monitor;
	monitorInCone->addCondition(selShouldFollow);
	monitorInCone->addAction(continuousFollowAction);

	ActiveSelector* activeHead = new ActiveSelector;
	activeHead->addChild(variantSeq2);
	activeHead->addChild(monitorInCone);
	activeHead->addChild(patrolAction);

	// Create a behavior tree
	bt = new BehaviorTree(activeHead);
}

// Behavior Actions
void BTCollabAgent::InitPatrolAction()
{
	SDL_Log("ID %d: Init Patrol Action", agentID);
}

Status BTCollabAgent::UpdatePatrolAction()
{
	//SDL_Log("ID %d: Update Patrol Action", agentID);
	SetBehavior(Behavior::PATH_FOLLOWING);
	return Status::BH_RUNNING;
}

void BTCollabAgent::TerminatePatrolAction()
{
	SDL_Log("ID %d: Terminate Patrol Action", agentID);
}

void BTCollabAgent::InitFollowAction()
{
	SDL_Log("ID %d: Init Follow Action", agentID);
	SetBehavior(Behavior::ARRIVE);
	targetDetected = true;
	// Write to Blackboard
	SimpleBlackboard::Instance().SetPlayerDetected(agentID, true);
}

Status BTCollabAgent::UpdateFollowAction()
{
	//SDL_Log("Follow Action");
	return Status::BH_SUCCESS;
}

void BTCollabAgent::TerminateFollowAction()
{
	SDL_Log("ID %d: Terminate Follow Action", agentID);
}

void BTCollabAgent::InitContinuousFollowAction()
{
	//SDL_Log("Init Continuous Follow Action");
	SetBehavior(Behavior::ARRIVE);
	targetDetected = true;
}

Status BTCollabAgent::UpdateContinuousFollowAction()
{
	//SDL_Log("Continuous Follow Action");
	return Status::BH_RUNNING;
}

void BTCollabAgent::TerminateContinuousFollowAction()
{
	//SDL_Log("Terminate Continuous Follow Action");
}

void BTCollabAgent::InitQuestionAction()
{
	SDL_Log("ID %d: Init Question Action", agentID);
	SetBehavior(Behavior::NONE);
	targetDetected = false;
	// Write to Blackboard
	SimpleBlackboard::Instance().SetPlayerDetected(agentID, false);
}

Status BTCollabAgent::UpdateQuestionAction()
{
	//SDL_Log("Question Action");
	++amusedCounter;

	if (amusedCounter > K_AMUSED_COUNTER)
	{
		return Status::BH_SUCCESS;
	}
	else
	{
		targetLost = true;
		return Status::BH_RUNNING;
	}
}

void BTCollabAgent::TerminateQuestionAction()
{
	SDL_Log("ID %d: Terminate Question Action", agentID);
	amusedCounter = 0;
	targetLost = false;
	pathFollowingStarted = false;
}

Status BTCollabAgent::UpdateIsFollowingCondition()
{
	if (targetDetected == true)
	{
		return Status::BH_SUCCESS;
	}
	else
	{
		return Status::BH_FAILURE;
	}
}

Status BTCollabAgent::UpdateIsNotLOSCondition()
{
	if (HasLOSWithTarget() == false)
	{
		return Status::BH_SUCCESS;
	}
	else
	{
		return Status::BH_FAILURE;
	}
}

Status BTCollabAgent::UpdateIsVisibleCondition()
{
	//SDL_Log("UpdateIsVisibleCondition");

	// Check if player inside vision cone
	Vector2D coneBasePoint = { position.x + visionConeHeight, position.y };
	Vector2D currentConeBase = Vector2DUtils::RotatePoint(position, coneBasePoint, angle);
	if (Vector2DUtils::IsInsideCone(*targetPosition, position, currentConeBase, visionConeHalfAngle))
	{
		if (HasLOSWithTarget())
		{
			return Status::BH_SUCCESS;
		}

		return Status::BH_FAILURE;
	}
	else
	{
		return Status::BH_FAILURE;
	}
}

Status BTCollabAgent::UpdateIsAlertCondition()
{
	if (SimpleBlackboard::Instance().IsAnyAgentFollowingPlayer() == true)
	{
		return Status::BH_SUCCESS;
	}
	else
	{
		return Status::BH_FAILURE;
	}
}

