/* ========================================================================
File: Boid.h
Revision: 0.1
Creator: David Collado Ochoa
Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"
#include "Path.h"
#include "SimplePath.h"
#include "Obstacle.h"
#include "FlowField.h"
#include "SteeringGroup.h"

enum Behavior
{
	NONE,
	// Basic Steering
	SEEK_KINEMATIC,
	SEEK_STEERING,
	FLEE_KINEMATIC,
	FLEE_STEERING,
	ARRIVE,
	PURSUE,
	EVADE,
	WANDER,
	SIMPLE_WANDER,
	// Advanced Steering
	SIMPLE_PATH_FOLLOWING,
	PATH_FOLLOWING,
	COLLISION_AVOIDANCE,
	OBSTACLE_AVOIDANCE,
	FLOCKING,
	FLOCKING_SEEK,
	FLOW_FLOCK_SEEK,
	// Priority Steering
	FLOW_FLOCK_SEEK_PRIORITY,
	PRIORITY_COLLISION,
	PRIORITY_OBSTACLE,
	PRIORITY_COLLISION_PATH_FOLLOWING,
	FLOCKING_ARRIVE,
};

// Make it global for easy debug manipulation
extern float K_FLOCK_COHESION;
extern float K_FLOCK_ALIGNMENT;
extern float K_FLOCK_SEPARATION;

class Boid : public DibSprite
{
public:
	Boid(int x = 0, int y = 0, int width = 0, int height = 0) :
		DibSprite(x, y, width, height)
	{
		moves = true;
		isActive = true;
	};

	// Overriden functions
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Clean() override;
	void Setup() override;

	// Constants
	const float K_MAX_SPEED = 4.0f * 60.0f;
	const float K_MAX_STEER_FORCE = 20.0f * 60.0f;

	// State fields
	Behavior currentBehavior = SEEK_KINEMATIC;
	bool isSteering = false;
	bool smoothAngleChange = false;

	// Steering fields
	Vector2D* targetPosition = {};
	Vector2D desiredVelocity = {};
	Vector2D steeringForce = {};
	Vector2D acceleration = {};
	const float mass = 1.0f;

	// Arrive fields
	const int slowingRadius = 150;

	// Pursue fields
	Vector2D* targetSpeed;
	Vector2D futurePosition = {};
	const float maximumLookAhead = 0.5f;
	float timeLookAhead = 0.0f;

	// Wander fields
	const int wanderMaxChange = 50;
	const int wanderCircleOffset = 80;
	const int wanderCircleRadius = 70;
	float wanderAngle = 0.0f;
	Vector2D wanderCircleCenter = {};
	Vector2D wanderDisplacementVector = {};

	// Simple Path Following
	SimplePath* simplePath = nullptr;
	int currentSegment = 0;
	int pathDirection = 1;
	const float K_SIMPLE_PATH_ARRIVAL_DISTANCE = 20.0f;

	// Path Following
	Path* path = nullptr;
	float pathOffset = 0.08f;
	Vector2D currentPathTarget;
	bool loopPath = true;
	bool pathFollowingStarted = false;
	const float K_PATH_ARRIVAL_DISTANCE = 10.0f;

	// Flocking
	Boid* boidArray;
	int* boidArraySize;
	Vector2D alignment;		// Debug purposes
	Vector2D cohesion;		// Debug purposes
	Vector2D separation;	// Debug purposes
	Vector2D flockingForce; // Debug purposes
	const float K_MAX_FLOCK_FORCE = 30.0f * 60.0f;
	const float K_FLOCK_NEIGHBOURHOOD = 60.0f;

	// Perimeter avoidance
	const float K_PERIMETER_BORDER = 20.0f;

	// Collision Avoidance
	static const int MAX_NUMBER_TARGETS = 80;
	const float K_MAX_STEER_AVOID_FORCE = 18.0f * 60.0f;
	DibObject* targets[MAX_NUMBER_TARGETS];
	int numTargets = 0;
	const float coneHeight = 80.0f;
	const float coneHalfAngle = 30.0f;
	void AddTargetForCollisionAvoidance(DibObject* target);
	void CleanTargetsForCollisionAvoidance();
	bool collisionDetected = false; // Debug purposes

	// Obstacle Avoidance
	Obstacle* obstacleArray;
	int* obstacleArraySize;
	const float avoidanceLookahead = 100.0f;
	const float avoidanceDistance = 30.0f;
	const float K_MAX_STEER_OBSTACLE_AVOID_FORCE = 30.0f * 60.0f;
	Vector2D avoidTarget = {}; // Debug purposes
	bool obstacleAvoidanceCollision = false; // Debug purposes

	// Flow Field Steering
	const float K_MAX_FLOW_FIELD_FORCE = 20.0f * 60.0f;
	FlowField* field;

	// Priority Steering
	SteeringGroup groupArray[4];
	SteeringGroup group1;
	SteeringGroup group2;
	SteeringGroup group3;
	SteeringGroup group4;
	float K_EPSILON_THRESHOLD = 11.0f * 60.0f;
	int usedGroup = 0;

	// Steering functions
	void SetBehavior(Behavior newBehavior);
	void DoKinematicSeek();
	void DoKinematicFlee();
	void DoSteeringSeek(float deltaTime);
	void DoSteeringFlee(float deltaTime);
	void DoArrive(float deltaTime);
	void DoPursue(float deltaTime);
	void DoEvade(float deltaTime);
	void DoSimpleWander(float deltaTime);
	void DoFullWander(float deltaTime);
	void DoSimplePathFollowing(float deltaTime);
	void DoPathFollowing(float deltaTime);
	void DoCollisionAvoidance(float deltaTime);
	void DoObstacleAvoidance(float deltaTime);
	void DoFlocking(float deltaTime);
	void DoPerimeterAvoidance(float deltaTime);
	void DoFlowFieldObstacleAvoidance(float deltaTime);
	void DoPriorityFlowFlockingSeek(float deltaTime);
	void InitPrioritySteering();
	void DoPriorityObstacleSteering(float deltaTime);
	void DoPriorityCollisionSteering(float deltaTime);
	void DoPriorityCollisionPathFollowing(float deltaTime);
	void DeinitPrioritySteering();

	// Movement function
	void UpdateAngle();

	// Debug functions
	void InitDebug(FC_Font* font);
	void InitFlockingDebug(SDL_Renderer* renderer, FC_Font* font);
	void DrawDebug();
	void DrawFlockingDebug(SDL_Renderer* renderer);
	void CleanDebug();

	// Debug Information
	RotatedText informationText1;
	RotatedText informationText2;
	RotatedText informationText3;
	RotatedText informationText4;
	char textBuffer[100];
};

