/* ========================================================================
   File: EllipseEnemyAgent.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "EllipseEnemyAgent.h"

void EllipseEnemyAgent::Update(float deltaTime)
{
	EnemyAgent::Update(deltaTime);
}

void EllipseEnemyAgent::Render()
{
	RenderVisionEllipse();
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

bool EllipseEnemyAgent::TargetDetected()
{
	if (IsInsideEllipse(*targetPosition))
	{
		if (HasLOSWithTarget())
		{
			return true;
		}
	}
	return false;
}

void EllipseEnemyAgent::ConfigureEllipse(float aParam, float halfAngle, float backDistance)
{
	ellipse_aParam = aParam;
	ellipseHalfAngle = halfAngle;
	ellipseBackDistance = backDistance;

	ellipse_bParam = ellipse_aParam * tanf(ellipseHalfAngle * DEG2RAD);
	ellipse_cParam = ellipse_aParam * sqrt(1 - powf(tanf(ellipseHalfAngle * DEG2RAD), 2.0f));
}

void EllipseEnemyAgent::RenderVisionEllipse()
{
	if (targetDetected == true)
	{
		if (ellipseDetectedTextureIsInit == false)
		{
			ellipseDetectedTexture = {};
			StaticTexture_Init(&ellipseDetectedTexture, ellipseTextureSize, ellipseTextureSize,
				Game::Instance().renderer);
			StaticTexture_StartDrawing(&ellipseDetectedTexture);
			filledEllipseColor(Game::Instance().renderer,
				ellipseTextureSize / 2 + ellipse_aParam - ellipseBackDistance,
				ellipseTextureSize / 2, ellipse_aParam, ellipse_bParam, Colors::ALIZARIN);
			StaticTexture_EndDrawing(&ellipseDetectedTexture);
			ellipseDetectedTextureIsInit = true;
		}

		StaticTexture_Render(&ellipseDetectedTexture, Game::Instance().renderer,
			position.x - ellipseTextureSize / 2,
			position.y - ellipseTextureSize / 2, angle);
	}
	else
	{
		if (ellipseTextureIsInit == false)
		{
			ellipseTexture = {};
			StaticTexture_Init(&ellipseTexture, ellipseTextureSize, ellipseTextureSize, 
				Game::Instance().renderer);
			StaticTexture_StartDrawing(&ellipseTexture);
			filledEllipseColor(Game::Instance().renderer, 
				ellipseTextureSize / 2 + ellipse_aParam - ellipseBackDistance, 
				ellipseTextureSize / 2, ellipse_aParam, ellipse_bParam, Colors::SUNFLOWER);
			StaticTexture_EndDrawing(&ellipseTexture);
			ellipseTextureIsInit = true;
		}

		StaticTexture_Render(&ellipseTexture, Game::Instance().renderer,
			position.x - ellipseTextureSize / 2,
			position.y - ellipseTextureSize / 2, angle);
	}

	// Debug drawing of center and focal points of the ellipse
	//Vector2D vectorFromAngle = { cosf(angle* DEG2RAD), sinf(angle* DEG2RAD) };
	//Vector2D center = position + vectorFromAngle * (ellipse_aParam - ellipseBackDistance);
	//Vector2D focal1 = position + vectorFromAngle * (ellipse_aParam - ellipseBackDistance + ellipse_cParam);
	//Vector2D focal2 = position + vectorFromAngle * (ellipse_aParam - ellipseBackDistance - ellipse_cParam);

	//filledCircleColor(Game::Instance().renderer, center.x, center.y, 20.0f, Colors::ASBESTOS);
	//filledCircleColor(Game::Instance().renderer, focal1.x, focal1.y, 40.0f, Colors::BELIZE_HOLE);
	//filledCircleColor(Game::Instance().renderer, focal2.x, focal2.y, 40.0f, Colors::BELIZE_HOLE);
}

void EllipseEnemyAgent::RenderLOS(Uint32 color)
{
	thickLineColor(Game::Instance().renderer, position.x, position.y, losVector.x, losVector.y, 5, color);
}

bool EllipseEnemyAgent::IsInsideEllipse(Vector2D positionToCheck)
{
	Vector2D vectorFromAngle = { cosf(angle* DEG2RAD), sinf(angle* DEG2RAD) };
	// Find Ellipse Center
	Vector2D center = position + vectorFromAngle * (ellipse_aParam - ellipseBackDistance);
	// Find 2 Focal Points
	Vector2D focal1 = position + vectorFromAngle * (ellipse_aParam - ellipseBackDistance + ellipse_cParam);
	Vector2D focal2 = position + vectorFromAngle * (ellipse_aParam - ellipseBackDistance - ellipse_cParam);
	// Check if sum of distances is bigger than 2 * a
	float distanceToF1 = Vector2D::Distance(focal1, positionToCheck);
	float distanceToF2 = Vector2D::Distance(focal2, positionToCheck);
	float sumOfDistances = distanceToF1 + distanceToF2;
	// For all points outside the Ellipse the sum of distances is > 2 * a
	return (sumOfDistances <= 2 * ellipse_aParam);
}
