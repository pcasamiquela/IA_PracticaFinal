/* ========================================================================
   File: SightModality.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

#include "Modality.h"

class SightModality: public Modality
{
public:
	SightModality(float maximumRange,
		float attenuation, float inverseTransmissionSpeed);
	~SightModality();

	// Functions
	virtual bool ExtraChecks(Signal* signal, Sensor* sensor) override;
};

