/* ========================================================================
   File: FlowFieldUtils.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

#include "FlowObstacle.h"
#include "FlowField.h"

namespace FlowFieldUtils
{
	inline Vector2D DoFlowFieldFollowing(Vector2D agentPosition, Vector2D agentSpeed,
		float agentMaxSpeed, float agentMaxForce, FlowField* field)
	{
		// Calculate Steering Force
		Vector2D desiredVelocity = FlowField_GetForceForPosition(field, agentPosition);
		desiredVelocity *= agentMaxSpeed;

		if (desiredVelocity.Length() == 0.0f)
			return Vector2D();

		// Calculate Steering Force
		Vector2D steeringForce = desiredVelocity - agentSpeed;

		// Convert to Force
		// Divide by K_MAX_SPEED to get the speed factor
		steeringForce /= agentMaxSpeed;

		// Scale this factor by K_MAX_STEER_FORCE
		steeringForce *= agentMaxForce;

		return steeringForce;
	}
}
