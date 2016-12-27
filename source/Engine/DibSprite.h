/* ========================================================================
   File: DibSprite.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "DibObject.h"
#include "Vector2D.h"
#include "Colors.h"
#include <SDL.h>
#include <string>

// Forward declaration of DibAnimationController
class DibAnimationController;

class DibSprite : public DibObject
{
public:
	// Animation variables
	DibAnimationController* animationController;

	DibSprite(int x = 0, int y = 0, int width = 0, int height = 0):
		DibObject(x, y, width, height),
		animates {false},
		currentFlip{ SDL_FLIP_NONE },
		showSpriteBox {false}
	{}
	
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Clean() override;

	// Sprite functions
	void LoadGraphic(std::string texturePath, std::string textureID,
		int spriteWidth = 0, int spriteHeight = 0, bool animates = false);
	void FlipHorizontal();
	void FlipVertical();
	void ShowSpriteBox(bool show, Uint32 color = Colors::ALIZARIN);

protected:
	// Animation variables
	bool animates;
	// Sprite variables
	int spriteWidth;
	int spriteHeight;
	std::string textureID;
	SDL_RendererFlip currentFlip;

private:
	// Debug variables
	bool showSpriteBox;
	Uint32 spriteBoxColor;
};
