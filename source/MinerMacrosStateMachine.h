/* ========================================================================
   File: MinerMacrosStateMachine.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "StateMachine.h"
#include "Miner.h"

class MinerMacrosStateMachine : public StateMachine, public Miner
{
public:
	// State Constants
	static constexpr int STATE_HOME = 0;
	static constexpr int STATE_GOLDMINE = 1;
	static constexpr int STATE_BANK = 2;
	static constexpr int STATE_SALOON = 3;

	// Overriden functions
	virtual void InitMiner(char* name) override;
	virtual void Update() override;
	virtual bool States(StateMachineEvent event, int state) override;
};