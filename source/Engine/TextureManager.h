/* ========================================================================
   File: TextureManager.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include <SDL.h>

#include <string>
#include <unordered_map>

#include "Vector2D.h"

class TextureManager
{
public:
	// Singleton Pattern - Accessor
	static TextureManager& Instance()
	{
		static TextureManager instance; // Guaranteed to be destroyed.
										// Instantiated on first use.
		return instance;
	}
	// Singleton Pattern - Delete Copy constructor and operator=
	TextureManager(TextureManager const&) = delete;
	void operator=(TextureManager const&) = delete;

	// Member functions
	bool Load(std::string fileName, std::string id, SDL_Renderer* renderer);
	bool Unload(std::string id);
	void Clean();

	void Draw(SDL_Renderer* renderer, std::string id, int x, int y, int width, int height,
		int scaleX = 1, int scaleY = 1, float angle = 0.0f, Vector2D center = { 0.5f, 0.5f }, 
		SDL_RendererFlip flip = SDL_FLIP_NONE);
	void DrawFrame(SDL_Renderer* renderer, std::string id, int x, int y, int width, int height,
		int scaleX = 1, int scaleY = 1, float angle = 0.0f, Vector2D center = {0.0f, 0.0f},
		int currentRow = 0, int currentFrame = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);

private: 

	typedef std::unordered_map<std::string, SDL_Texture*> TextureMap;
	// Member variables
	TextureMap textureMap;

	// Singleton Pattern - Constructor is private
	TextureManager() {}
};

