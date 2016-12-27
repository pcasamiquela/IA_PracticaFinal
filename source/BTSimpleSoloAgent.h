/* ========================================================================
   File: BTSimpleSoloAgent.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "BTAgent.h"

class BTSimpleSoloAgent : public BTAgent
{
public:
	// Constructor
	BTSimpleSoloAgent(int x, int y, int width, int height, Texture* questionTexture) :
		BTAgent(x, y, width, height, questionTexture)
	{
		InitBehaviorTree();
	}

protected:
	//  functions
	virtual void InitBehaviorTree() override;

private:
	void InitPatrolAction();
	Status UpdatePatrolAction();
	void TerminatePatrolAction();

	void InitFollowAction();
	Status UpdateFollowAction();
	void TerminateFollowAction();

	void InitQuestionAction();
	Status UpdateQuestionAction();
	void TerminateQuestionAction();

	Status UpdateIsFollowingCondition();
	Status UpdateIsNotLOSCondition();
	Status UpdateIsVisibleCondition();
};