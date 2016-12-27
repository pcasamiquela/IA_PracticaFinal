/* ========================================================================
   File: DecisionTreeInteractive.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"
#include "DecisionTree.h"

class InteractiveDecision : public Decision
{
public:
	enum DecisionResultEnum
	{
		PENDING, TRUE_NODE, FALSE_NODE
	};

	void CreateInteractiveDecision
		(char* decisionText, DecisionTreeNode* trueBranch, DecisionTreeNode* falseBranch);
	DecisionTreeNode* Update();
	void HandleInput();
	// Public for debug purposes
	char *decisionText;
private:
	DecisionResultEnum decisionResult = PENDING;

	virtual bool GetBranch();
	virtual DecisionTreeNode* MakeDecision();
};

class InteractiveAction : public Action
{
public:
	void CreateInteractiveAction
		(char* actionText);
	// Public for debug purposes
	char *actionText;
};


