/* ========================================================================
   File: InfluencePlayer.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "InfluenceAgent.h"
#include "Player.h"

struct InfluencePlayer : public Player, public InfluenceAgent
{
	InfluencePlayer(int x, int y, int width, int height):
		Player(x, y, width, height)
	{};

	virtual void StampInfluence(InfluenceMap* map);
};