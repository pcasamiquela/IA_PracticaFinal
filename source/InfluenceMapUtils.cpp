/* ========================================================================
   File: InfluenceMapUtils.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "InfluenceMapUtils.h"
#include "PathfindingUtils.h"

using namespace std;

void InfluenceMap_Init(InfluenceMap* map, Vector2D mapOrigin)
{
	map->mapOrigin = mapOrigin;
	map->frontBuffer = (float*)map->secondBufferStorage;
	map->backBuffer = (float*)map->firstBufferStorage;
}

void InfluenceMap_Clear(InfluenceMap* map)
{
	map->influenceAgentList.clear();
}

void InfluenceMap_AddInfluenceSource(InfluenceMap* map, InfluenceAgent* influenceAgent)
{
	map->influenceAgentList.push_back(influenceAgent);
}

void InfluenceMap_DeleteInfluenceSource(InfluenceMap* map, InfluenceAgent* influenceAgent)
{
	std::list<InfluenceAgent*>::iterator it = map->influenceAgentList.begin();
	while (it != map->influenceAgentList.end())
	{
		InfluenceAgent* currentAgent = *it;
		if (currentAgent == influenceAgent)
		{
			it = map->influenceAgentList.erase(it);
		}
		else
		{
			++it;
		}
	}
}

float InfluenceMap_CalculateNeighborsInfluence(InfluenceMap* map, int posX, int posY)
{
	static Vector2D directions[8] = {
		{ -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f },
		{ -1.0f, 0.0f }, { 1.0f, 0.0f },
		{ -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }
	};
	
	Vector2D currentDirection;
	float averageInfluence = 0.0f;

	for (int i = 0; i < 8; ++i)
	{
		currentDirection = directions[i];
		int neighborX = posX + currentDirection.x;
		int neighborY = posY + currentDirection.y;

		if (0 <= neighborX && neighborX < map->width
			&& 0 <= neighborY && neighborY < map->height)
		{
			averageInfluence += (map->frontBuffer[neighborX + neighborY * map->width]
				* expf(-1.0f * map->decay)); 
		}
	}
	averageInfluence /= 8;

	return averageInfluence;
}

float InfluenceMap_GetInfluence(InfluenceMap* map, int x, int y)
{
	return map->frontBuffer[x + y * map->width];
}

void InfluenceMap_SetInfluence(InfluenceMap* map, int x, int y, float influence)
{
	map->frontBuffer[x + y * map->width] = influence;
}

void InfluenceMap_AddInfluence(InfluenceMap* map, int x, int y, float influence)
{
	map->frontBuffer[x + y * map->width] += influence;
}

void InfluenceMap_UpdateInfluence(InfluenceMap* map)
{
	for (std::list<InfluenceAgent*>::const_iterator it = map->influenceAgentList.begin();
		it != map->influenceAgentList.end(); ++it)
	{
		InfluenceAgent* currentAgent = *it;
		currentAgent->StampInfluence(map);
	}
}

void InfluenceMap_PropagateInfluence(InfluenceMap* map)
{
	// Front / Back buffer dance
	for (int i = 0; i < map->width; ++i)
	{
		for (int j = 0; j < map->height; ++j)
		{
			// Get neighbors influence
			float averageNeighborsInfluence = InfluenceMap_CalculateNeighborsInfluence(map, i, j);
			map->backBuffer[i + j * map->width] = FloatUtils::Lerp(
				map->frontBuffer[i + j * map->width], averageNeighborsInfluence, 1.0f - map->momentum);
		}
	}
	InfluenceMap_ChangeBuffer(map);
}

void InfluenceMap_ChangeBuffer(InfluenceMap* map)
{
	float* temp = map->backBuffer;
	map->backBuffer = map->frontBuffer;
	map->frontBuffer = temp;
}

void InfluenceMap_RenderMap(InfluenceMap* map, SDL_Renderer* renderer, Vector2D gridOrigin, Uint32 mapColor)
{
	for (int i = 0; i <= map->width; ++i)
	{
		lineColor(renderer, gridOrigin.x + i * map->cellSize, gridOrigin.y, 
			gridOrigin.x + i * map->cellSize, gridOrigin.y + map->height * map->cellSize, mapColor);
	}

	for (int j = 0; j <= map->height; ++j)
	{
		lineColor(renderer, gridOrigin.x, gridOrigin.y + j * map->cellSize, 
			gridOrigin.x + map->width * map->cellSize, gridOrigin.y + j * map->cellSize, mapColor);
	}
}

void InfluenceMap_RenderInfluence(InfluenceMap* map, SDL_Renderer* renderer, Vector2D gridOrigin)
{
	for (int i = 0; i < map->width; ++i)
	{
		for (int j = 0; j < map->height; ++j)
		{
			Uint32 colorToUse;
			float currentInfluence = InfluenceMap_GetInfluence(map, i, j);
			if (currentInfluence > 0)
			{
				colorToUse = Colors::WISTERIA;
			}
			else
			{
				colorToUse = Colors::PUMPKIN;
			}
			static float K_MAX_INFLUENCE = 30.0f;
			int influenceAlpha = FloatUtils::Clamp(powf(abs(currentInfluence / K_MAX_INFLUENCE), 0.3f), 0.0f, 1.0f) * 255;

			boxRGBA(renderer, 
				gridOrigin.x + i * map->cellSize,
				gridOrigin.y + j * map->cellSize,
				gridOrigin.x + i * map->cellSize + map->cellSize,
				gridOrigin.y + j * map->cellSize + map->cellSize,
				colorToUse,
				colorToUse >> 8,
				colorToUse >> 16,
				influenceAlpha);
		}
	}
}

Vector2D InfluenceMap_GetMapPositionFromWorldPosition(InfluenceMap* map, Vector2D worldPosition)
{
	int mapX = (int)floor((worldPosition.x - map->mapOrigin.x) / map->cellSize);
	int mapY = (int)floor((worldPosition.y - map->mapOrigin.y) / map->cellSize);
	return Vector2D(mapX, mapY);
}

void CalculatedMap_UpdateBasicInfluenceMap(CalculatedMap* destination, InfluenceMap* firstSource, InfluenceMap* secondSource)
{
	for (int i = 0; i < destination->width; ++i)
	{
		for (int j = 0; j < destination->height; ++j)
		{
			destination->mapArray[i + j * destination->width] =
				firstSource->frontBuffer[i + j * destination->width]
				+ secondSource->frontBuffer[i + j * destination->width];
		}
	}
	destination->maxPoint = { -1.0f, -1.0f };
}

void CalculatedMap_UpdateTensionMap(CalculatedMap* destination, InfluenceMap* firstSource, InfluenceMap* secondSource)
{
	float maxValue = -999999999.0f;
	int maxPosX = -1;
	int maxPosY = -1;
	float minValue = 999999999.0f;
	int minPosX = -1;
	int minPosY = -1;

	for (int i = 0; i < destination->width; ++i)
	{
		for (int j = 0; j < destination->height; ++j)
		{
			float calculatedTension = firstSource->frontBuffer[i + j * destination->width]
				+ abs(secondSource->frontBuffer[i + j * destination->width]);
			destination->mapArray[i + j * destination->width] = calculatedTension;

			if (calculatedTension > maxValue)
			{
				maxPosX = i;
				maxPosY = j;
				maxValue = calculatedTension;
			}

			if (calculatedTension < minValue)
			{
				minPosX = i;
				minPosY = j;
				minValue = calculatedTension;
			}
		}
	}
	destination->maxPoint = { (float)maxPosX, (float)maxPosY };
	destination->minPoint = { (float)minPosX, (float)minPosY };
}

void CalculatedMap_UpdateTensionVariationMap(CalculatedMap* destination, InfluenceMap* firstSource, InfluenceMap* secondSource)
{
	float maxValue = -999999999.0f;
	int maxPosX = -1;
	int maxPosY = -1;
	float minValue = 999999999.0f;
	int minPosX = -1;
	int minPosY = -1;

	for (int i = 0; i < destination->width; ++i)
	{
		for (int j = 0; j < destination->height; ++j)
		{
			float calculatedTension = firstSource->frontBuffer[i + j * destination->width]
										* abs(secondSource->frontBuffer[i + j * destination->width]);
			destination->mapArray[i + j * destination->width] = calculatedTension;

			if (calculatedTension > maxValue)
			{
				maxPosX = i;
				maxPosY = j;
				maxValue = calculatedTension;
			}

			if (calculatedTension < minValue)
			{
				minPosX = i;
				minPosY = j;
				minValue = calculatedTension;
			}
		}
	}

	destination->maxPoint = { (float)maxPosX, (float)maxPosY };
	destination->minPoint = { (float)minPosX, (float)minPosY };
}

void CalculatedMap_UpdateVulnerabilityMap(CalculatedMap* destination, InfluenceMap* firstSource, InfluenceMap* secondSource)
{
	float maxValue = -999999999.0f;
	int maxPosX = -1;
	int maxPosY = -1;
	float minValue = 999999999.0f;
	int minPosX = -1;
	int minPosY = -1;

	for (int i = 0; i < destination->width; ++i)
	{
		for (int j = 0; j < destination->height; ++j)
		{
			float firstValue = firstSource->frontBuffer[i + j * destination->width];
			float secondValue = secondSource->frontBuffer[i + j * destination->width];
			float calculatedTension = firstValue + abs(secondValue);
			float calculatedInfluence = firstValue + secondValue;
			float calculatedVulnerability = calculatedTension - abs(calculatedInfluence);

			destination->mapArray[i + j * destination->width] = calculatedVulnerability;

			if (calculatedVulnerability > maxValue)
			{
				maxPosX = i;
				maxPosY = j;
				maxValue = calculatedVulnerability;
			}

			if (calculatedVulnerability < minValue)
			{
				minPosX = i;
				minPosY = j;
				minValue = calculatedVulnerability;
			}
		}
	}

	destination->maxPoint = { (float)maxPosX, (float)maxPosY };
	destination->minPoint = { (float)minPosX, (float)minPosY };
}

void CalculatedMap_UpdateVulnerabilityVariationMap(CalculatedMap* destination, InfluenceMap* firstSource, InfluenceMap* secondSource)
{
	float maxValue = -999999999.0f;
	int maxPosX = -1;
	int maxPosY = -1;
	float minValue = 999999999.0f;
	int minPosX = -1;
	int minPosY = -1;

	for (int i = 0; i < destination->width; ++i)
	{
		for (int j = 0; j < destination->height; ++j)
		{
			float firstValue = firstSource->frontBuffer[i + j * destination->width];
			float secondValue = secondSource->frontBuffer[i + j * destination->width];
			float calculatedTension = firstValue + abs(secondValue);
			float calculatedInfluence = firstValue + secondValue;
			float calculatedVulnerability = calculatedTension + calculatedInfluence;
			 
			destination->mapArray[i + j * destination->width] = calculatedVulnerability;
				
			if (calculatedVulnerability > maxValue)
			{
				maxPosX = i;
				maxPosY = j;
				maxValue = calculatedVulnerability;
			}

			if (calculatedVulnerability < minValue)
			{
				minPosX = i;
				minPosY = j;
				minValue = calculatedVulnerability;
			}
		}
	}

	destination->maxPoint = { (float)maxPosX, (float)maxPosY };
	destination->minPoint = { (float)minPosX, (float)minPosY };
}

void CalculatedMap_UpdateVulnerabilityVariation2Map(CalculatedMap* destination, InfluenceMap* firstSource, InfluenceMap* secondSource)
{
	float maxValue = -999999999.0f;
	int maxPosX = -1;
	int maxPosY = -1;
	float minValue = 999999999.0f;
	int minPosX = -1;
	int minPosY = -1;

	for (int i = 0; i < destination->width; ++i)
	{
		for (int j = 0; j < destination->height; ++j)
		{
			float firstValue = firstSource->frontBuffer[i + j * destination->width];
			float secondValue = secondSource->frontBuffer[i + j * destination->width];
			float calculatedTension = firstValue + abs(secondValue);
			float calculatedInfluence = firstValue + secondValue;
			float calculatedVulnerability = calculatedTension - calculatedInfluence;

			destination->mapArray[i + j * destination->width] = calculatedVulnerability;

			if (calculatedVulnerability > maxValue)
			{
				maxPosX = i;
				maxPosY = j;
				maxValue = calculatedVulnerability;
			}

			if (calculatedVulnerability < minValue)
			{
				minPosX = i;
				minPosY = j;
				minValue = calculatedVulnerability;
			}
		}
	}

	destination->maxPoint = { (float)maxPosX, (float)maxPosY };
	destination->minPoint = { (float)minPosX, (float)minPosY };
}

void CalculatedMap_RenderInfluence(CalculatedMap* map, SDL_Renderer* renderer, Vector2D gridOrigin)
{
	for (int i = 0; i < map->width; ++i)
	{
		for (int j = 0; j < map->height; ++j)
		{
			Uint32 colorToUse;
			float currentInfluence = map->mapArray[i + j * map->width];
			if (currentInfluence > 0)
			{
				colorToUse = Colors::WISTERIA;
			}
			else
			{
				colorToUse = Colors::PUMPKIN;
			}
			static float K_MAX_INFLUENCE = 60.0f;
			int influenceAlpha = FloatUtils::Clamp(powf(abs(currentInfluence / K_MAX_INFLUENCE), 0.3f), 0.0f, 1.0f) * 255;

			boxRGBA(renderer,
				gridOrigin.x + i * map->cellSize,
				gridOrigin.y + j * map->cellSize,
				gridOrigin.x + i * map->cellSize + map->cellSize,
				gridOrigin.y + j * map->cellSize + map->cellSize,
				colorToUse,
				colorToUse >> 8,
				colorToUse >> 16,
				influenceAlpha);
		}
	}
}

void CalculatedMap_RenderInterestPoint(CalculatedMap* map, SDL_Renderer* renderer, Vector2D gridOrigin)
{
	// Render interestPoint
	if (map->maxPoint.x > -1.0f)
	{
		//boxColor(renderer,
		//	gridOrigin.x + (int)map->minPoint.x * map->cellSize,
		//	gridOrigin.y + (int)map->minPoint.y * map->cellSize,
		//	gridOrigin.x + (int)map->minPoint.x * map->cellSize + map->cellSize,
		//	gridOrigin.y + (int)map->minPoint.y * map->cellSize + map->cellSize,
		//	Colors::WHITE);

		boxColor(renderer,
			gridOrigin.x + (int)map->maxPoint.x * map->cellSize,
			gridOrigin.y + (int)map->maxPoint.y * map->cellSize,
			gridOrigin.x + (int)map->maxPoint.x * map->cellSize + map->cellSize,
			gridOrigin.y + (int)map->maxPoint.y * map->cellSize + map->cellSize,
			Colors::SUNFLOWER);
	}
}


