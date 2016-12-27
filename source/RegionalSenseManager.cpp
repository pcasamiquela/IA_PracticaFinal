/* ========================================================================
   File: RegionalSenseManager.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "RegionalSenseManager.h"
#include "Modality.h"
#include "Sensor.h"
#include "SensorNotification.h"
#include "Signal.h"

RegionalSenseManager::RegionalSenseManager()
{
}

RegionalSenseManager::~RegionalSenseManager()
{
}

void RegionalSenseManager::AddSensor(Sensor* sensor)
{
	sensorList.push_back(sensor);
}

// Introduces a signal into the game.
void RegionalSenseManager::AddSignal(Signal* signal)
{
	// Aggregation Phase
	for (std::list<Sensor*>::const_iterator it = sensorList.begin(); 
			it != sensorList.end(); ++it)
	{
		Sensor* currentSensor = *it;
		// Testing phase

		// Check the modality first
		if (currentSensor->DetectsModality(signal->modality->type) == false)
		{
			continue;
		}

		// Find the distance of the signal and check range
		float signalDistance = Vector2D::Distance(signal->position, 
			*currentSensor->position);
		if (signal->modality->maximumRange < signalDistance)
		{
			continue;
		}

		// Find the intensity of the signal and check threshold
		float currentIntensity = signal->intensity 
			* powf(signal->modality->attenuation, signalDistance);
		if (currentIntensity < currentSensor->GetThesholdForModality(signal->modality->type))
		{
			continue;
		}

		// Perform additional modality specific checks
		if (signal->modality->ExtraChecks(signal, currentSensor) == false)
		{
			continue;
		}

		// We're going to notify the sensor, work out when
		const int K_SECONDS_TO_MS_FACTOR = 1000;
		Uint32 time = SDL_GetTicks() + signalDistance * signal->modality->inverseTransmissionSpeed * K_SECONDS_TO_MS_FACTOR;

		// Create a notification record and add it to the queue
		SensorNotification* notification = new SensorNotification(time, currentIntensity, currentSensor, signal);
		notificationQueue.push(notification);

		// Send signals, in case the current signal is ready to notify immediately
		SendSignals();
	}
}

// Flushes notifications from the queue, up to the current time
// This function should be called each frame
void RegionalSenseManager::SendSignals()
{
	// Notification Phase
	Uint32 currentTime = SDL_GetTicks();

	while (notificationQueue.size() > 0)
	{
		SensorNotification* currentNotification = notificationQueue.top();
		// Check if the notification is due
		if (currentNotification->time < currentTime)
		{
			currentNotification->sensor->Notify(currentNotification->signal, currentNotification->intensity);
			notificationQueue.pop();
		}
		// If we are beyond the current time, then stop
		// (The queue is sorted)
		else
		{
			break;
		}
	}
}
