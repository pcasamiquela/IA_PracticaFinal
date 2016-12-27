/* ========================================================================
   File: UncertainAdvancedAgent.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"
#include "EllipseEnemyAgent.h"

class UncertaintyAgent : public EllipseEnemyAgent
{
public:
	// Constructor
	UncertaintyAgent(int x, int y, int width, int height, Texture* questionTexture) :
		EllipseEnemyAgent(x, y, width, height, questionTexture)
	{}

	virtual bool TargetDetected();

	// Helper Functions for Uncertain vision
	int CalculateDetectionCertainty(Vector2D positionToCheck);

	// Status flags
	bool flagInsideEllipse = false;
	bool flagInsidePerimeterFar = false;
	bool flagInsidePerimeterNear = false;
	bool flagInsideCenterFar = false;
	bool flagInsideCenterNear = false;
	bool flagInsideBackZone = false;

private:
	// Helper Functions for Ellipse Vision
	virtual void RenderVisionEllipse() override;

	// Helper Functions for Uncertain vision
	bool IsInsidePerimeter(Vector2D positionToCheck);
	bool IsInsideCenter(Vector2D positionToCheck);

	// Perception fields (Could go inside a blackboard)

	// Uncertainty variables
	int insideEllipseCertainty = 100;
	int insideCenterViewNear = 80;
	int insideCenterViewFar = 30;
	int insidePerimeterViewNear = 30;
	int insidePerimeterViewFar = 0;
	int insideBackZone = 80;
	float nearTheshold = 180.0f;
	float farTheshold = 400.0f;

	// Ellipse Debug Drawing
	StaticTexture ellipseTexture;
	int ellipseTextureSize = 850;
	bool elipseTextureIsInit = false;
};