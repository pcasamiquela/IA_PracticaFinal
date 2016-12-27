/* ========================================================================
   File: EllipseEnemyAgent.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"
#include "EnemyAgent.h"

class EllipseEnemyAgent : public EnemyAgent
{
public:
	// Constructor
	EllipseEnemyAgent(int x, int y, int width, int height, Texture* questionTexture) :
		EnemyAgent(x, y, width, height, questionTexture)
	{
		ellipse_bParam = ellipse_aParam * tanf(ellipseHalfAngle * DEG2RAD);
		ellipse_cParam = ellipse_aParam * sqrt(1 - powf(tanf(ellipseHalfAngle * DEG2RAD), 2.0f));
	}

	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual bool TargetDetected();

	void ConfigureEllipse(float aParam, float halfAngle, float backDistance);

protected:
	// Helper Functions for Ellipse Vision
	bool IsInsideEllipse(Vector2D positionToCheck);

	// Perception fields (Could go inside a blackboard)
	// Ellipse Vision variables
	float ellipse_aParam = 190.0f;
	float ellipseHalfAngle = 30.0f;
	float ellipseBackDistance = 60.0f;

	// Calculated params
	float ellipse_bParam;
	float ellipse_cParam;

private:
	// Ellipse Debug Drawing
	StaticTexture ellipseTexture;
	StaticTexture ellipseDetectedTexture;
	int ellipseTextureSize = 800;
	bool ellipseTextureIsInit = false;
	bool ellipseDetectedTextureIsInit = false;

	void RenderLOS(Uint32 color);

	// Helper Functions for Ellipse Vision
	virtual void RenderVisionEllipse();
};