/* ========================================================================
   File: TextureManager.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "TextureManager.h"

#include <SDL_image.h>

bool TextureManager::Load(std::string fileName, std::string id, SDL_Renderer* renderer)
{
	bool success = false;

	if (textureMap.find(id) == textureMap.end())
	{
		SDL_Surface* temporarySurface = IMG_Load(fileName.c_str());
		if (temporarySurface == nullptr)
		{
			SDL_Log("Unable to load image %s.\nIMG_Error: %s\n", fileName, IMG_GetError());
			success = false;
		}
		else
		{
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, temporarySurface);
			if (texture == nullptr)
			{
				SDL_Log("Unable to create texture from %s.\nSDL_Error: %s\n", fileName, SDL_GetError());
				success = false;
			}
			else
			{
				textureMap[id] = texture;
				success = true;
			}
		}

		SDL_FreeSurface(temporarySurface);
	}
	else
	{
		//SDL_Log("Texture with id %s already exists in the TextureManager.", id.c_str());
	}

	return success;
}

bool TextureManager::Unload(std::string id)
{
	TextureMap::iterator entry = textureMap.find(id);
	if (entry != textureMap.end())
	{
		SDL_DestroyTexture(textureMap[id]);
		textureMap.erase(entry);
		return true;
	}
	else
	{
		SDL_Log("Texture with id %s not present in the TextureManager.", id.c_str());
		return false;
	}
}

void TextureManager::Clean()
{
	for (auto entry : textureMap)
	{
		SDL_DestroyTexture(entry.second);
		textureMap.erase(entry.first);
	}
}

void TextureManager::Draw(SDL_Renderer* renderer, std::string id, int x, int y, int width, int height,
	int scaleX, int scaleY, float angle, Vector2D center, SDL_RendererFlip flip)
{
	SDL_Rect sourceRect;
	SDL_Rect destinationRect;

	sourceRect.x = 0;
	sourceRect.y = 0;
	sourceRect.w = width;
	sourceRect.h = height;

	destinationRect.w = width * scaleX;
	destinationRect.h = height * scaleY;

	int centerDeltaX = (int)roundf(width * scaleX * center.x);
	int centerDeltaY = (int)roundf(height * scaleY * center.y);
	SDL_Point centerPoint = { centerDeltaX, centerDeltaY };
	destinationRect.x = x - centerPoint.x;
	destinationRect.y = y - centerPoint.y;

	SDL_RenderCopyEx(renderer, textureMap[id], &sourceRect, &destinationRect, angle, &centerPoint, flip);
}

void TextureManager::DrawFrame(SDL_Renderer* renderer, std::string id, int x, int y, int width, int height, 
	int scaleX, int scaleY, float angle, Vector2D center, 
	int currentRow, int currentFrame, SDL_RendererFlip flip)
{
	SDL_Rect sourceRect;
	SDL_Rect destinationRect;

	sourceRect.x = width * currentFrame;
	sourceRect.y = height * currentRow;
	sourceRect.w = width;
	sourceRect.h = height;

	destinationRect.w = width * scaleX;
	destinationRect.h = height * scaleY;

	int centerDeltaX = (int)roundf(width * scaleX * center.x);
	int centerDeltaY = (int)roundf(height * scaleY * center.y);
	SDL_Point centerPoint = { centerDeltaX, centerDeltaY };
	destinationRect.x = x - centerPoint.x;
	destinationRect.y = y - centerPoint.y;

	SDL_RenderCopyEx(renderer, textureMap[id], &sourceRect, &destinationRect, angle, &centerPoint, flip);
}
