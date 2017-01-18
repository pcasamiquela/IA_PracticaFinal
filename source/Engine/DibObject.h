/* ========================================================================
   File: DibObject.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include <SDL.h>
#include <vector>

#include "Vector2D.h"
#include "Colors.h"

class DibObject
{
public:
	// State functions
	bool GetActive();
	void SetActive(bool isActive);

	// Position of the center of this object
	Vector2D GetPosition();
	void SetPosition(float x, float y);

	// Width / Height of Collision Box
	int GetWidth();
	void SetWidth(int width);
	int GetHeight();
	void SetHeight(int height);

	// Collision Box positioning
	Vector2D GetOrigin();
	void SetOrigin(float x, float y);
	Vector2D GetScale();
	void SetScale(float x, float y);
	SDL_Rect GetCollisionBox();

	// Position of the center of this object
	Vector2D position;
	// Speed of this object
	Vector2D speed;

	// Angle in degrees of the object's sprite (It does not rotate bounding box)
	float angle;

	// Angular Movement varriables
	float angularVelocity;
	float angularAcceleration;
	float angularDrag;
	float maxAngular;

	// State Flags
	bool moves;

	// Functions
	DibObject(int x = 0, int y = 0, int width = 0, int height = 0);
	virtual void Update(float deltaTime);
	virtual void Render();
	virtual void Clean();
	// Override this function to setup the object to some
	// special conditions
	virtual void Setup();

	// Collision function
	void SetSolidCollisions(std::vector<DibObject*>& solidArray);
	void SetCollidesFlag(bool collides);
	void ShowCollisionBox(bool show, Uint32 color = Colors::CARROT);

protected:
	// Flag to Enable / Disable Update and Render
	bool isActive;
	// The width of the object's Collision Box
	int width;
	// The height of the object's Collision Box
	int height;
	// Collision Box positioning
	Vector2D origin;
	Vector2D scale;

	// Collision variables
	std::vector<DibObject*>*  solidArray;
	bool showCollisionBox;
	SDL_Rect collisionBox;

	// Collision function
	bool PlaceMeeting(int x, int y);
	bool PositionMeeting(int x, int y);
	void UpdateCollisionBoxPosition();
	void UpdateCollisionBoxSize();
	
private:
	// Collision variables
	bool collides;

	// Internals for speed movement
	Vector2D speedCounter = {};
	Vector2D realSpeed = {};

	// Debug variables
	Uint32 collisionBoxColor;
};