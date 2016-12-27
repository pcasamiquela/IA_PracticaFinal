/* ========================================================================
   File: BasicAgent.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "BasicAgent.h"

void BasicAgent::Render()
{
	RenderVisionCone(Colors::SetAlpha(Colors::SUNFLOWER, 100));
	if (targetDetected)
	{
		RenderLOS(Colors::SetAlpha(Colors::WISTERIA, 100));
	}
	Boid::Render();
	if (targetLost == true)
	{
		// Render question mark texture
		Texture_Render(questionTexture, Game::Instance().renderer,
			(int)position.x - (width * scale.x) / 2,
			(int)position.y - 2 * (height * scale.y));
	}
}

bool BasicAgent::HasLOSWithTarget()
{
	// Check if there is line of sight between enemy and target (Player)
	losVector = *targetPosition;
	int totalObstacles = *losObstacleArraySize;
	for (int i = 0; i < totalObstacles; ++i)
	{
		// Check intersetcion for each obstacle
		if (LOS_Obstacle_IntersectsSegment(&losObstacleArray[i], position, losVector))
		{
			return false;
		}
	}
	return true;
}


bool BasicAgent::TargetDetected()
{
	Vector2D coneBasePoint = { position.x + visionConeHeight, position.y };
	Vector2D currentConeBase = Vector2DUtils::RotatePoint(position, coneBasePoint, angle);
	if (Vector2DUtils::IsInsideCone(*targetPosition, position, currentConeBase, visionConeHalfAngle))
	{
		if (HasLOSWithTarget())
		{
			return true;
		}
	}
	return false;
}

void BasicAgent::RenderVisionCone(Uint32 coneColor)
{
	Vector2D triangleX1 = { position.x + visionConeHeight, position.y - tanf(visionConeHalfAngle * DEG2RAD) * visionConeHeight };
	Vector2D triangleX2 = { position.x + visionConeHeight, position.y + tanf(visionConeHalfAngle * DEG2RAD) * visionConeHeight };
	Vector2D rotatedTriangle1 = Vector2DUtils::RotatePoint(position, triangleX1, angle);
	Vector2D rotatedTriangle2 = Vector2DUtils::RotatePoint(position, triangleX2, angle);

	filledTrigonColor(Game::Instance().renderer, position.x, position.y,
		rotatedTriangle1.x, rotatedTriangle1.y,
		rotatedTriangle2.x, rotatedTriangle2.y,
		targetDetected ? Colors::SetAlpha(Colors::ALIZARIN, 100) : coneColor);
}

void BasicAgent::RenderLOS(Uint32 color)
{
	thickLineColor(Game::Instance().renderer, position.x, position.y, losVector.x, losVector.y, 5, color);
}