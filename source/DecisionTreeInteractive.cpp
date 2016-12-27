/* ========================================================================
   File: DecisionTreeInteractive.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "DecisionTreeInteractive.h"

void InteractiveDecision::CreateInteractiveDecision(char* decisionText, DecisionTreeNode* trueBranch, DecisionTreeNode* falseBranch)
{
	InitDecision();
	this->decisionText = decisionText;
	this->trueBranch = trueBranch;
	this->falseBranch = falseBranch;
}

bool InteractiveDecision::GetBranch()
{
	if (decisionResult == TRUE_NODE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

DecisionTreeNode* InteractiveDecision::MakeDecision()
{
	if (decisionResult == PENDING)
	{
		return nullptr;
	}

	// Choose a branch based on the GetBranch method
	if (GetBranch())
	{
		// Make sure its not null before recursing.
		if (trueBranch == nullptr)
		{
			return nullptr;
		}
		else
		{
			return trueBranch;
		}
	}
	else {
		// Make sure its not null before recursing.
		if (falseBranch == nullptr)
		{
			return nullptr;
		}
		else
		{
			return falseBranch;
		}
	}
}

DecisionTreeNode* InteractiveDecision::Update()
{
	if (decisionResult != PENDING)
	{
		return MakeDecision();
	}
	else
	{
		return nullptr;
	}
}

void InteractiveDecision::HandleInput()
{
	if (Input::Instance().GetKeyDown(KeyCode::Y))
	{
		decisionResult = TRUE_NODE;
	}
	else if (Input::Instance().GetKeyDown(KeyCode::N))
	{
		decisionResult = FALSE_NODE;
	}
}

void InteractiveAction::CreateInteractiveAction(char* actionText)
{
	InitAction();
	this->actionText = actionText;
}
