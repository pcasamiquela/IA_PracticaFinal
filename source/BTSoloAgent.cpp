/* ========================================================================
   File: BTSoloAgent.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "BTSoloAgent.h"

// BT Setup
void BTSoloAgent::InitBehaviorTree()
{
	LeafNode<BTSoloAgent>* patrolAction = new LeafNode<BTSoloAgent>(this,
		&BTSoloAgent::InitPatrolAction,
		&BTSoloAgent::UpdatePatrolAction,
		&BTSoloAgent::TerminatePatrolAction);
	LeafNode<BTSoloAgent>* followAction = new LeafNode<BTSoloAgent>(this,
		&BTSoloAgent::InitFollowAction,
		&BTSoloAgent::UpdateFollowAction,
		&BTSoloAgent::TerminateFollowAction);
	LeafNode<BTSoloAgent>* continuousFollowAction = new LeafNode<BTSoloAgent>(this,
		&BTSoloAgent::InitContinuousFollowAction,
		&BTSoloAgent::UpdateContinuousFollowAction,
		&BTSoloAgent::TerminateContinuousFollowAction);
	LeafNode<BTSoloAgent>* questionAction = new LeafNode<BTSoloAgent>(this,
		&BTSoloAgent::InitQuestionAction,
		&BTSoloAgent::UpdateQuestionAction,
		&BTSoloAgent::TerminateQuestionAction);
	LeafNode<BTSoloAgent>* isFollowingCondition = new LeafNode<BTSoloAgent>(this,
		nullptr,
		&BTSoloAgent::UpdateIsFollowingCondition,
		nullptr);
	LeafNode<BTSoloAgent>* isNotLOSCondition = new LeafNode<BTSoloAgent>(this,
		nullptr,
		&BTSoloAgent::UpdateIsNotLOSCondition,
		nullptr);
	LeafNode<BTSoloAgent>* isVisibleCondition = new LeafNode<BTSoloAgent>(this,
		nullptr,
		&BTSoloAgent::UpdateIsVisibleCondition,
		nullptr);

	Sequence* seqNotFollowing = new Sequence;
	seqNotFollowing->addChild(isNotLOSCondition);
	seqNotFollowing->addChild(questionAction);

	Selector* selectorFollow = new Selector;
	selectorFollow->addChild(seqNotFollowing);
	selectorFollow->addChild(followAction);

	Sequence* variantSeq2 = new Sequence;
	variantSeq2->addChild(isFollowingCondition);
	variantSeq2->addChild(selectorFollow);

	// Changed Parallel for Sequence
	Monitor* monitorInCone = new Monitor;
	monitorInCone->addCondition(isVisibleCondition);
	monitorInCone->addAction(continuousFollowAction);

	ActiveSelector* activeHead = new ActiveSelector;
	activeHead->addChild(variantSeq2);
	activeHead->addChild(monitorInCone);
	activeHead->addChild(patrolAction);

	// Create a behavior tree
	bt = new BehaviorTree(activeHead);
}


// Behavior Actions
void BTSoloAgent::InitPatrolAction()
{
	SDL_Log("Init Patrol Action");
}

Status BTSoloAgent::UpdatePatrolAction()
{
	SDL_Log("Update Patrol Action");
	SetBehavior(Behavior::PATH_FOLLOWING);
	return Status::BH_RUNNING;
}

void BTSoloAgent::TerminatePatrolAction()
{
	SDL_Log("Terminate Patrol Action");
}

void BTSoloAgent::InitFollowAction()
{
	//SDL_Log("Init Follow Action");
	SetBehavior(Behavior::ARRIVE);
	targetDetected = true;
}

Status BTSoloAgent::UpdateFollowAction()
{
	//SDL_Log("Follow Action");
	return Status::BH_SUCCESS;
}

void BTSoloAgent::TerminateFollowAction()
{
	//SDL_Log("Terminate Follow Action");
}

void BTSoloAgent::InitContinuousFollowAction()
{
	//SDL_Log("Init Continuous Follow Action");
	SetBehavior(Behavior::ARRIVE);
	targetDetected = true;
}

Status BTSoloAgent::UpdateContinuousFollowAction()
{
	//SDL_Log("Continuous Follow Action");
	return Status::BH_RUNNING;
}

void BTSoloAgent::TerminateContinuousFollowAction()
{
	//SDL_Log("Terminate Continuous Follow Action");
}

void BTSoloAgent::InitQuestionAction()
{
	//SDL_Log("Init Question Action");
	SetBehavior(Behavior::NONE);
	targetDetected = false;
}

Status BTSoloAgent::UpdateQuestionAction()
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

void BTSoloAgent::TerminateQuestionAction()
{
	//SDL_Log("Terminate Question Action");
	amusedCounter = 0;
	targetLost = false;
	pathFollowingStarted = false;
}

Status BTSoloAgent::UpdateIsFollowingCondition()
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

Status BTSoloAgent::UpdateIsNotLOSCondition()
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

Status BTSoloAgent::UpdateIsVisibleCondition()
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
