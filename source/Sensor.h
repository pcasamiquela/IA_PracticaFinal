/* ========================================================================
   File: Sensor.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

#include "Modality.h"

class Signal;

// Agents register their interests with the RegionalSenseManager as a sensor where 
// they store their position , orientation and sensory capabilities

// Position and orientation are provided as pointers
// Sensory Capabilities consist of a threshold for each modality the agent can sense

struct SupportedModality
{
	ModalityType type = NO_MODALITY;
	float threshold = 0.0f;
};

class Sensor
{
public:
	// Lifecycle
	Sensor(Vector2D* ptrPosition, float* ptrOrientation);
	~Sensor();

	// Functions
	void AddModality(ModalityType type, float threshold);
	float GetThesholdForModality(ModalityType type);
	bool DetectsModality(ModalityType typeToCheck);
	void Notify(Signal* notificationSignal, float currentIntensity);

	// Data
	Vector2D* position;
	float* orientation;
	int threshold;
	SupportedModality supportedModalities[COUNT_MODALITY];
	char lastNotification[256]; // For debug purposes
};

