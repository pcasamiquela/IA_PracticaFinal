/* ========================================================================
   File: EngineFramework.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

// Dependencies
// SDL_gfx
#include "SDL_gfx/SDL2_rotozoom.h"
#include "SDL_gfx/SDL2_gfxPrimitives.h"
#include "SDL_gfx/SDL2_gfxPrimitives_font.h"
// SDL_FontCache
#include "SDL_FontCache/SDL_FontCache.h"

// System includes
#include "Game.h"
#include "Input.h"
#include "TextureManager.h"
#include "SoundManager.h"
// Utilities includes
#include "Timer.h"
#include "Utilities.h"
#include "Vector2D.h"
#include "Colors.h"
#include "Trail.h"
// Graphics includes
#include "Texture.h"
#include "StaticTexture.h"
// State includes
#include "BaseState.h"
#include "StateManager.h"
// Entity includes
#include "DibObject.h"
#include "DibSprite.h"
#include "GenericPool.h"
// Animation includes
#include "DibAnimationController.h"
#include "DibAnimation.h"
// Level includes
#include "Level.h"
// Text includes
#include "BitmapFont.h"
#include "RotatedText.h"