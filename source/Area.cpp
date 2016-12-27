/* ========================================================================
   File: Area.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Area.h"

void Area_Create(Area* area, Vector2D center, float radius, char* name)
{
	area->center = center;
	area->radius = radius;
	area->name = name;
}

void Area_Render(Area* area, SDL_Renderer* renderer, Uint32 color)
{
	filledCircleColor(renderer, area->center.x, area->center.y, area->radius, color);
}