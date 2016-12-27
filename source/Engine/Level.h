/* ========================================================================
   File: Level.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include <string>
#include <vector>

#include "DibObject.h"

class Level
{
public:
	Level() {};
	void Render();
	void Clean();

	void LoadFromArray(int* levelArray,
		Vector2D  levelOrigin,
		int levelWidth, int levelHeight,
		std::string tileImagePath, std::string tileImageID,
		int tileImageWidth, int tileImageHeight, Vector2D tileImageScale);

	std::vector<DibObject*> solids;
};
