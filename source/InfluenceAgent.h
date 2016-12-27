/* ========================================================================
   File: InfluenceAgent.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "InfluenceTypes.h"

struct InfluenceMap;

struct InfluenceAgent
{
	// Data
	UnitType unitType;

	// Methods
	virtual void Init(UnitType unitType);
	virtual void StampInfluence(InfluenceMap* map) = 0;
};