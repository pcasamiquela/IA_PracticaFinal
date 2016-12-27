/* ========================================================================
   File: PathfindingHelpers.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

namespace PathfindingHelpers
{
	static Vector2D ConvertGridPositionToRealWorldPosition(Vector2D gridPosition, Vector2D gridOrigin, int cellSize)
	{
		return Vector2D(gridPosition.x * cellSize + gridOrigin.x,
			gridPosition.y * cellSize + gridOrigin.y);
	}

	// Example usage:
	// If path is the result of you A* Pathfinding
	// and simplePath is a SimplePath variable
	//boid.simplePath = &simplePath;
	//boid.simplePath->pathOccupation = path.size();
	//PathfindingHelpers::FillVector2DArrayWithNodePositions(path, (Vector2D*)simplePath.pathArray, gridOrigin, grid.cellSize);

	// pathArray is filled in place
	// New Vector2D positions in pathArray are in World coordinates
	static void FillVector2DArrayWithNodePositions(std::vector<Node*> path, Vector2D* pathArray, Vector2D gridOrigin, int cellSize)
	{
		for (int i = 0; i < (int)path.size(); ++i)
		{
			Vector2D* pathArrayPosition = &pathArray[i];
			*pathArrayPosition = ConvertGridPositionToRealWorldPosition(path[i]->position, gridOrigin, cellSize);
		}
	}
}
