/* ========================================================================
   File: SensorNotification.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

class Sensor;
class Signal;

class SensorNotification
{
public:
	SensorNotification(float time, float intensity, Sensor* sensor, Signal* signal);
	~SensorNotification();

	// Data
	float time;
	float intensity;
	Sensor* sensor;
	Signal* signal;
};

class ExpiredFirstPriority
{
public:
	bool operator() (SensorNotification* lhs, SensorNotification* rhs)
	{
		return (lhs->time > rhs->time);
	}
};

