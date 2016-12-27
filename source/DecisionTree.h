/* ========================================================================
   File: DecisionTree.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

class DecisionTreeNode
{
public:
	enum NodeTypeEnum
	{
		DECISION, ACTION
	};

	virtual DecisionTreeNode* MakeDecision() = 0;
	NodeTypeEnum type = DECISION;
};

class Decision : public DecisionTreeNode
{
public:
	virtual bool GetBranch() = 0;
	virtual DecisionTreeNode* MakeDecision() override;
protected:
	DecisionTreeNode* trueBranch;
	DecisionTreeNode* falseBranch;
	void InitDecision()
	{
		type = DECISION;
	}
};

class Action : public DecisionTreeNode
{
public:
	virtual DecisionTreeNode* MakeDecision() override;
protected:
	void InitAction()
	{
		type = ACTION;
	}
};


