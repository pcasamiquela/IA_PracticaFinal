/* ========================================================================
   File: Modality.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

class Sensor;
class Signal;

enum ModalityType
{
	NO_MODALITY,
	MODALITY_HEAR,
	MODALITY_SMELL,
	MODALITY_SIGHT,
	COUNT_MODALITY
};

class Modality
{
public:
	// Lifecycle
	Modality(ModalityType type, float maximumRange, 
		float attenuation, float inverseTransmissionSpeed);
	~Modality();

	// Functions
	virtual bool ExtraChecks(Signal* signal, Sensor* sensor);

	// Data
	ModalityType type;
	float maximumRange;
	float attenuation;
	float inverseTransmissionSpeed;
};

