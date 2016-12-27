/* ========================================================================
   File: StrategyUtils.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "InfluenceMapUtils.h"

namespace StrategyUtils
{
	struct UnitInfluence
	{
		int width = 0;
		int height = 0;
		int* influenceArray;
	};

	static int soldierInfluenceArray[25] = { 5, 5, 5, 5, 5,
											5, 5, 5, 5, 5,
											5, 5, 5, 5, 5,
											5, 5, 5, 5, 5,
											5, 5, 5, 5, 5 };

	static void AddAlliedSoldier(InfluenceMap* map, int x, int y)
	{
		static bool unitInitialized = false;
		static UnitInfluence unitInfluence;
		if (unitInitialized == false)
		{
			unitInfluence.width = 5;
			unitInfluence.height = 5;
			unitInfluence.influenceArray = soldierInfluenceArray;
			unitInitialized = true;
		}

		int initialX = x - floorf(unitInfluence.width / 2);
		int initialY = y - floorf(unitInfluence.height / 2);
		for (int i = 0; i < unitInfluence.width; ++i)
		{
			for (int j = 0; j < unitInfluence.height; ++j)
			{
				int currentX = initialX + i;
				int currentY = initialY + j;
				if (currentX >= 0 && currentX < map->width &&
					currentY >= 0 && currentY < map->height)
				{
					if (unitInfluence.influenceArray[i + j*unitInfluence.width] != 0)
					{
						InfluenceMap_AddInfluence(map, initialX + i, initialY + j,
							unitInfluence.influenceArray[i + j*unitInfluence.width]);
					}
				}
			}
		}
	}

	static void AddEnemySoldier(InfluenceMap* map, int x, int y)
	{
		static bool unitInitialized = false;
		static UnitInfluence unitInfluence;
		if (unitInitialized == false)
		{
			unitInfluence.width = 5;
			unitInfluence.height = 5;
			unitInfluence.influenceArray = soldierInfluenceArray;
			unitInitialized = true;
		}

		int initialX = x - floorf(unitInfluence.width / 2);
		int initialY = y - floorf(unitInfluence.height / 2);
		for (int i = 0; i < unitInfluence.width; ++i)
		{
			for (int j = 0; j < unitInfluence.height; ++j)
			{
				int currentX = initialX + i;
				int currentY = initialY + j;
				if (currentX >= 0 && currentX < map->width &&
					currentY >= 0 && currentY < map->height)
				{
					if (unitInfluence.influenceArray[i + j*unitInfluence.width] != 0)
					{
						InfluenceMap_AddInfluence(map, initialX + i, initialY + j,
							-unitInfluence.influenceArray[i + j*unitInfluence.width]);
					}
				}
			}
		}
	}

	static int archerInfluenceArray[81] = { 
		5, 5, 5, 5, 5, 5, 5, 5, 5,
		5, 0, 0, 0, 0, 0, 0, 0, 5,
		5, 0, 0, 0, 0, 0, 0, 0, 5,
		5, 0, 0, 0, 0, 0, 0, 0, 5,
		5, 0, 0, 0, 0, 0, 0, 0, 5,
		5, 0, 0, 0, 0, 0, 0, 0, 5,
		5, 0, 0, 0, 0, 0, 0, 0, 5,
		5, 0, 0, 0, 0, 0, 0, 0, 5,
		5, 5, 5, 5, 5, 5, 5, 5, 5
	};

	static void AddAlliedArcher(InfluenceMap* map, int x, int y)
	{
		static bool unitInitialized = false;
		static UnitInfluence unitInfluence;
		if (unitInitialized == false)
		{
			unitInfluence.width = 9;
			unitInfluence.height = 9;
			unitInfluence.influenceArray = archerInfluenceArray;
			unitInitialized = true;
		}

		int initialX = x - floorf(unitInfluence.width / 2);
		int initialY = y - floorf(unitInfluence.height / 2);
		for (int i = 0; i < unitInfluence.width; ++i)
		{
			for (int j = 0; j < unitInfluence.height; ++j)
			{
				int currentX = initialX + i;
				int currentY = initialY + j;
				if (currentX >= 0 && currentX < map->width &&
					currentY >= 0 && currentY < map->height)
			 	{
					if (unitInfluence.influenceArray[i + j*unitInfluence.width] != 0)
					{
						InfluenceMap_AddInfluence(map, initialX + i, initialY + j,
							unitInfluence.influenceArray[i + j*unitInfluence.width]);
					}
				}
			}
		}
	}

	static void AddEnemyArcher(InfluenceMap* map, int x, int y)
	{
		static bool unitInitialized = false;
		static UnitInfluence unitInfluence;
		if (unitInitialized == false)
		{
			unitInfluence.width = 9;
			unitInfluence.height = 9;
			unitInfluence.influenceArray = archerInfluenceArray;
			unitInitialized = true;
		}

		int initialX = x - floorf(unitInfluence.width / 2);
		int initialY = y - floorf(unitInfluence.height / 2);
		for (int i = 0; i < unitInfluence.width; ++i)
		{
			for (int j = 0; j < unitInfluence.height; ++j)
			{
				int currentX = initialX + i;
				int currentY = initialY + j;
				if (currentX >= 0 && currentX < map->width &&
					currentY >= 0 && currentY < map->height)
				{
					if (unitInfluence.influenceArray[i + j*unitInfluence.width] != 0)
					{
						InfluenceMap_AddInfluence(map, initialX + i, initialY + j,
							-unitInfluence.influenceArray[i + j*unitInfluence.width]);
					}
				}
			}
		}
	}
}
