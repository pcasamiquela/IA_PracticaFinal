/* ========================================================================
   File: CleanerRobotEnergy.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma  once

#include "PointerFSM.h"
#include "CleanerRobotClean.h"

class CleanerRobotEnergy : public PointerFSM
{
public:
	virtual void InitCleanerRobotEnergy(char* name);
	virtual void UpdateFSM() override;
	void Update();

	CleanerLocationType Location()
	{
		return currentLocation;
	}
	void ChangeLocation(CleanerLocationType loc)
	{
		currentLocation = loc;
	}

	// Public for debug purposes
	PointerStateTemplate<CleanerRobotEnergy> stateCleanup;
	PointerStateTemplate<CleanerRobotEnergy> stateCharge;

	CleanerRobotClean cleanFSM;
	char stateInformation[256];
	char currentStateInformation[24];
	static constexpr int K_MAX_ENERGY = 5;
	int energy = K_MAX_ENERGY;

private:
	// Methods for StateClean
	void CleanerRobotEnergy::StateCleanup_Enter();
	void CleanerRobotEnergy::StateCleanup_Update();
	void CleanerRobotEnergy::StateCleanup_Exit();

	// Methods for StateCharge
	void CleanerRobotEnergy::StateCharge_Enter();
	void CleanerRobotEnergy::StateCharge_Update();
	void CleanerRobotEnergy::StateCharge_Exit();

	CleanerLocationType currentLocation = clean;
	char name[48];
};

