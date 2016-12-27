/* ========================================================================
   File: DecisionTreeRealTime.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"
#include "DecisionTree.h"

template <typename T>
class RealTimeDecision : public Decision
{
public:
	typedef bool (T::*DecisionFunction)();
	void CreateRealTimeDecision(T *instance,
		DecisionFunction decisionFunction, char* decisionText,
		DecisionTreeNode* trueBranch, DecisionTreeNode* falseBranch);
	virtual bool GetBranch() override;

private:
	char* decisionText; // For debug purposes
	DecisionFunction decisionFunction;
	T* instance; // Instance Pointer
};

// RealtimeDecision

template <typename T>
void RealTimeDecision<T>::CreateRealTimeDecision(T *instance,
	DecisionFunction decisionFunction,
	char* decisionText, DecisionTreeNode* trueBranch, DecisionTreeNode* falseBranch)
{
	this->instance = instance;
	this->type = DECISION;
	this->decisionFunction = decisionFunction;
	this->decisionText = decisionText;
	this->trueBranch = trueBranch;
	this->falseBranch = falseBranch;
}

template<typename T>
bool RealTimeDecision<T>::GetBranch()
{
	SDL_Log("Executing Decision Function for decision %s", decisionText);
	return(instance->*decisionFunction)();
}

template <typename T>
class RealTimeAction : public Action
{
public:
	typedef void (T::*ActionFunction)();
	void CreateRealTimeAction(T *instance,
		ActionFunction actionFunction, char* actionText);
	virtual DecisionTreeNode* MakeDecision();

	char* actionText; // Public for debug purposes
private:
	ActionFunction actionFunction;
	T* instance; // Instance Pointer
};

// RealtimeAction
template<typename T>
void RealTimeAction<T>::CreateRealTimeAction(T *instance,
	ActionFunction actionFunction, char* actionText)
{
	this->instance = instance;
	this->type = ACTION;
	this->actionFunction = actionFunction;
	this->actionText = actionText;
}

template<typename T>
DecisionTreeNode* RealTimeAction<T>::MakeDecision()
{
	SDL_Log("Executing Make Decision Function for action %s", actionText);
	(instance->*actionFunction)();
	return this;
}

// Real Time Decision Tree
template <typename T>
class RealTimeDecisionTree
{
public:
	RealTimeDecisionTree(int numDecisions, int numActions) :
		numDecisions{ numDecisions },
		numActions{ numActions }
	{
		decisions = (RealTimeDecision<T>*)
			SDL_calloc(numDecisions, sizeof(RealTimeDecision<T>));
		for (int i = 0; i < numDecisions; ++i)
		{
			new(&decisions[i]) RealTimeDecision<T>();
		}

		actions = (RealTimeAction<T>*)
			SDL_calloc(numActions, sizeof(RealTimeAction<T>));
		for (int i = 0; i < numActions; ++i)
		{
			new(&actions[i]) RealTimeAction<T>();
		}
	}

	void Clean()
	{
		SDL_free(decisions);
		SDL_free(actions);
	}

	RealTimeDecision<T>* GetDecision(int index)
	{
		return &decisions[index];
	}

	void SetDecision(int index,
		T *instance,
		typename RealTimeDecision<T>::DecisionFunction decisionFunction, char* decisionText,
		DecisionTreeNode* trueBranch, DecisionTreeNode* falseBranch);

	RealTimeAction<T>* GetAction(int index)
	{
		return &actions[index];
	}

	void SetAction(int index,
		T *instance,
		typename RealTimeAction<T>::ActionFunction actionFunction, char* actionText);

	void SetRoot(RealTimeDecision<T>* rootDecision)
	{
		root = rootDecision;
	}

	void RunTree()
	{
		root->MakeDecision();
	}

private:
	RealTimeDecision<T>* decisions;
	int numDecisions;
	RealTimeAction<T>* actions;
	int numActions;

	RealTimeDecision<T>* root;
};

template <typename T>
void RealTimeDecisionTree<T>::SetDecision(int index,
	T *instance,
	typename RealTimeDecision<T>::DecisionFunction decisionFunction, char* decisionText,
	DecisionTreeNode* trueBranch, DecisionTreeNode* falseBranch)
{
	decisions[index].CreateRealTimeDecision(instance,
		decisionFunction,
		decisionText,
		trueBranch,
		falseBranch);
}

template <typename T>
void RealTimeDecisionTree<T>::SetAction(int index,
	T *instance,
	typename RealTimeAction<T>::ActionFunction actionFunction, char* actionText)
{
	actions[index].CreateRealTimeAction(instance,
		actionFunction,
		actionText);
}