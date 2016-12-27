/* ========================================================================
   File: DibObject.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "DibObject.h"
#include "Utilities.h"
#include "Game.h"

   // For debug
#include <cassert>
#include "SDL_gfx/SDL2_gfxPrimitives.h"

DibObject::DibObject(int x, int y, int width, int height) :
	isActive{ true },
	position{ x, y },
	width{ width },
	height{ height },
	speed {0.0f, 0.0f},
	angle{ 0.0f },
	angularVelocity{ 0.0f },
	angularAcceleration{ 0.0f },
	angularDrag{ 0.0f },
	maxAngular{ 0.0f },
	scale{ 1.0f, 1.0f },
	origin{ 0.5f, 0.5f },
	moves{ false },
	collides{ false },
	showCollisionBox {false},
	collisionBox {}
	{
		UpdateCollisionBoxSize();
		UpdateCollisionBoxPosition();
	}

void DibObject::Update(float deltaTime)
{
	if (!isActive)
		return;

	// Move entity
	if (moves)
	{
		// Add speed to speed counter and get number of pixels to move this frame (integer)
		speedCounter += (speed * deltaTime);
		realSpeed = { FloatUtils::floor0(speedCounter.x),   FloatUtils::floor0(speedCounter.y) };

		// Remainder for next frame
		speedCounter -= realSpeed;

		if (collides)
		{
			UpdateCollisionBoxPosition();

			// Check horizontal collisions
			if (PlaceMeeting(position.x + realSpeed.x, position.y))
			{
				int currentSign = FloatUtils::Sign(realSpeed.x);
				while (!PlaceMeeting(position.x + currentSign, position.y))
				{
					position.x += currentSign;
				}
				// Collided, set all horizontal speed to 0
				speed.x = 0.0f;
				realSpeed.x = 0.0f;
				speedCounter.x = 0.0f;
			}
			position.x += realSpeed.x;

			// Check vertical collisions
			if (PlaceMeeting(position.x, position.y + realSpeed.y))
			{
				int currentSign = FloatUtils::Sign(realSpeed.y);
				while (!PlaceMeeting(position.x, position.y + currentSign))
				{
					position.y += currentSign;
				}
				// Collided, set all vertical speed to 0
				speed.y = 0.0f;
				realSpeed.y = 0.0f;
				speedCounter.y = 0.0f;
			}
			position.y += realSpeed.y;

			UpdateCollisionBoxPosition();
		}
		else
		{
			position += realSpeed;
		}

		// Angular movement
		float velocityDelta = Utils::ComputeVelocity(angularVelocity, angularAcceleration,
			angularDrag, maxAngular, deltaTime) - angularVelocity;
		angularVelocity += velocityDelta;
		angle += angularVelocity * deltaTime;
	}
}

void DibObject::Render()
{
	if (!isActive)
		return;

	if (showCollisionBox)
	{
		filledCircleColor(Game::Instance().renderer, 
			position.x, position.y, 4, collisionBoxColor);

		collisionBox.x = position.x - collisionBox.w * origin.x;
		collisionBox.y = position.y - collisionBox.h * origin.y;

		rectangleColor(Game::Instance().renderer,
			collisionBox.x,
			collisionBox.y,
			collisionBox.x + collisionBox.w,
			collisionBox.y + collisionBox.h,
			collisionBoxColor);
	}
}

void DibObject::Clean()
{

}

void DibObject::Setup()
{

}

bool DibObject::GetActive()
{
	return isActive;
}

void DibObject::SetActive(bool isActive)
{
	this->isActive = isActive;
}

void DibObject::SetPosition(float x, float y)
{
	position = { x, y };
}

Vector2D DibObject::GetPosition()
{
	return position;
}

void DibObject::SetWidth(int width)
{
	this->width = width;
	UpdateCollisionBoxSize();
	UpdateCollisionBoxPosition();
}

void DibObject::SetHeight(int height)
{
	this->height = height;
	UpdateCollisionBoxSize();
	UpdateCollisionBoxPosition();
}

int DibObject::GetWidth()
{
	return width;
}

int DibObject::GetHeight()
{
	return height;
}

void DibObject::SetOrigin(float x, float y)
{
	this->origin = { x, y };
}

Vector2D DibObject::GetOrigin()
{
	return origin;
}

void DibObject::SetScale(float x, float y)
{
	this->scale = { x, y };
	UpdateCollisionBoxSize();
	UpdateCollisionBoxPosition();
}

SDL_Rect DibObject::GetCollisionBox()
{
	return collisionBox;
}

Vector2D DibObject::GetScale()
{
	return scale;
}

void DibObject::SetSolidCollisions(std::vector<DibObject*>& solidArray)
{
	if (solidArray.size() > 0)
	{
		this->solidArray = &solidArray;
		collides = true;
	}
	else
	{
		collides = false;
	}
}

void DibObject::SetCollidesFlag(bool collides)
{
	this->collides = collides;
}

void DibObject::ShowCollisionBox(bool show, Uint32 color)
{
	showCollisionBox = show;
	collisionBoxColor = color;
}

bool DibObject::PlaceMeeting(int x, int y)
{
	// Calculate collision box current position
	collisionBox.x = x - collisionBox.w * origin.x;
	collisionBox.y = y - collisionBox.h * origin.y;
	
	bool collided = false;
	for (int i = 0; i < (int)(*solidArray).size(); ++i)
	{
		if (CollisionUtils::SDL_CollideBoundingBox(collisionBox, (*solidArray)[i]->collisionBox))
		{
			// Collision Found
			collided = true;
			break;
		}
	}

	return collided;
}

bool DibObject::PositionMeeting(int x, int y)
{
	SDL_Rect pointCollisionBox = {};
	pointCollisionBox.x = x;
	pointCollisionBox.y = y;
	pointCollisionBox.w = 0;
	pointCollisionBox.h = 0;

	bool collided = false;
	for (int i = 0; i < (int)(*solidArray).size(); ++i)
	{
		if (CollisionUtils::SDL_CollideBoundingBox(pointCollisionBox, (*solidArray)[i]->collisionBox))
		{
			// Collision Found
			collided = true;
			break;
		}
	}

	return collided;
}

void DibObject::UpdateCollisionBoxPosition()
{
	collisionBox.x = position.x - collisionBox.w * origin.x;
	collisionBox.y = position.y - collisionBox.h * origin.y;
}

void DibObject::UpdateCollisionBoxSize()
{
	collisionBox.w = width * scale.x;
	collisionBox.h = height * scale.y;
}
