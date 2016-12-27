/* ========================================================================
   File: SightModality.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "SightModality.h"

SightModality::SightModality(float maximumRange,
	float attenuation, float inverseTransmissionSpeed):
	Modality(MODALITY_SIGHT, maximumRange, attenuation, inverseTransmissionSpeed)
{
}

SightModality::~SightModality()
{
}

bool SightModality::ExtraChecks(Signal* signal, Sensor* sensor)
{
	// PENDING: Add Cone AND LOS Checks
	return true;
}