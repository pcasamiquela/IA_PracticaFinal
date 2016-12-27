/* ========================================================================
   File: MinerPointer.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma  once

#include "PointerFSM.h"
#include "Miner.h"

class MinerPointer : public Miner, public PointerFSM
{	
public:
	// Int & Update
	virtual void InitMiner(char* name) override;
	virtual void Update() override;

private:
	// States of the FSM
	PointerStateTemplate<MinerPointer> stateHome;
	PointerStateTemplate<MinerPointer> stateBank;
	PointerStateTemplate<MinerPointer> stateGoldMine;
	PointerStateTemplate<MinerPointer> stateSaloon;

	// Methods for StateGoldMine
	void MinerPointer::StateGoldMine_Enter();
	void MinerPointer::StateGoldMine_Update();
	void MinerPointer::StateGoldMine_Exit();

	// Methods for StateBank
	void MinerPointer::StateBank_Enter();
	void MinerPointer::StateBank_Update();
	void MinerPointer::StateBank_Exit();

	// Methods for StateHome
	void MinerPointer::StateHome_Enter();
	void MinerPointer::StateHome_Update();
	void MinerPointer::StateHome_Exit();

	// Methods for StateSaloon
	void MinerPointer::StateSaloon_Enter();
	void MinerPointer::StateSaloon_Update();
	void MinerPointer::StateSaloon_Exit();
};


