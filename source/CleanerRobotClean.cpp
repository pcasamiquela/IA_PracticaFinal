/* ========================================================================
   File: CleanerRobotClean.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "CleanerRobotClean.h"

#include "Engine/EngineFramework.h"
#include <stdio.h>

void CleanerRobotClean::InitCleanerRobotClean(char* name)
{
	strcpy(this->name, name);

	// Setup States
	stateSearch.Set(this, &CleanerRobotClean::StateSearch_Enter,
		&CleanerRobotClean::StateSearch_Update,
		&CleanerRobotClean::StateSearch_Exit);
	stateTrash.Set(this, &CleanerRobotClean::StateTrash_Enter,
		&CleanerRobotClean::StateTrash_Update,
		&CleanerRobotClean::StateTrash_Exit);
	stateCompactor.Set(this, &CleanerRobotClean::StateCompactor_Enter,
		&CleanerRobotClean::StateCompactor_Update,
		&CleanerRobotClean::StateCompactor_Exit);

	currentState = &stateSearch;

	StateSearch_Enter();
}

void CleanerRobotClean::UpdateFSM()
{
	PointerFSM::UpdateFSM();
}

// Methods for StateSearch
void CleanerRobotClean::StateSearch_Enter()
{
	sprintf(currentStateInformation, "SubState: Search");
	sprintf(stateInformation, "%s: Going searching!", name);
	ChangeLocation(search);
}

void CleanerRobotClean::StateSearch_Update()
{
	if (IntegerUtils::RandomRange(0, 5) > 3)
	{
		ChangeState(stateTrash);
	}
	else
	{
		sprintf(stateInformation, "%s: Trash found!", name);
	}
}

void CleanerRobotClean::StateSearch_Exit()
{
	sprintf(stateInformation, "%s: Out of search!", name);
}

// Methods for StateTrash
void CleanerRobotClean::StateTrash_Enter()
{
	sprintf(currentStateInformation, "SubState: Trash");
	sprintf(stateInformation, "%s: Go to the trash!", name);
	ChangeLocation(trash);
}

void CleanerRobotClean::StateTrash_Update()
{
	if (IntegerUtils::RandomRange(0, 10) > 8)
	{
		ChangeState(stateCompactor);
	}
	else
	{
		sprintf(stateInformation, "%s: No item found!", name);
	}
}

void CleanerRobotClean::StateTrash_Exit()
{
	sprintf(stateInformation, "%s: I got an item!", name);
}

// Methods for StateCompactor 
void CleanerRobotClean::StateCompactor_Enter()
{
	sprintf(currentStateInformation, "SubState: Compactor");
	sprintf(stateInformation, "%s: Let's go to the compactor!", name);
	ChangeLocation(compactor);
}

void CleanerRobotClean::StateCompactor_Update()
{
	if (IntegerUtils::RandomRange(0, 5) > 3)
	{
		ChangeState(stateSearch);
	}
	else
	{
		sprintf(stateInformation, "%s: Still in the compactor!", name);
	}
}

void CleanerRobotClean::StateCompactor_Exit()
{
	sprintf(stateInformation, "%s: Out of the compactor!", name);
}
