/* ========================================================================
   File: SensorNotification.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "SensorNotification.h"

SensorNotification::SensorNotification(float time, float intensity, Sensor* sensor, Signal* signal):
	time{ time }, intensity{ intensity }, sensor {sensor}, signal{ signal }
{
}


SensorNotification::~SensorNotification()
{
}
