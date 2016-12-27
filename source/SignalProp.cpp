/* ========================================================================
   File: SignalProp.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "SignalProp.h"

SignalProp::SignalProp(int strength, Vector2D position, ModalityType type, float maximumRange,
	float attenuation, float inverseTransmissionSpeed):
	Signal(strength, position, type, maximumRange, attenuation, inverseTransmissionSpeed)
{
}

SignalProp::~SignalProp()
{
	//Signal::~Signal();
}

void SignalProp::Render(SDL_Renderer* renderer, Uint32 propColor, Uint32 rangeColor, bool renderArea)
{
	if (renderArea)
	{
		filledCircleColor(renderer, position.x, position.y, modality->maximumRange, rangeColor);
	}
	filledCircleColor(renderer, position.x, position.y, 4, propColor);
}
