/* ========================================================================
   File: Signal.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Signal.h"
#include "SightModality.h"

Signal::Signal(int intensity, Vector2D position, ModalityType type, float maximumRange,
	float attenuation, float inverseTransmissionSpeed):
	intensity{intensity}, position{position}
{
	if (type != MODALITY_SIGHT)
	{
		modality = new Modality(type, maximumRange, attenuation, inverseTransmissionSpeed);
	}
	else
	{
		modality = new SightModality(maximumRange, attenuation, inverseTransmissionSpeed);
	}
}

Signal::~Signal()
{
	delete modality;
}
