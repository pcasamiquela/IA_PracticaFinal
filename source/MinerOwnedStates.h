/* ========================================================================
   File: MinerOwnedStates.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "State.h"
class MinerSimplePointer;

//------------------------------------------------------------------------
//
//  In this state the miner will walk to a goldmine and pick up a nugget
//  of gold. If the miner already has a nugget of gold he'll change state
//  to VisitBankAndDepositGold. If he gets thirsty he'll change state
//  to QuenchThirst
//------------------------------------------------------------------------
class EnterMineAndDigForNugget : public State
{
public:
	// This is a singleton
	static EnterMineAndDigForNugget* Instance();
	virtual void Enter(MinerSimplePointer* miner);
	virtual void Update(MinerSimplePointer* miner);
	virtual void Exit(MinerSimplePointer* miner);
};

//------------------------------------------------------------------------
//
//  Entity will go to a bank and deposit any nuggets he is carrying. If the 
//  miner is subsequently wealthy enough he'll walk home, otherwise he'll
//  keep going to get more gold
//------------------------------------------------------------------------
class VisitBankAndDepositGold : public State
{
public:
	// This is a singleton
	static VisitBankAndDepositGold* Instance();
	virtual void Enter(MinerSimplePointer* miner);
	virtual void Update(MinerSimplePointer* miner);
	virtual void Exit(MinerSimplePointer* miner);
};

//------------------------------------------------------------------------
//
//  Miner will go home and sleep until his fatigue is decreased
//  sufficiently
//------------------------------------------------------------------------
class GoHomeAndSleepTilRested : public State
{
public:
	// This is a singleton
	static GoHomeAndSleepTilRested* Instance();
	virtual void Enter(MinerSimplePointer* miner);
	virtual void Update(MinerSimplePointer* miner);
	virtual void Exit(MinerSimplePointer* miner);
};

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
class QuenchThirst : public State
{
public:
	// This is a singleton
	static QuenchThirst* Instance();
	virtual void Enter(MinerSimplePointer* miner);
	virtual void Update(MinerSimplePointer* miner);
	virtual void Exit(MinerSimplePointer* miner);
};
