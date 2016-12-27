/* ========================================================================
   File: CleanerRobotEnergy.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "CleanerRobotEnergy.h"

#include "Engine/EngineFramework.h"
#include <stdio.h>

void CleanerRobotEnergy::InitCleanerRobotEnergy(char* name)
{
	strcpy(this->name, name);

	// Setup States
	stateCleanup.Set(this, &CleanerRobotEnergy::StateCleanup_Enter,
		&CleanerRobotEnergy::StateCleanup_Update,
		&CleanerRobotEnergy::StateCleanup_Exit);
	stateCharge.Set(this, &CleanerRobotEnergy::StateCharge_Enter,
		&CleanerRobotEnergy::StateCharge_Update,
		&CleanerRobotEnergy::StateCharge_Exit);

	currentState = &stateCleanup;
	StateCleanup_Enter();

	cleanFSM.InitCleanerRobotClean(name);
}

void CleanerRobotEnergy::UpdateFSM()
{
	PointerFSM::UpdateFSM();
}

void CleanerRobotEnergy::Update()
{
	UpdateFSM();
}

// Methods for StateCleanup
void CleanerRobotEnergy::StateCleanup_Enter()
{
	sprintf(currentStateInformation, "State: Cleanup");
	sprintf(stateInformation, "%s: Let's clean!", name);
	ChangeLocation(clean);
}

void CleanerRobotEnergy::StateCleanup_Update()
{
	// Execute internal FSM
	cleanFSM.UpdateFSM();

	energy -= 1;
	if (energy <= 0)
	{
		ChangeState(stateCharge);
	}
}

void CleanerRobotEnergy::StateCleanup_Exit()
{
	sprintf(stateInformation, "%s: Cannot clean any more!", name);
}

// Methods for StateCharge
void CleanerRobotEnergy::StateCharge_Enter()
{
	sprintf(currentStateInformation, "State: Charge");
	sprintf(stateInformation, "%s: Walking to the charger!", name);
	ChangeLocation(charger);
}

void CleanerRobotEnergy::StateCharge_Update()
{
	sprintf(stateInformation, "%s: Charging!", name);
	energy += 1;
	if (energy >= K_MAX_ENERGY)
	{
		ChangeState(stateCleanup);
	}
}

void CleanerRobotEnergy::StateCharge_Exit()
{
	sprintf(stateInformation, "%s: Going back to work!", name);
}
