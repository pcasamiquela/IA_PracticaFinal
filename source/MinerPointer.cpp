/* ========================================================================
   File: MinerPointer.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "MinerPointer.h"

#include "Engine/EngineFramework.h"
#include <stdio.h>

void MinerPointer::InitMiner(char* name)
{
	Miner::InitMiner(name);

	// Setup States
	stateHome.Set(this, &MinerPointer::StateHome_Enter,
		&MinerPointer::StateHome_Update,
		&MinerPointer::StateHome_Exit);
	stateBank.Set(this, &MinerPointer::StateBank_Enter,
		&MinerPointer::StateBank_Update,
		&MinerPointer::StateBank_Exit);
	stateSaloon.Set(this, &MinerPointer::StateSaloon_Enter,
		&MinerPointer::StateSaloon_Update,
		&MinerPointer::StateSaloon_Exit);
	stateGoldMine.Set(this, &MinerPointer::StateGoldMine_Enter,
		&MinerPointer::StateGoldMine_Update,
		&MinerPointer::StateGoldMine_Exit);

	currentState = &stateHome;
}

void MinerPointer::Update()
{
	Miner::Update();
	PointerFSM::UpdateFSM();
}

// Methods for StateGoldmine
void MinerPointer::StateGoldMine_Enter()
{
	// If the miner is not already located at the goldmine, he must
	// change location to the gold mine
	if (Location() != goldmine)
	{
		sprintf(stateInformation, "%s: Walking to the goldmine!", name);
		ChangeLocation(goldmine);
	}
}

void MinerPointer::StateGoldMine_Update()
{
	// The miner digs for gold until he is carrying in excess of MaxNuggets. 
	// If he gets thirsty during his digging he packs up work for a while and 
	// changes state to go to the saloon for a whiskey.
	AddToGoldCarried(1);
	IncreaseFatigue();

	sprintf(stateInformation, "%s: Pickin' up a nugget.", name);

	// If enough gold mined, go and put it in the bank
	if (PocketsFull())
	{
		ChangeState(stateBank);
	}
	if (Thirsty())
	{
		ChangeState(stateSaloon);
	}
}

void MinerPointer::StateGoldMine_Exit()
{
	sprintf(stateInformation, "%s: Leaving the goldmine with pockets full of gold.", name);
}

// Methods for StateBank
void MinerPointer::StateBank_Enter()
{
	// On entry the miner makes sure he is located at the bank
	if (Location() != bank)
	{
		sprintf(stateInformation, "%s: Going to the bank!", name);
		ChangeLocation(bank);
	}
}

void MinerPointer::StateBank_Update()
{
	// Deposit the gold
	AddToWealth(GoldCarried());
	SetGoldCarried(0);

	sprintf(stateInformation, "%s: Depositing gold. Total savings now: %d",
		name, Wealth());

	// Wealthy enough to have a well earned rest?
	if (Wealth() >= K_COMFORT_LEVEL)
	{
		sprintf(stateInformation, "%s: WooHoo! Rich enough for now. Back home!",
			name);
		ChangeState(stateHome);
	}
	// Otherwise get more gold
	else
	{
		ChangeState(stateGoldMine);
	}
}

void MinerPointer::StateBank_Exit()
{
	sprintf(stateInformation, "%s: Leaving the bank.", name);
}

// Methods for StateHome
void MinerPointer::StateHome_Enter()
{
	if (Location() != home)
	{
		sprintf(stateInformation, "%s: Walking home.", name);
		ChangeLocation(home);
	}
}

void MinerPointer::StateHome_Update()
{
	// If miner is not fatigued start to dig for nuggets again.
	if (!Fatigued())
	{
		sprintf(stateInformation, "%s: What a fantastic nap! Time to find more gold.",
			name);
		ChangeState(stateGoldMine);
	}
	else
	{
		// Sleep
		DecreaseFatigue();
		sprintf(stateInformation, "%s: ZZZZ... ", name);
	}
}

void MinerPointer::StateHome_Exit()
{
	sprintf(stateInformation, "%s: Leaving the house.", name);
}

// Methods for StateSaloon
void MinerPointer::StateSaloon_Enter()
{
	if (Location() != saloon)
	{
		ChangeLocation(saloon);
		sprintf(stateInformation, "%s: I'm thirsty! Walking to the saloon!", name);
	}
}

void MinerPointer::StateSaloon_Update()
{
	if (Thirsty())
	{
		BuyAndDrinkAWhiskey();
		sprintf(stateInformation, "%s: That's mighty fine liquor!", name);
		ChangeState(stateGoldMine);
	}
}

void MinerPointer::StateSaloon_Exit()
{
	sprintf(stateInformation, "%s: Leaving the saloon.", name);
}

