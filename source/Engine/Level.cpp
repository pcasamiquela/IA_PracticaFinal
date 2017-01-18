/* ========================================================================
   File: Level.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Level.h"
#include "Utilities.h"
#include "DibSprite.h"
#include <string>

void Level::Render()
{
	for (int i = 0; i < (int)solids.size(); ++i)
	{
		solids[i]->Render();
	}
	for (int i = 0; i < (int)noSolids.size(); ++i)
	{
		noSolids[i]->Render();
	}
}

void Level::Clean()
{
	for (int i = 0; i < (int)solids.size(); ++i)
	{
		delete solids[i];
	}
}

void Level::LoadFromArray(int* levelArray, 
	Vector2D  levelOrigin, 
	int levelWidth, int levelHeight,
	std::string tileImagePath, std::string tileImageID,
	int tileImageWidth, int tileImageHeight, Vector2D tileImageScale)
{
	solids = {};

	// Loop to create solid objects
	for (int i = 0; i < levelHeight; ++i)
	{
		for (int j = 0; j < levelWidth; ++j)
		{
			if (*(levelArray + (i*levelWidth) + j) == 0)
			{
				tileImagePath = PathUtils::GetResourcesPath("images/Floor_Tile.png");
				tileImageID = "Floor";
				//SDL_Log("%d-%d = %d", i, j, *(levelArray + (i*width) + j));
				DibSprite* currentTile = new DibSprite
					(levelOrigin.x + tileImageWidth * j * tileImageScale.x,
						levelOrigin.y + tileImageHeight * i * tileImageScale.y,
						tileImageWidth, tileImageHeight);
				currentTile->SetOrigin(0.0f, 0.0f);
				currentTile->LoadGraphic(tileImagePath, tileImageID, 
					tileImageWidth, tileImageHeight, false);
				currentTile->SetScale(tileImageScale.x, tileImageScale.y);
				currentTile->moves = false;
				noSolids.push_back(currentTile);
			}
			else if (*(levelArray + (i*levelWidth) + j) == 1)
			{
				tileImagePath = PathUtils::GetResourcesPath("images/Wall_Tile.png");
				tileImageID = "Wall";
				//SDL_Log("%d-%d = %d", i, j, *(levelArray + (i*width) + j));
				DibSprite* currentTile = new DibSprite
				(levelOrigin.x + tileImageWidth * j * tileImageScale.x,
					levelOrigin.y + tileImageHeight * i * tileImageScale.y,
					tileImageWidth, tileImageHeight);
				currentTile->SetOrigin(0.0f, 0.0f);
				currentTile->LoadGraphic(tileImagePath, tileImageID,
					tileImageWidth, tileImageHeight, false);
				currentTile->SetScale(tileImageScale.x, tileImageScale.y);
				currentTile->moves = false;
				currentTile->isWall = true;
				//currentTile->ShowCollisionBox(true);
				solids.push_back(currentTile);
			}
			else if (*(levelArray + (i*levelWidth) + j) == 2 || 
				*(levelArray + (i*levelWidth) + j) == 5 ||
				*(levelArray + (i*levelWidth) + j) == 6 ||
				*(levelArray + (i*levelWidth) + j) == 7)
			{
				tileImagePath = PathUtils::GetResourcesPath("images/Room_Tile.png");
				tileImageID = "Room";
				//SDL_Log("%d-%d = %d", i, j, *(levelArray + (i*width) + j));
				DibSprite* currentTile = new DibSprite
				(levelOrigin.x + tileImageWidth * j * tileImageScale.x,
					levelOrigin.y + tileImageHeight * i * tileImageScale.y,
					tileImageWidth, tileImageHeight);
				currentTile->SetOrigin(0.0f, 0.0f);
				currentTile->LoadGraphic(tileImagePath, tileImageID,
					tileImageWidth, tileImageHeight, false);
				//currentTile->SetScale(tileImageScale.x, tileImageScale.y);
				currentTile->moves = false;
				noSolids.push_back(currentTile);
			}
			else if (*(levelArray + (i*levelWidth) + j) == 3)
			{
				tileImagePath = PathUtils::GetResourcesPath("images/Room_Tile.png");
				tileImageID = "Room";
				//SDL_Log("%d-%d = %d", i, j, *(levelArray + (i*width) + j));
				DibSprite* currentTile = new DibSprite
				(levelOrigin.x + tileImageWidth * j * tileImageScale.x,
					levelOrigin.y + tileImageHeight * i * tileImageScale.y,
					tileImageWidth, tileImageHeight);
				currentTile->SetOrigin(0.0f, 0.0f);
				currentTile->LoadGraphic(tileImagePath, tileImageID,
					tileImageWidth, tileImageHeight, false);
				currentTile->SetScale(tileImageScale.x, tileImageScale.y);
				currentTile->moves = false;
				currentTile->isWall = true;
				//currentTile->ShowCollisionBox(true);
				solids.push_back(currentTile);
			}
			if (*(levelArray + (i*levelWidth) + j) == 7)
			{
				tileImagePath = PathUtils::GetResourcesPath("images/Stairs.png");
				tileImageID = "Stair";
				//SDL_Log("%d-%d = %d", i, j, *(levelArray + (i*width) + j));
				DibSprite* currentTile = new DibSprite
				(levelOrigin.x + tileImageWidth * j * tileImageScale.x,
					levelOrigin.y + tileImageHeight * i * tileImageScale.y,
					tileImageWidth, tileImageHeight);
				currentTile->SetOrigin(0.0f, 0.0f);
				currentTile->LoadGraphic(tileImagePath, tileImageID,
					tileImageWidth, tileImageHeight, false);
				currentTile->SetScale(tileImageScale.x, tileImageScale.y);
				currentTile->moves = false;
				//currentTile->ShowCollisionBox(true);
				noSolids.push_back(currentTile);
			}
		}
	}
}
