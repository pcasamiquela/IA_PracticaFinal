/* ========================================================================
   File: MinerOwnedStates.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "MinerOwnedStates.h"
#include "State.h"
#include "MinerSimplePointer.h"
#include <stdio.h>

// Methods for EnterMineAndDigForNugget
EnterMineAndDigForNugget* EnterMineAndDigForNugget::Instance()
{
	static EnterMineAndDigForNugget instance;
	return &instance;
}

void EnterMineAndDigForNugget::Enter(MinerSimplePointer* miner)
{
	// If the miner is not already located at the goldmine, he must
	// change location to the gold mine
	if (miner->Location() != Miner::goldmine)
	{
		sprintf(miner->stateInformation, "%s: Walking to the goldmine!", miner->name);
		miner->ChangeLocation(Miner::goldmine);
	}
}

void EnterMineAndDigForNugget::Update(MinerSimplePointer* miner)
{
	// The miner digs for gold until he is carrying in excess of MaxNuggets. 
	// If he gets thirsty during his digging he packs up work for a while and 
	// changes state to go to the saloon for a whiskey.
	miner->AddToGoldCarried(1);
	miner->IncreaseFatigue();

	sprintf(miner->stateInformation, "%s: Pickin' up a nugget.", miner->name);

	// If enough gold mined, go and put it in the bank
	if (miner->PocketsFull())
	{
		miner->ChangeState(VisitBankAndDepositGold::Instance());
	}
	if (miner->Thirsty())
	{
		miner->ChangeState(QuenchThirst::Instance());
	}
}


void EnterMineAndDigForNugget::Exit(MinerSimplePointer* miner)
{
	sprintf(miner->stateInformation, "%s: Leaving the goldmine with pockets full of gold.", miner->name);
}

// Methods for VisitBankAndDepositGold
VisitBankAndDepositGold* VisitBankAndDepositGold::Instance()
{
	static VisitBankAndDepositGold instance;
	return &instance;
}

void VisitBankAndDepositGold::Enter(MinerSimplePointer* miner)
{
	// On entry the miner makes sure he is located at the bank
	if (miner->Location() != Miner::bank)
	{
		sprintf(miner->stateInformation, "%s: Going to the bank!", miner->name);
		miner->ChangeLocation(Miner::bank);
	}
}

void VisitBankAndDepositGold::Update(MinerSimplePointer* miner)
{
	// Deposit the gold
	miner->AddToWealth(miner->GoldCarried());
	miner->SetGoldCarried(0);

	sprintf(miner->stateInformation, "%s: Depositing gold. Total savings now: %d", 
		miner->name, miner->Wealth());

	// Wealthy enough to have a well earned rest?
	if (miner->Wealth() >= Miner::K_COMFORT_LEVEL)
	{
		sprintf(miner->stateInformation, "%s: WooHoo! Rich enough for now. Back home!",
			miner->name);
		miner->ChangeState(GoHomeAndSleepTilRested::Instance());
	}
	// Otherwise get more gold
	else
	{
		miner->ChangeState(EnterMineAndDigForNugget::Instance());
	}
}

void VisitBankAndDepositGold::Exit(MinerSimplePointer* miner)
{
	sprintf(miner->stateInformation, "%s: Leaving the bank.", miner->name);
}

// Methods for GoHomeAndSleepTilRested

GoHomeAndSleepTilRested* GoHomeAndSleepTilRested::Instance()
{
	static GoHomeAndSleepTilRested instance;
	return &instance;
}

void GoHomeAndSleepTilRested::Enter(MinerSimplePointer* miner)
{
	if (miner->Location() != Miner::home)
	{
		sprintf(miner->stateInformation, "%s: Walking home.", miner->name);
		miner->ChangeLocation(Miner::home);
	}
}

void GoHomeAndSleepTilRested::Update(MinerSimplePointer* miner)
{
	// If miner is not fatigued start to dig for nuggets again.
	if (!miner->Fatigued())
	{
		sprintf(miner->stateInformation, "%s: What a fantastic nap! Time to find more gold.", 
			miner->name);
		miner->ChangeState(EnterMineAndDigForNugget::Instance());
	}
	else
	{
		// Sleep
		miner->DecreaseFatigue();
		sprintf(miner->stateInformation, "%s: ZZZZ... ", miner->name);
	}
}

void GoHomeAndSleepTilRested::Exit(MinerSimplePointer* miner)
{
	sprintf(miner->stateInformation, "%s: Leaving the house.", miner->name);
}

// Methods for QuenchThirst

QuenchThirst* QuenchThirst::Instance()
{
	static QuenchThirst instance;
	return &instance;
}

void QuenchThirst::Enter(MinerSimplePointer* miner)
{
	if (miner->Location() != Miner::saloon)
	{
		miner->ChangeLocation(Miner::saloon);
		sprintf(miner->stateInformation, "%s: I'm thirsty! Walking to the saloon!", miner->name);
	}
}

void QuenchThirst::Update(MinerSimplePointer* miner)
{
	if (miner->Thirsty())
	{
		miner->BuyAndDrinkAWhiskey();
		sprintf(miner->stateInformation, "%s: That's mighty fine liquor!", miner->name);
		miner->ChangeState(EnterMineAndDigForNugget::Instance());
	}
	else
	{
		sprintf(miner->stateInformation, "%s: ERROR!", miner->name);
	}
}

void QuenchThirst::Exit(MinerSimplePointer* miner)
{
	sprintf(miner->stateInformation, "%s: Leaving the saloon.", miner->name);
}

