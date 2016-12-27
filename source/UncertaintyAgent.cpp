/* ========================================================================
   File: UncertaintyAgent.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "UncertaintyAgent.h"

bool UncertaintyAgent::TargetDetected()
{
	if (flagInsideEllipse || (flagInsidePerimeterNear && flagInsideCenterNear) )
	{
		if (HasLOSWithTarget())
		{
			return true;
		}
	}
	return false;
}

void UncertaintyAgent::RenderVisionEllipse()
{
	if (elipseTextureIsInit == false)
	{
		ellipseTexture = {};
		StaticTexture_Init(&ellipseTexture, ellipseTextureSize, ellipseTextureSize, 
			Game::Instance().renderer);
		StaticTexture_StartDrawing(&ellipseTexture);
		filledEllipseColor(Game::Instance().renderer, 
			ellipseTextureSize/2 + ellipse_aParam - ellipseBackDistance, 
			ellipseTextureSize/2, ellipse_aParam, ellipse_bParam, Colors::SUNFLOWER);
		arcColor(Game::Instance().renderer, ellipseTextureSize / 2,
			ellipseTextureSize / 2, nearTheshold, 270, 90, Colors::WET_ASPHALT);
		arcColor(Game::Instance().renderer, ellipseTextureSize / 2,
			ellipseTextureSize / 2, farTheshold, 270, 90, Colors::WET_ASPHALT);
		lineColor(Game::Instance().renderer, ellipseTextureSize / 2,
			ellipseTextureSize / 2,
			ellipseTextureSize / 2 + farTheshold * cosf(60.0f* (float)DEG2RAD),
			ellipseTextureSize / 2 + farTheshold * sinf(60.0f* (float)DEG2RAD),
			Colors::WET_ASPHALT);
		lineColor(Game::Instance().renderer, ellipseTextureSize / 2,
			ellipseTextureSize / 2,
			ellipseTextureSize / 2 + farTheshold * cosf(-60.0f* (float)DEG2RAD),
			ellipseTextureSize / 2 + farTheshold * sinf(-60.0f* (float)DEG2RAD),
			Colors::WET_ASPHALT);
		lineColor(Game::Instance().renderer, ellipseTextureSize / 2,
			ellipseTextureSize / 2 - farTheshold,
			ellipseTextureSize / 2,
			ellipseTextureSize / 2 + farTheshold,
			Colors::WET_ASPHALT);
		StaticTexture_EndDrawing(&ellipseTexture);
		elipseTextureIsInit = true;
	}

	StaticTexture_Render(&ellipseTexture, Game::Instance().renderer,
		position.x - ellipseTextureSize/2, 
		position.y - ellipseTextureSize/2, angle);

	// Debug drawing of center and focal points of the ellipse
	//Vector2D vectorFromAngle = { cosf(angle* DEG2RAD), sinf(angle* DEG2RAD) };
	//Vector2D center = position + vectorFromAngle * (ellipse_aParam - ellipseBackDistance);
	//Vector2D focal1 = position + vectorFromAngle * (ellipse_aParam - ellipseBackDistance + ellipse_cParam);
	//Vector2D focal2 = position + vectorFromAngle * (ellipse_aParam - ellipseBackDistance - ellipse_cParam);

	//filledCircleColor(Game::Instance().renderer, center.x, center.y, 20.0f, Colors::ASBESTOS);
	//filledCircleColor(Game::Instance().renderer, focal1.x, focal1.y, 40.0f, Colors::BELIZE_HOLE);
	//filledCircleColor(Game::Instance().renderer, focal2.x, focal2.y, 40.0f, Colors::BELIZE_HOLE);
}

bool UncertaintyAgent::IsInsidePerimeter(Vector2D positionToCheck)
{
	Vector2D normalizedForward = { cosf(angle* DEG2RAD), sinf(angle* DEG2RAD) };
	Vector2D targetVector = positionToCheck - position;
	targetVector.Normalize();
	float dotProduct = Vector2D::Dot(normalizedForward, targetVector);
	return (dotProduct > 0.0f);
}

bool UncertaintyAgent::IsInsideCenter(Vector2D positionToCheck)
{
	Vector2D normalizedForward = { cosf(angle* DEG2RAD), sinf(angle* DEG2RAD) };
	Vector2D targetVector = positionToCheck - position;
	targetVector.Normalize();
	float dotProduct = Vector2D::Dot(normalizedForward, targetVector);
	return (dotProduct > 0.5f);
}

int UncertaintyAgent::CalculateDetectionCertainty(Vector2D positionToCheck)
{
	// Flags for debug purposes
	flagInsideEllipse = false;
	flagInsidePerimeterFar = false;
	flagInsidePerimeterNear = false;
	flagInsideCenterFar = false;
	flagInsideCenterNear = false;
	flagInsideBackZone = false;

	if (Vector2D::Distance(position, positionToCheck) > farTheshold)
	{
		return 0.0f;
	}
	else if (IsInsidePerimeter(positionToCheck))
	{
		if (IsInsideCenter(positionToCheck))
		{
			if (IsInsideEllipse(positionToCheck))
			{
				flagInsideEllipse = true;
				return insideEllipseCertainty;
			}
			else
			{
				if (Vector2D::Distance(position, positionToCheck) < nearTheshold)
				{
					flagInsideCenterNear = true;
					return insideCenterViewNear;
				}
				else
				{
					flagInsideCenterFar = true;
					return insideCenterViewFar;
				}
			}
		}
		else 
		{
			if (Vector2D::Distance(position, positionToCheck) < nearTheshold)
			{
				flagInsidePerimeterNear = true;
				return insidePerimeterViewNear;
			}
			else
			{
				flagInsidePerimeterFar = true;
				return insidePerimeterViewFar;
			}
		}
	}
	else
	{
		if (IsInsideEllipse(positionToCheck))
		{
			flagInsideBackZone = true;
			return insideBackZone;
		}
		else
		{
			return 0.0f;
		}
	}
}

