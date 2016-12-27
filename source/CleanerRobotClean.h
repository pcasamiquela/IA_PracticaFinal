/* ========================================================================
   File: CleanerRobotClean.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma  once

#include "PointerFSM.h"

enum CleanerLocationType
{
	charger,
	clean,
	trash,
	compactor,
	search
};

class CleanerRobotClean : public PointerFSM
{
public:
	virtual void InitCleanerRobotClean(char* name);
	virtual void UpdateFSM() override;

	CleanerLocationType Location()
	{
		return currentLocation;
	}
	void ChangeLocation(CleanerLocationType loc)
	{
		currentLocation = loc;
	}

	// Public for debug purposes
	char stateInformation[256];
	char currentStateInformation[24];
private:
	PointerStateTemplate<CleanerRobotClean> stateSearch;
	PointerStateTemplate<CleanerRobotClean> stateTrash;
	PointerStateTemplate<CleanerRobotClean> stateCompactor;

	// Methods for StateClean
	void CleanerRobotClean::StateSearch_Enter();
	void CleanerRobotClean::StateSearch_Update();
	void CleanerRobotClean::StateSearch_Exit();

	// Methods for StateTrash
	void CleanerRobotClean::StateTrash_Enter();
	void CleanerRobotClean::StateTrash_Update();
	void CleanerRobotClean::StateTrash_Exit();

	// Methods for StateCompactor
	void CleanerRobotClean::StateCompactor_Enter();
	void CleanerRobotClean::StateCompactor_Update();
	void CleanerRobotClean::StateCompactor_Exit();

	CleanerLocationType currentLocation = search;
	char name[48];
};

