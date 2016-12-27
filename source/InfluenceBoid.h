/* ========================================================================
   File: InfluenceBoid.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Boid.h"
#include "InfluenceAgent.h"

struct InfluenceBoid : public Boid, public InfluenceAgent
{
	InfluenceBoid(int x = 0, int y = 0, int width = 0, int height = 0):
		Boid(x, y, width, height)
	{};

	virtual void StampInfluence(InfluenceMap* map);
};