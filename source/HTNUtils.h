/* ========================================================================
   File: HTNUtils.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"
#include <stdint.h>
#include <string>
#include <stack>
#include <vector>

#define MAXATOMS 64
#define MAXMETHODS 10
#define MAXSUBTASKS 10

typedef int64_t bitField;

// Enumerator to distinguish task types
enum TaskType
{
	PRIMITIVETASK,
	COMPOUNDTASK
};

// Describes the world state by listing values (t/f) for all known atoms
struct WorldState
{
	// Data
	bitField values = 0LL;		// Values for atoms
	bitField dontcare = -1LL;	// Mask for atoms that do not matter
};

struct Task
{
	// Data
	std::string name;				// Task name - Could be a function pointer
	TaskType type = PRIMITIVETASK;	// Task type
	WorldState preconditions;		// Preconditions for Task
};

struct PrimitiveTask : Task
{
	// Data
	WorldState postconditions;		// Postconditions for PrimitiveTask

	// Functions

	// Return true if task preconditions are met in input WorldState.
	bool ConditionsMet(WorldState ws);
};

struct Method : Task
{
	// Data
	Task* subtasks[MAXSUBTASKS];	// Subtasks for Method
	int numSubtasks = 0;			// Number of Subtasks
	bool triedToExecute = false;	// Flag if method already tried to be executed
	
	// Functions

	// Add subtask to method
	void AddSubtask(Task* task);
};

struct CompoundTask: Task
{
	// Data
	Method* methods[MAXMETHODS];		// Methods for CompoundTask			
	int numMethods = 0;                 // Number of Methods

	// Functions

	// Add method to compound task
	void AddMethod(Method* method);

	// Return the method which preconditions are satisfied by input WorldState
	// Return nullptr if no method found
	Method* FindSatisfiedMethod(WorldState ws);
};

// Structure that stores the state of the planner at some point during 
// the planning.
struct PlannerState
{
	// Data
	Task* currentTask;
	std::stack<Task*> taskStack;
	std::vector<std::string> finalPlan;
	WorldState workingWorldState;

	// Functions

	// Initializer
	void Init(Task* currentTask, WorldState workingWorldState,
		std::stack<Task*> taskStack, std::vector<std::string> finalPlan);
};

struct HTN_Planner
{
	// Data
	const char* atomNames[MAXATOMS];		// Names associated with all world state atoms
	int numAtoms = 0;						// Number of world state atoms.
	Task* rootTask = nullptr;				// Root task for planner

	// Functions

	// Initialize a worldstate to 'dontcare' for all state atoms.
	void WorldstateClear(WorldState* ws);

	// Set an atom of worldstate to specified value.
	bool WorldstateSet(WorldState* ws, const char* atomName, bool value);

	// Add a precondition for named action.
	bool SetPrecondition(Task* task, const char* atomName, bool value);

	// Add a postcondition for named action.
	bool SetPostcondition(PrimitiveTask* task, const char* atomName, bool value);

	// Helper Function: Get index for input atomName 
	int IndexForAtomName(const char* atomName);

	// Make a plan of actions that will reach desired world state.
	std::vector<std::string> DoPlan(WorldState start);
	
	// Plan helper functions
	
	// Execute PrimitiveTask and return the resulting WorldState
	WorldState DoPrimitiveTask(PrimitiveTask* task, WorldState fr);

	// Restore state of planner to the last recorded state
	PlannerState RestoreToLastDecomposedTask(std::stack<PlannerState>* worldHistory);

	// Store the state of the planner
	void RecordDecompositionOfTask(Task* task, PlannerState plan,
		std::stack<PlannerState>* plannerHistory);
};
