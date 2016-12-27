/* ========================================================================
   File: BTSimpleSoloAgent.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "BTSimpleSoloAgent.h"

// BT Setup
void BTSimpleSoloAgent::InitBehaviorTree()
{
	LeafNode<BTSimpleSoloAgent>* patrolAction = new LeafNode<BTSimpleSoloAgent>(this,
		&BTSimpleSoloAgent::InitPatrolAction,
		&BTSimpleSoloAgent::UpdatePatrolAction,
		&BTSimpleSoloAgent::TerminatePatrolAction);
	LeafNode<BTSimpleSoloAgent>* followAction = new LeafNode<BTSimpleSoloAgent>(this,
		&BTSimpleSoloAgent::InitFollowAction,
		&BTSimpleSoloAgent::UpdateFollowAction,
		&BTSimpleSoloAgent::TerminateFollowAction);
	LeafNode<BTSimpleSoloAgent>* questionAction = new LeafNode<BTSimpleSoloAgent>(this,
		&BTSimpleSoloAgent::InitQuestionAction,
		&BTSimpleSoloAgent::UpdateQuestionAction,
		&BTSimpleSoloAgent::TerminateQuestionAction);
	LeafNode<BTSimpleSoloAgent>* isFollowingCondition = new LeafNode<BTSimpleSoloAgent>(this,
		nullptr,
		&BTSimpleSoloAgent::UpdateIsFollowingCondition,
		nullptr);
	LeafNode<BTSimpleSoloAgent>* isNotLOSCondition = new LeafNode<BTSimpleSoloAgent>(this,
		nullptr,
		&BTSimpleSoloAgent::UpdateIsNotLOSCondition,
		nullptr);
	LeafNode<BTSimpleSoloAgent>* isVisibleCondition = new LeafNode<BTSimpleSoloAgent>(this,
		nullptr,
		&BTSimpleSoloAgent::UpdateIsVisibleCondition,
		nullptr);

	Sequence* seqNotFollowing = new Sequence;
	seqNotFollowing->addChild(isNotLOSCondition);
	seqNotFollowing->addChild(questionAction);

	Selector* selectorFollow = new Selector;
	selectorFollow->addChild(seqNotFollowing);
	selectorFollow->addChild(followAction);

	Sequence* seqInCone = new Sequence;
	seqInCone->addChild(isVisibleCondition);
	seqInCone->addChild(followAction);

	Sequence* seq2 = new Sequence;
	seq2->addChild(isFollowingCondition);
	seq2->addChild(selectorFollow);

	Selector* head = new Selector;
	head->addChild(seq2);
	head->addChild(seqInCone);
	head->addChild(patrolAction);

	// Create a behavior tree
	bt = new BehaviorTree(head);
}

// Behavior Actions
void BTSimpleSoloAgent::InitPatrolAction()
{
	//SDL_Log("Init Patrol Action");
}

Status BTSimpleSoloAgent::UpdatePatrolAction()
{
	//SDL_Log("Update Patrol Action");
	SetBehavior(Behavior::PATH_FOLLOWING);
	return Status::BH_SUCCESS;
}

void BTSimpleSoloAgent::TerminatePatrolAction()
{
	//SDL_Log("Terminate Patrol Action");
}

void BTSimpleSoloAgent::InitFollowAction()
{
	//SDL_Log("Init Follow Action");
	SetBehavior(Behavior::ARRIVE);
	targetDetected = true;
}

Status BTSimpleSoloAgent::UpdateFollowAction()
{
	//SDL_Log("Follow Action");
	return Status::BH_SUCCESS;
}

void BTSimpleSoloAgent::TerminateFollowAction()
{
	//SDL_Log("Terminate Follow Action");
}

void BTSimpleSoloAgent::InitQuestionAction()
{
	//SDL_Log("Init Question Action");
	SetBehavior(Behavior::NONE);
	targetDetected = false;
}

Status BTSimpleSoloAgent::UpdateQuestionAction()
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

void BTSimpleSoloAgent::TerminateQuestionAction()
{
	//SDL_Log("Terminate Question Action");
	amusedCounter = 0;
	targetLost = false;
	pathFollowingStarted = false;
}

Status BTSimpleSoloAgent::UpdateIsFollowingCondition()
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

Status BTSimpleSoloAgent::UpdateIsNotLOSCondition()
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

Status BTSimpleSoloAgent::UpdateIsVisibleCondition()
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



