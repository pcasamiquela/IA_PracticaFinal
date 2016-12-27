/* ========================================================================
   File: Miner.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Miner.h"
#include <string.h> 

void Miner::InitMiner(char* name)
{
	strcpy(this->name, name);
	currentLocation = home;
	goldCarried = 0;
	moneyInBank = 0;
	thirst = 0;
	fatigue = 0;
}

void Miner::Update()
{
	thirst += 1;
}

void Miner::AddToGoldCarried(int val)
{
	goldCarried += val;
	if (goldCarried < 0) 
	{
		goldCarried = 0;
	}
}

void Miner::AddToWealth(int val)
{
	moneyInBank += val;
	if (moneyInBank < 0)
	{
		moneyInBank = 0;
	}
}

bool Miner::Thirsty()
{
	if (thirst >= K_THIRST_LEVEL) 
	{ 
		return true; 
	}
	return false;
}


bool Miner::Fatigued()
{
	if (fatigue > K_TIREDNESS_THRESHOLD)
	{
		return true;
	}
	return false;
}