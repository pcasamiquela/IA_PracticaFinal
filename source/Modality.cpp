/* ========================================================================
   File: Modality.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Modality.h"

Modality::Modality(ModalityType type, float maximumRange,
	float attenuation, float inverseTransmissionSpeed):
	type{type}, maximumRange{maximumRange}, attenuation{attenuation},
	inverseTransmissionSpeed{inverseTransmissionSpeed}
{
}

Modality::~Modality()
{
}

bool Modality::ExtraChecks(Signal* signal, Sensor* sensor)
{
	return true;
}
