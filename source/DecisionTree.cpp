/* ========================================================================
   File: DecisionTree.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "DecisionTree.h"

DecisionTreeNode* Decision::MakeDecision()
{
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
			return trueBranch->MakeDecision();
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
			return falseBranch->MakeDecision();
		}
	}
}

DecisionTreeNode* Action::MakeDecision()
{
	return this;
}

