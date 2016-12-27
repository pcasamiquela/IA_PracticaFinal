/* ========================================================================
   File: DibSprite.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "DibSprite.h"
#include "TextureManager.h"
#include "Game.h"
#include "DibAnimationController.h"

// For debug
#include <cassert>
#include "SDL_gfx/SDL2_gfxPrimitives.h"

void DibSprite::Update(float deltaTime)
{
	if (!isActive)
		return;

	// Movement
	if (moves)
	{
		DibObject::Update(deltaTime);
	}

	// Animation
	if (animates)
	{
		assert(animationController != nullptr && "animationController == nullptr");
		animationController->Update(deltaTime);
	}
}

void DibSprite::Render()
{
	if (!isActive)
		return;

	if (animates)
	{
		// Render frame of animation
		assert(animationController != nullptr && "animationController == nullptr");
		assert(animationController->currentAnimation != nullptr && "currentAnimation == nullptr - Add at least one animation!");
		TextureManager::Instance().DrawFrame(Game::Instance().renderer,
			textureID,
			position.x, position.y,
			spriteWidth, spriteHeight, scale.x, scale.y, angle, origin,
			animationController->currentAnimation->row, animationController->currentAnimation->currentIndex,
			currentFlip);
	}
	else
	{
		// Render static image
		TextureManager::Instance().Draw(Game::Instance().renderer, textureID,
			position.x, position.y,
			spriteWidth, spriteHeight, scale.x, scale.y, angle, origin);
	}

	if (showSpriteBox)
	{
		rectangleColor(Game::Instance().renderer,
			position.x - spriteWidth * scale.x * origin.x,
			position.y - spriteHeight * scale.y * origin.y,
			position.x + spriteWidth * scale.x * (1.0f - origin.x),
			position.y + spriteHeight * scale.y * (1.0f - origin.y),
			spriteBoxColor);
	}

	if (showCollisionBox)
	{
		DibObject::Render();
	}
}

void DibSprite::Clean()
{
	if (animationController != nullptr)
	{
		animationController->Clear();
		delete animationController;
	}
}

void DibSprite::LoadGraphic(std::string texturePath, std::string textureID,
	int spriteWidth, int spriteHeight, bool animates)
{
	this->spriteWidth = spriteWidth;
	this->spriteHeight = spriteHeight;
	this->textureID = textureID;
	this->animates = animates;
	TextureManager::Instance().Load(texturePath, this->textureID, Game::Instance().renderer);

	if (this->animates)
	{
		animationController = new DibAnimationController(this);
	}
}

void DibSprite::FlipHorizontal()
{
	currentFlip = (SDL_RendererFlip)(currentFlip ^ SDL_FLIP_HORIZONTAL);
}

void DibSprite::FlipVertical()
{
	currentFlip = (SDL_RendererFlip)(currentFlip ^ SDL_FLIP_VERTICAL);;
}

void DibSprite::ShowSpriteBox(bool show, Uint32 color)
{
	showSpriteBox = show;
	spriteBoxColor = color;
}

