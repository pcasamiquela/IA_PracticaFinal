/* ========================================================================
   File: ConeEnemyAgent.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "ConeEnemyAgent.h"

void ConeEnemyAgent::Update(float deltaTime)
{
	EnemyAgent::Update(deltaTime);
}

void ConeEnemyAgent::Render()
{
	RenderVisionCone(Colors::SetAlpha(Colors::SUNFLOWER,600));
	if (targetDetected)
	{
		RenderLOS(Colors::WISTERIA);
	}
	EnemyAgent::Render();

	if (targetLost == true)
	{
		// Render question mark texture
		Texture_Render(questionTexture, Game::Instance().renderer,
			(int)position.x - (width * scale.x) / 2,
			(int)position.y - 2 * (height * scale.y));
	}
}

bool ConeEnemyAgent::TargetDetected()
{
	Vector2D coneBasePoint = { position.x + visionConeHeight, position.y };
	Vector2D currentConeBase = Vector2DUtils::RotatePoint(position, coneBasePoint, angle);
	if (Vector2DUtils::IsInsideCone(*targetPosition, position, currentConeBase, visionConeHalfAngle))
	{
		if (HasLOSWithTarget() && playerIsActive)
		{
			return true;
		}
	}
	return false;
}

void ConeEnemyAgent::Setup()
{
	SDL_Log("Boid - Setup");
	SetPosition(0.0f, 0.0f);
	SetWidth(32);
	SetHeight(24);
	std::string assetPath = PathUtils::GetResourcesPath("images/Boid.png");
	LoadGraphic(assetPath, "Boid", 32, 24, false);
	SetScale(GLOBAL_SCALE, GLOBAL_SCALE);
	SetOrigin(0.5f, 0.5f);
	SetActive(false);
}

void ConeEnemyAgent::RenderVisionCone(Uint32 coneColor)
{
	Vector2D triangleX1 = { position.x + visionConeHeight, position.y - tanf(visionConeHalfAngle * DEG2RAD) * visionConeHeight };
	Vector2D triangleX2 = { position.x + visionConeHeight, position.y + tanf(visionConeHalfAngle * DEG2RAD) * visionConeHeight };
	Vector2D rotatedTriangle1 = Vector2DUtils::RotatePoint(position, triangleX1, angle);
	Vector2D rotatedTriangle2 = Vector2DUtils::RotatePoint(position, triangleX2, angle);

	filledTrigonColor(Game::Instance().renderer, position.x, position.y,
		rotatedTriangle1.x, rotatedTriangle1.y,
		rotatedTriangle2.x, rotatedTriangle2.y,
		targetDetected ? Colors::SetAlpha(Colors::ALIZARIN,600):coneColor);
}

void ConeEnemyAgent::RenderLOS(Uint32 color)
{
	thickLineColor(Game::Instance().renderer, position.x, position.y, 
		losVector.x, losVector.y, 5, color);
}