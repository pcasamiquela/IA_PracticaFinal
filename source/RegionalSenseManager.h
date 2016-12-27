/* ========================================================================
   File: RegionalSenseManager.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

#include "SensorNotification.h"

#include <queue>
#include <list>

class Sensor;
class Signal;

class RegionalSenseManager
{
public:
	// Lifecycle
	RegionalSenseManager();
	~RegionalSenseManager();

	// Functions
	void AddSensor(Sensor* sensor);
	void AddSignal(Signal* signal);
	void SendSignals();

private:
	// Data
	std::priority_queue<SensorNotification*, 
		std::vector<SensorNotification*>, 
		ExpiredFirstPriority> notificationQueue;
	std::list<Sensor*> sensorList;
};

