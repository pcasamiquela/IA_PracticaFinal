/* ========================================================================
   File: MinerMacrosStateMachine.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "MinerMacrosStateMachine.h"

#include "FSMMacros.h"
#include <stdio.h>

void MinerMacrosStateMachine::InitMiner(char* name)
{
	Miner::InitMiner(name);
	InitStateMachine();
}

void MinerMacrosStateMachine::Update()
{
	// Update miner vars
	Miner::Update();
	// Run State Machine Logic
	Process(StateMachine::EVENT_Update);
}

bool MinerMacrosStateMachine::States(StateMachineEvent event, int state)
{
	BeginStateMachine
		State(STATE_HOME)
			OnEnter
				if (Location() != home)
				{
					ChangeLocation(home);
					sprintf(stateInformation, "%s: Walking home.", name);
				}
			OnUpdate
				// If miner is not fatigued start to dig for nuggets again.
				if (!Fatigued())
				{
					ChangeState(STATE_GOLDMINE);
					sprintf(stateInformation,
						"%s: What a fantastic nap! Time to find more gold.",
						name);
				}
				else
				{
					// Sleep
					DecreaseFatigue();
					sprintf(stateInformation, "%s: ZZZZ... ", name);
				}
			OnExit
				sprintf(stateInformation, "%s: Leaving the house.", name);
		State(STATE_GOLDMINE)
			OnEnter
				// If the miner is not already located at the goldmine, he must
				// change location to the gold mine
				if (Location() != goldmine)
				{
					sprintf(stateInformation, "%s: Walking to the goldmine!", name);
					ChangeLocation(goldmine);
				}
			OnUpdate
				// The miner digs for gold until he is carrying in excess of MaxNuggets. 
				// If he gets thirsty during his digging he packs up work for a while and 
				// changes state to go to the saloon for a whiskey.
				AddToGoldCarried(1);
				IncreaseFatigue();

				sprintf(stateInformation, "%s: Pickin' up a nugget.", name);

				// If enough gold mined, go and put it in the bank
				if (PocketsFull())
				{
					ChangeState(STATE_BANK);
				}
				if (Thirsty())
				{
					ChangeState(STATE_SALOON);
				}
			OnExit
				sprintf(stateInformation,
					"%s: Leaving the goldmine with pockets full of gold.", 
					name);
		State(STATE_BANK)
			OnEnter
				// On entry the miner makes sure he is located at the bank
				if (Location() != bank)
				{
					ChangeLocation(bank);
					sprintf(stateInformation, "%s: Going to the bank!", name);
				}
			OnUpdate
				// Deposit the gold
				AddToWealth(GoldCarried());
				SetGoldCarried(0);

				sprintf(stateInformation, "%s: Depositing gold. Total savings now: %d",
					name, Wealth());

				// Wealthy enough to have a well earned rest?
				if (Wealth() >= K_COMFORT_LEVEL)
				{
					ChangeState(STATE_HOME);
					sprintf(stateInformation, "%s: WooHoo! Rich enough for now. Back home!",
						name);
				}
				// Otherwise get more gold
				else
				{
					ChangeState(STATE_GOLDMINE);
				}
			OnExit
				sprintf(stateInformation, "%s: Leaving the bank.", name);
		State(STATE_SALOON)
			OnEnter
				if (Location() != saloon)
				{
					ChangeLocation(saloon);
					sprintf(stateInformation, 
						"%s: I'm thirsty! Walking to the saloon!", 
						name);
				}
			OnUpdate
				if (Thirsty())
				{
					BuyAndDrinkAWhiskey();
					ChangeState(STATE_GOLDMINE);
					sprintf(stateInformation, "%s: That's mighty fine liquor!", name);
				}
			OnExit
				sprintf(stateInformation, "%s: Leaving the saloon.", name);
	EndStateMachine
}
