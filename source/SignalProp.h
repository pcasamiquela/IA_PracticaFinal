/* ========================================================================
   File: SignalProp.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

#include "Signal.h"

class SignalProp : public Signal
{
public:
	//Lifecycle
	SignalProp(int strength, Vector2D position, ModalityType type, float maximumRange,
		float attenuation, float inverseTransmissionSpeed);
	~SignalProp();

	void Render(SDL_Renderer* renderer, Uint32 propColor, Uint32 rangeColor, bool renderArea);
};

