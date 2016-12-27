/* ========================================================================
   File: LOS_Obstacle.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "LOS_Obstacle.h"

LOS_Obstacle LOS_Obstacle_Create(Vector2D position, float width, float height)
{
	LOS_Obstacle createdObstacle = {};
	createdObstacle.position = position;
	createdObstacle.width = width;
	createdObstacle.height = height;
	return createdObstacle;
}

void LOS_Obstacle_Render(LOS_Obstacle* obstacle, SDL_Renderer* renderer, Uint32 color)
{
	boxColor(renderer, obstacle->position.x, obstacle->position.y,
		obstacle->position.x + obstacle->width, obstacle->position.y + obstacle->height, color);
}

bool LOS_Obstacle_IntersectsSegment(LOS_Obstacle* obstacle, Vector2D start, Vector2D end)
{
	// Check if segment intersects any of the 4 lines that comprise the wall
	// For each side of the wall
	// Top
	Vector2D wallStart = obstacle->position;
	Vector2D wallEnd = wallStart + Vector2D(obstacle->width, 0.0f);
	if (Vector2DUtils::SegmentSegmentIntersection(start,
		end,
		wallStart,
		wallEnd, true) == true)
	{
		return true;
	}
	//Bottom
	wallStart = obstacle->position + Vector2D(0.0f, obstacle->height);
	wallEnd = wallStart + Vector2D(obstacle->width, 0.0f);
	if (Vector2DUtils::SegmentSegmentIntersection(start,
		end,
		wallStart,
		wallEnd, true) == true)
	{
		return true;
	}
	// Left
	wallStart = obstacle->position;
	wallEnd = wallStart + Vector2D(obstacle->width, 0.0f);
	if (Vector2DUtils::SegmentSegmentIntersection(start,
		end,
		wallStart,
		wallEnd, true) == true)
	{
		return true;
	}
	// Right
	wallStart = obstacle->position + Vector2D(obstacle->width, 0.0f);
	wallEnd = wallStart + Vector2D(0.0f, obstacle->height);
	if (Vector2DUtils::SegmentSegmentIntersection(start,
		end,
		wallStart,
		wallEnd, true) == true)
	{
		return true;
	}

	// If no intersection
	return false;
}
