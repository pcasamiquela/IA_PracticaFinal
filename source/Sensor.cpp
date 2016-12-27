/* ========================================================================
   File: Sensor.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Sensor.h"
#include "Signal.h"

Sensor::Sensor(Vector2D* ptrPosition, float* ptrOrientation):
	position{ ptrPosition }, orientation{ ptrOrientation }
{
}

Sensor::~Sensor()
{
}

void Sensor::AddModality(ModalityType type, float threshold)
{
	supportedModalities[type].type = type;
	supportedModalities[type].threshold = threshold;
}

float Sensor::GetThesholdForModality(ModalityType type)
{
	return supportedModalities[type].threshold;
}

bool Sensor::DetectsModality(ModalityType typeToCheck)
{
	// Iterate over supported modalities
	for (int i = 0; i < COUNT_MODALITY; ++i)
	{
		if (supportedModalities[i].type == typeToCheck)
		{
			return true;
		}
	}
	return false;
}

void Sensor::Notify(Signal* notificationSignal, float currentIntensity)
{
	sprintf(lastNotification,
		"Notified of signal type %d\n at position(%f, %f)\n with intensity %f",
		notificationSignal->modality->type,
		notificationSignal->position.x,
		notificationSignal->position.y,
		currentIntensity);

	SDL_Log("%s", lastNotification);
}
