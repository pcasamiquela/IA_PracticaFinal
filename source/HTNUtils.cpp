/* ========================================================================
   File: HTNUtils.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "HTNUtils.h"

using namespace std;

bool PrimitiveTask::ConditionsMet(WorldState ws)
{
	// See if precondition is met
	const WorldState pre = preconditions;
	const bitField care = ~pre.dontcare;
	const bool met = ((pre.values & care) == (ws.values & care));
	if (met)
	{
		return true;
	}
	return false;
}

void CompoundTask::AddMethod(Method* method)
{
	// Set task to compound if it was not already set
	type = COMPOUNDTASK;
	methods[numMethods] = method;
	numMethods++;
}

Method* CompoundTask::FindSatisfiedMethod(WorldState ws)
{
	for (int i = 0; i < numMethods; ++i)
	{
		Method* currentMethod = methods[i];
		if (currentMethod->triedToExecute == false)
		{
			// See if precondition is met
			const WorldState pre = currentMethod->preconditions;
			const bitField care = ~pre.dontcare;
			const bool met = ((pre.values & care) == (ws.values & care));
			if (met)
			{
				return currentMethod;
			}
		}
	}
	return nullptr;
}

void Method::AddSubtask(Task* task)
{
	subtasks[numSubtasks] = task;
	numSubtasks++;
}

void PlannerState::Init(Task* currentTask, WorldState workingWorldState, 
	std::stack<Task*> taskStack, std::vector<std::string> finalPlan)
{
	this->currentTask = currentTask;
	this->workingWorldState = workingWorldState;
	this->taskStack = taskStack;
	this->finalPlan = finalPlan;
}

int HTN_Planner::IndexForAtomName(const char* atomName)
{
	int idx;
	for (idx = 0; idx < numAtoms; ++idx)
	{
		if (!strcmp(atomNames[idx], atomName))
		{
			return idx;
		}
	}

	if (idx < MAXATOMS)
	{
		atomNames[idx] = atomName;
		numAtoms++;
		return idx;
	}

	return -1;
}

PlannerState HTN_Planner::RestoreToLastDecomposedTask(stack<PlannerState>* plannerHistory)
{
	SDL_Log("RestoreToLastDecomposedTask");

	PlannerState stateToReturn = plannerHistory->top();
	plannerHistory->pop();
	stateToReturn.taskStack.push(stateToReturn.currentTask);
	return stateToReturn;
}

void HTN_Planner::RecordDecompositionOfTask(Task* task, PlannerState plan, stack<PlannerState>* plannerHistory)
{
	SDL_Log("RecordDecompositionOfTask");

	plan.currentTask = task;
	plannerHistory->push(plan);
}

WorldState HTN_Planner::DoPrimitiveTask(PrimitiveTask* task, WorldState fr)
{
	const WorldState pst = task->postconditions;
	const bitField unaffected = pst.dontcare;
	const bitField affected = ~unaffected;

	fr.values = (fr.values & unaffected) | (pst.values & affected);
	fr.dontcare &= pst.dontcare;
	return fr;
}

vector<string> HTN_Planner::DoPlan(WorldState start)
{
	// Initialize helper structures
	stack<PlannerState> decompositionHistory;
	PlannerState currentState;
	currentState.workingWorldState = start;
	currentState.taskStack.push(rootTask);

	// Book-keeping of Compound Tasks to unflag on planning's end
	std::vector<Method*> flaggedMethods;

	// Loop while there are tasks in the stack
	while (currentState.taskStack.empty() == false)
	{
		Task* currentTask = currentState.taskStack.top();
		SDL_Log("Checking Task with name = %s", currentTask->name.c_str());
		currentState.taskStack.pop();

		if (currentTask->type == COMPOUNDTASK)
		{
			SDL_Log("Task is CompoundTask");
			CompoundTask* currentCompound = (CompoundTask*)currentTask;
			// Check if some method's preconditions are satisfied by current WorldState
			Method* satisfiedMethod = currentCompound->FindSatisfiedMethod(currentState.workingWorldState);

			if (satisfiedMethod != nullptr)
			{
				// Flag the method as tried to avoid later retry
				satisfiedMethod->triedToExecute = true;
				flaggedMethods.push_back(satisfiedMethod);
				SDL_Log("Satisfied SubTask with name = %s", satisfiedMethod->name.c_str());

				// Record State
				RecordDecompositionOfTask(currentTask, currentState, &decompositionHistory);
				
				// If we found a satisfied method, add its subtasks (in inverse order!)
				for (int i = satisfiedMethod->numSubtasks - 1; i >= 0; --i)
				{
					currentState.taskStack.push(satisfiedMethod->subtasks[i]);
				}
			}
			else
			{
				if (decompositionHistory.size() > 0)
				{
					// Rollback if possible
					SDL_Log("No Method is satisfiable - Rolling Back");
					currentState = RestoreToLastDecomposedTask(&decompositionHistory);
				}
			}
		}
		else // Primitive Task
		{
			SDL_Log("Task is PrimitiveTask");
			PrimitiveTask* currentPrimitive = (PrimitiveTask*)currentTask;

			// Check if some PrimitiveTask's preconditions are satisfied by current WorldState
			if (currentPrimitive->ConditionsMet(currentState.workingWorldState))
			{
				SDL_Log("Conditions Met for PrimitiveTask");
				// Execute PrimitiveTask
				currentState.workingWorldState = DoPrimitiveTask(currentPrimitive, 
					currentState.workingWorldState);
				// Push Action to Plan
				currentState.finalPlan.push_back(currentPrimitive->name);
			}
			else
			{
				if (decompositionHistory.size() > 0)
				{
					// Rollback if possible
					SDL_Log("Conditions Not Met for Task - Rolling Back");
					currentState = RestoreToLastDecomposedTask(&decompositionHistory);
				}
			}
		}
	}

	// Unflag Methods - could be optimized
	for (int i = 0; i < (int)flaggedMethods.size(); ++i)
	{
		flaggedMethods[i]->triedToExecute = false;
	}

	// Return plan
	return currentState.finalPlan;
}

void HTN_Planner::WorldstateClear(WorldState* ws)
{
	ws->values = 0LL;
	ws->dontcare = -1LL;
}

bool HTN_Planner::WorldstateSet(WorldState* ws, const char* atomName, bool value)
{
	const int idx = IndexForAtomName(atomName);
	if (idx == -1)
	{
		return false;
	}
	ws->values = value ? (ws->values | (1LL << idx)) : (ws->values & ~(1LL << idx));
	ws->dontcare &= ~(1LL << idx);
	return true;
}

bool HTN_Planner::SetPrecondition(Task* task, const char* atomName, bool value)
{
	const int atmidx = IndexForAtomName(atomName);
	if (atmidx == -1)
	{
		return false;
	}
	WorldstateSet(&task->preconditions, atomName, value);
	return true;
}

bool HTN_Planner::SetPostcondition(PrimitiveTask* task, const char* atomName, bool value)
{
	const int atmidx = IndexForAtomName(atomName);
	if (atmidx == -1)
	{
		return false;
	}
	WorldstateSet(&task->postconditions, atomName, value);
	return true;
}
