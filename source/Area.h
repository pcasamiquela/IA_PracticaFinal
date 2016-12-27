/* ========================================================================
   File: Area.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

struct Area
{
	Vector2D center;
	float radius;
	char* name;
};

void Area_Create(Area* area, Vector2D center, float radius, char* name);
void Area_Render(Area* area, SDL_Renderer* renderer, Uint32 color);