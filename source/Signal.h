/* ========================================================================
   File: Signal.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

#include "Modality.h"

class Signal
{
public:
	Signal(int intensity, Vector2D position, ModalityType type, float maximumRange,
		float attenuation, float inverseTransmissionSpeed);
	~Signal();

	// Data
	int intensity;
	Vector2D position;
	Modality* modality;
};

