/* ========================================================================
   File: LOS_Obstacle.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

struct LOS_Obstacle
{
	Vector2D position = {};
	float width = 0.0f;
	float height = 0.0f;
};

LOS_Obstacle LOS_Obstacle_Create(Vector2D position, float width, float height);
void LOS_Obstacle_Render(LOS_Obstacle* obstacle, SDL_Renderer* renderer, Uint32 color);
bool LOS_Obstacle_IntersectsSegment(LOS_Obstacle* obstacle, Vector2D start, Vector2D end);

