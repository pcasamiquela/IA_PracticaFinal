/* ========================================================================
   File: InfluenceBoid.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "InfluenceBoid.h"
#include "InfluenceMapUtils.h"
#include "StrategyUtils.h"

void InfluenceBoid::StampInfluence(InfluenceMap* map)
{
	Vector2D centerPosition = { position.x, position.y };
	Vector2D positionInInfluenceMap = InfluenceMap_GetMapPositionFromWorldPosition(
		map, centerPosition);

	switch (unitType)
	{
	case ALLIED_SOLDIER_TYPE:
		StrategyUtils::AddAlliedSoldier(map, positionInInfluenceMap.x, positionInInfluenceMap.y);
		break;
	case ENEMY_SOLDIER_TYPE:
		StrategyUtils::AddEnemySoldier(map, positionInInfluenceMap.x, positionInInfluenceMap.y);
		break;
	case ALLIED_ARCHER_TYPE:
		StrategyUtils::AddAlliedArcher(map, positionInInfluenceMap.x, positionInInfluenceMap.y);
		break;
	case ENEMY_ARCHER_TYPE:
		StrategyUtils::AddEnemyArcher(map, positionInInfluenceMap.x, positionInInfluenceMap.y);
		break;
	}
}

