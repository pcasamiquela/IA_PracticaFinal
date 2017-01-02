/* ========================================================================
File: Boid.cpp
Revision: 0.1
Creator: David Collado Ochoa
Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
======================================================================== */

#include "Boid.h"
#include "SteeringUtils.h"
#include "AdvancedSteeringUtils.h"
#include "FlockingUtils.h"
#include "FlowFieldUtils.h"

float K_FLOCK_COHESION = 0.4f;
float K_FLOCK_ALIGNMENT = 0.6f;
float K_FLOCK_SEPARATION = 1.7f;

void Boid::SetBehavior(Behavior newBehavior)
{
	currentBehavior = newBehavior;
	if (currentBehavior == Behavior::SEEK_KINEMATIC 
		|| currentBehavior == Behavior::FLEE_KINEMATIC
		|| currentBehavior == Behavior::SIMPLE_WANDER)
	{
		isSteering = false;
	}
	else
	{
		isSteering = true;
	}
}

void Boid::InitPrioritySteering()
{
	SteeringGroup_Initialize(&group1, 1);
	group1.weightArray[0] = 1.0f;
	group1.calculatedForces[0] = {};

	SteeringGroup_Initialize(&group2, 1);
	group2.weightArray[0] = 1.0f;
	group2.calculatedForces[0] = {};

	SteeringGroup_Initialize(&group3, 1);
	group3.weightArray[0] = 1.0f;
	group3.calculatedForces[0] = {};

	SteeringGroup_Initialize(&group4, 1);
	group4.weightArray[0] = 1.0f;
	group4.calculatedForces[0] = {};

	groupArray[0] = group1;
	groupArray[1] = group2;
	groupArray[2] = group3;
	groupArray[3] = group4;
}

void Boid::DeinitPrioritySteering()
{
	SteeringGroup_Free(&group1);
	SteeringGroup_Free(&group2);
	SteeringGroup_Free(&group3);
	SteeringGroup_Free(&group4);
}

void Boid::Update(float deltaTime)
{
	if (!GetActive())
		return;

	switch (currentBehavior) {
	case Behavior::NONE:
		return;
	case Behavior::SEEK_KINEMATIC:
		DoKinematicSeek();
		break;
	case Behavior::SEEK_STEERING:
		DoSteeringSeek(deltaTime);
		break;
	case Behavior::FLEE_KINEMATIC:
		DoKinematicFlee();
		break;
	case Behavior::FLEE_STEERING:
		DoSteeringFlee(deltaTime);
		break;
	case Behavior::ARRIVE:
		DoArrive(deltaTime);
		break;
	case Behavior::PURSUE:
		DoPursue(deltaTime);
		break;
	case Behavior::EVADE:
		DoEvade(deltaTime);
		break;
	case Behavior::SIMPLE_WANDER:
		DoSimpleWander(deltaTime);
		break;
	case Behavior::WANDER:
		DoFullWander(deltaTime);
		break;
	case Behavior::SIMPLE_PATH_FOLLOWING:
		DoSimplePathFollowing(deltaTime);
		break;
	case Behavior::PATH_FOLLOWING:
		DoPathFollowing(deltaTime);
		break;
	case Behavior::OBSTACLE_AVOIDANCE:
		// Mix both to spice it up!
		DoObstacleAvoidance(deltaTime);
		if (obstacleAvoidanceCollision == false)
		{
			DoSteeringSeek(deltaTime);
		}
		break;
	case Behavior::COLLISION_AVOIDANCE:
		// Mix both to spice it up!
		DoCollisionAvoidance(deltaTime);
		DoSteeringSeek(deltaTime);
		//DoSimplePathFollowing(deltaTime);
		break;
	case Behavior::FLOCKING:
		DoFlocking(deltaTime);
		DoPerimeterAvoidance(deltaTime);
		break;
	case Behavior::FLOCKING_SEEK:
		DoSteeringSeek(deltaTime);
		DoFlocking(deltaTime);
		DoPerimeterAvoidance(deltaTime);
		break;
	case Behavior::PRIORITY_OBSTACLE:
		DoPriorityObstacleSteering(deltaTime);
		break;
	case Behavior::PRIORITY_COLLISION:
		DoPriorityCollisionSteering(deltaTime);
		break;
	case Behavior::FLOW_FLOCK_SEEK:
		DoSteeringSeek(deltaTime);
		DoFlocking(deltaTime);
		DoPerimeterAvoidance(deltaTime);
		DoFlowFieldObstacleAvoidance(deltaTime);
		break;
	case Behavior::FLOW_FLOCK_SEEK_PRIORITY:
		DoPriorityFlowFlockingSeek(deltaTime);
		break;
	case Behavior::PRIORITY_COLLISION_PATH_FOLLOWING:
		DoPriorityCollisionPathFollowing(deltaTime);
		break;
	case Behavior::FLOCKING_ARRIVE:
		DoArrive(deltaTime);
		DoFlocking(deltaTime);
		break;
	default:
		break;
	}

	// Call base Update
	DibSprite::Update(deltaTime);

	// Update Angle
	UpdateAngle();

	// Wrap player
	Utils::DoWrap(position);
}

void Boid::UpdateAngle()
{
	if (speed.Length() > 0.0f)
	{
		if (smoothAngleChange == true)
		{
			float angleToUpdate = FloatUtils::CalculateOrientation(speed);
			float angleDelta = angleToUpdate - angle;
			if (angleDelta > 180.0f)
			{
				SDL_Log("Should add 360");
				angle += 360.0f;
			}
			else if (angleDelta < -180.0f)
			{
				SDL_Log("Should sub -360");
				angle -= 360.0f;
			}
			angle = FloatUtils::Lerp(angle, angleToUpdate, 0.1f);
		}
		else
		{
			angle = FloatUtils::CalculateOrientation(speed);
		}
	}
}

void Boid::Render()
{
	DibSprite::Render();
}

void Boid::Clean()
{
	DibSprite::Clean();
}

void Boid::Setup()
{
	SDL_Log("Boid - Setup");
	SetPosition(0.0f, 0.0f);
	SetWidth(32);
	SetHeight(24);
	std::string assetPath = PathUtils::GetResourcesPath("images/Boid.png");
	LoadGraphic(assetPath, "Boid", 32, 24, false );
	SetScale(GLOBAL_SCALE, GLOBAL_SCALE);
	SetOrigin(0.5f, 0.5f);
	SetActive(false);
}

void Boid::DoKinematicSeek()
{
	desiredVelocity = SteeringUtils::DoKinematicSeek(*targetPosition, position, K_MAX_SPEED);
	speed = desiredVelocity;
}

void Boid::DoKinematicFlee()
{
	desiredVelocity = SteeringUtils::DoKinematicFlee(*targetPosition, position, K_MAX_SPEED);
	speed = desiredVelocity;
}

void Boid::DoSteeringSeek(float deltaTime)
{
#if DEBUG
	// Calculate desired velocity
	desiredVelocity = *targetPosition - position;

	// Normalize and Scale by K_MAX_SPEED
	desiredVelocity.Normalize();
	desiredVelocity *= K_MAX_SPEED;

	// Calculate Steering Force
	steeringForce = desiredVelocity - speed;

	// Convert to Force
	// Divide by K_MAX_SPEED to get the speed factor
	steeringForce /= K_MAX_SPEED;
	// Scale this factor by K_MAX_STEER_FORCE
	steeringForce *= K_MAX_STEER_FORCE;
#else
	steeringForce = SteeringUtils::DoSteeringSeek(*targetPosition, position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE);
#endif

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoSteeringFlee(float deltaTime)
{
#if DEBUG
	// Calculate desired velocity
	desiredVelocity = position - *targetPosition;

	// Normalize and Scale by K_MAX_SPEED
	desiredVelocity.Normalize();
	desiredVelocity *= K_MAX_SPEED;

	// Calculate Steering Force
	steeringForce = desiredVelocity - speed;

	// Convert to Force
	// Divide by K_MAX_SPEED to get the speed factor
	steeringForce /= K_MAX_SPEED;
	// Scale this factor by K_MAX_STEER_FORCE
	steeringForce *= K_MAX_STEER_FORCE;
#else
	steeringForce = SteeringUtils::DoSteeringFlee(*targetPosition, position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE);
#endif

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoArrive(float deltaTime)
{
#if DEBUG
	// Calculate desired velocity
	desiredVelocity = *targetPosition - position;
	// Are we inside slowing radius?
	float distanceToTarget = desiredVelocity.Length();

	float targetSpeed;
	if (distanceToTarget > slowingRadius)
	{
		// Scale by K_MAX_SPEED
		targetSpeed = K_MAX_SPEED;
	}
	else
	{
		// Slow down based on distance to target
		targetSpeed = K_MAX_SPEED * (distanceToTarget / slowingRadius);
	}

	// Normalize and scale by targetSpeed
	desiredVelocity.Normalize();
	desiredVelocity *= targetSpeed;

	// Update Steering Velocity
	steeringForce = desiredVelocity - speed;

	// Convert to Force
	// Divide by K_MAX_SPEED to get the speed factor
	steeringForce /= K_MAX_SPEED;
	// Scale this factor by K_MAX_STEER_FORCE
	steeringForce *= K_MAX_STEER_FORCE;
#else
	steeringForce = SteeringUtils::DoSteeringArrive(*targetPosition, position, speed,
		K_MAX_SPEED, K_MAX_STEER_FORCE, slowingRadius);
#endif

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoPursue(float deltaTime)
{
#if DEBUG
	// Calculate desired velocity
	Vector2D distanceVector = *targetPosition - position;
	float distanceToTarget = distanceVector.Length();

	// Calculate lookAhead time
	float currentSpeed = speed.Length() * deltaTime;

	// If speed is to small or distance to big to give a reasonable 
	// look ahead time to target, use maximumLookAhead
	timeLookAhead = distanceToTarget / currentSpeed;
	if (timeLookAhead > maximumLookAhead)
	{
		timeLookAhead = maximumLookAhead;
	}
	// Calculate future position based on current position and speed
	futurePosition = *targetPosition + (*targetSpeed * timeLookAhead);

	// Calculate desired velocity
	desiredVelocity = futurePosition - position;

	// Normalize
	desiredVelocity.Normalize();
	// Scale by K_MAX_SPEED
	desiredVelocity *= K_MAX_SPEED;

	// Update Steering Velocity
	steeringForce = desiredVelocity - speed;

	// Convert to Force
	// Divide by K_MAX_SPEED to get the speed factor
	steeringForce /= K_MAX_SPEED;
	// Scale this factor by K_MAX_STEER_FORCE
	steeringForce *= K_MAX_STEER_FORCE;
#else
	steeringForce = SteeringUtils::DoSteeringPursue(*targetPosition, *targetSpeed, position, speed,
		K_MAX_SPEED, K_MAX_STEER_FORCE, maximumLookAhead);
#endif

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoEvade(float deltaTime)
{
#if DEBUG
	// Calculate desired velocity
	Vector2D distanceVector = *targetPosition - position;
	float distanceToTarget = distanceVector.Length();

	// Calculate lookAhead time
	float currentSpeed = speed.Length();

	// If speed is to small or distance to big to give a reasonable 
	// look ahead time to target, use maximumLookAhead
	timeLookAhead = distanceToTarget / currentSpeed;
	if (timeLookAhead > maximumLookAhead)
	{
		timeLookAhead = maximumLookAhead;
	}

	// Calculate future position based on current position and speed
	futurePosition = *targetPosition + (*targetSpeed * timeLookAhead);

	// Calculate desired velocity
	desiredVelocity = position - futurePosition;

	// Normalize
	desiredVelocity.Normalize();
	// Scale by K_MAX_SPEED
	desiredVelocity *= K_MAX_SPEED;

	// Update Steering Velocity
	steeringForce = desiredVelocity - speed;

	// Convert to Force
	// Divide by K_MAX_SPEED to get the speed factor
	steeringForce /= K_MAX_SPEED;
	// Scale this factor by K_MAX_STEER_FORCE
	steeringForce *= K_MAX_STEER_FORCE;
#else
	steeringForce = SteeringUtils::DoSteeringEvade(*targetPosition, *targetSpeed, position, speed,
		K_MAX_SPEED, K_MAX_STEER_FORCE, maximumLookAhead);
#endif

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoSimpleWander(float deltaTime)
{
	float simpleWanderAngle = FloatUtils::RandomBinomial() * wanderMaxChange;
	angle = FloatUtils::Lerp(angle, angle + simpleWanderAngle, 0.3f);
	//angle += simpleWanderAngle;
	speed.x = cosf(angle * DEG2RAD) * K_MAX_SPEED;
	speed.y = sinf(angle * DEG2RAD) * K_MAX_SPEED;
}

void Boid::DoFullWander(float deltaTime)
{
#if DEBUG
	// Update wanderAngle
	wanderAngle += FloatUtils::RandomBinomial() * wanderMaxChange;
	float targetAngle = angle + wanderAngle;

	// Calculate the center of the wander circle
	Vector2D speedOrientationVector = Vector2D::Normalize(speed);
	wanderCircleCenter = position + speedOrientationVector * wanderCircleOffset;

	// Calculate and add displacement location (Using the Parametric equation of the circle)
	wanderDisplacementVector.x = wanderCircleRadius * cosf(targetAngle * DEG2RAD);
	wanderDisplacementVector.y = wanderCircleRadius * sinf(targetAngle * DEG2RAD);
	wanderDisplacementVector += wanderCircleCenter;

	// Seek Behavior with target wanderDisplacementVector
	// Calculate desired velocity
	desiredVelocity = wanderDisplacementVector - position;

	// Normalize and Scale by K_MAX_SPEED
	desiredVelocity.Normalize();
	desiredVelocity *= K_MAX_SPEED;

	// Calculate Steering Force
	steeringForce = desiredVelocity - speed;

	// Convert to Force
	// Divide by K_MAX_SPEED to get the speed factor
	steeringForce /= K_MAX_SPEED;
	// Scale this factor by K_MAX_STEER_FORCE
	steeringForce *= K_MAX_STEER_FORCE;
#else
	steeringForce = SteeringUtils::DoSteeringWander(position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE,
		angle, &wanderAngle, wanderMaxChange, wanderCircleOffset, wanderCircleRadius);
#endif

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoSimplePathFollowing(float deltaTime)
{
#if DEBUG
	// Find Current segment to target
	Vector2D targetSegment = simplePath->pathArray[currentSegment];

	// Are we near enough targetSegment
	if (Vector2D::Distance(position, targetSegment) < K_SIMPLE_PATH_ARRIVAL_DISTANCE)
	{
		// Update targetSegment next time
		currentSegment += pathDirection;
		if (currentSegment >= simplePath->pathOccupation || currentSegment < 0)
		{
			// Loop
			pathDirection *= -1;
			currentSegment += pathDirection;
		}
	}

	// Seek target segment
	steeringForce = SteeringUtils::DoKinematicSeek(targetSegment, position, K_MAX_SPEED);
#else
	steeringForce = AdvancedSteeringUtils::DoSimplePathFollowing(position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE,
		simplePath, currentSegment, pathDirection, K_SIMPLE_PATH_ARRIVAL_DISTANCE);
#endif
	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoPathFollowing(float deltaTime)
{
#if DEBUG
	if (pathFollowingStarted == false)
	{
		float targetParameter = Path_GetParameter(path, position, currentPathTarget);
		currentPathTarget = Path_GetPosition(path, targetParameter);
		pathFollowingStarted = true;
	}
	else if (Vector2D::Distance(position, currentPathTarget) < K_PATH_ARRIVAL_DISTANCE)
	{
		float targetParameter = Path_GetParameter(path, position, currentPathTarget);
		targetParameter += pathOffset;

		if (targetParameter > 1.0f)
		{
			if (loopPath == true)
			{
				targetParameter = 1.0f;
				pathOffset *= -1.0f;
			}
			else
			{
				targetParameter = 1.0f;
			}
		}
		else if (targetParameter < 0.0f)
		{
			if (loopPath == true)
			{
				targetParameter = 0.0f;
				pathOffset *= -1.0f;
			}
			else
			{
				targetParameter = 0.0f;
			}
		}
		// Update Current Path Target
		currentPathTarget = Path_GetPosition(path, targetParameter);
	}

	steeringForce = SteeringUtils::DoSteeringSeek(currentPathTarget, position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE);
#else
	steeringForce = AdvancedSteeringUtils::DoPathFollowing(position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE, path, pathFollowingStarted,
		currentPathTarget, pathOffset, K_PATH_ARRIVAL_DISTANCE, loopPath);
#endif
	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoCollisionAvoidance(float deltaTime)
{
#if DEBUG
	float shortestDistance = 999999999.9f;
	DibObject* firstTarget = nullptr;

	Vector2D coneBasePoint = { position.x + coneHeight, position.y };
	Vector2D currentConeBase = Vector2DUtils::RotatePoint(position, coneBasePoint, angle);

	collisionDetected = false;

	for (int i = 0; i < numTargets; ++i)
	{
		DibObject* currentTarget = targets[i];

		// Is target inside the cone?
		if (Vector2DUtils::IsInsideCone(currentTarget->position, position, currentConeBase, coneHalfAngle))
		{
			float currentDistance = Vector2D::Distance(position, currentTarget->position);
			// Is the nearest targest inside the cone?
			if (currentDistance < shortestDistance)
			{
				firstTarget = currentTarget;
				shortestDistance = currentDistance;
				collisionDetected = true;
			}
		}
	}

	if (firstTarget == nullptr)
	{
		return;
	}

	// Flee from collision
	steeringForce = SteeringUtils::DoSteeringFlee(firstTarget->position, position, speed, K_MAX_SPEED, K_MAX_STEER_AVOID_FORCE);
#else
	steeringForce = AdvancedSteeringUtils::DoCollisionAvoidance(position, speed, angle, K_MAX_SPEED, K_MAX_STEER_AVOID_FORCE, (DibObject**)targets,
		numTargets, coneHeight, coneHalfAngle, collisionDetected);
#endif
	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;

}

void Boid::AddTargetForCollisionAvoidance(DibObject* target)
{
	if (numTargets < MAX_NUMBER_TARGETS)
	{
		targets[numTargets] = target;
		++numTargets;
	}
	else
	{
		SDL_Log("Target Array reached max number of targets.");
	}
}


void Boid::CleanTargetsForCollisionAvoidance()
{
	numTargets = 0;
}

void Boid::DoObstacleAvoidance(float deltaTime)
{
#if DEBUG
	Vector2D raycastVector = position;
	raycastVector += Vector2D::Normalize(speed) * avoidanceLookahead;

	Vector2D intersectionPoint, normalVector;

	int wallSize = *obstacleArraySize;
	obstacleAvoidanceCollision = false;
	for (int i = 0; i < wallSize; ++i)
	{
		obstacleAvoidanceCollision = Obstacle_IntersectsSegment(&obstacleArray[i], position, 
			raycastVector, &intersectionPoint, &normalVector);
		if (obstacleAvoidanceCollision == true)
		{
			break;
		}
	}

	if (obstacleAvoidanceCollision == true)
	{
		// Seek where target = normal * avoidanceDistance
		avoidTarget = intersectionPoint;
		avoidTarget += normalVector * avoidanceDistance;

		steeringForce = SteeringUtils::DoSteeringSeek(avoidTarget, position, speed, K_MAX_SPEED, K_MAX_STEER_OBSTACLE_AVOID_FORCE);
	}
	else
	{
		steeringForce = {};
	}
#else
	steeringForce = AdvancedSteeringUtils::DoObstacleAvoidance(position, speed, K_MAX_SPEED,
		K_MAX_STEER_OBSTACLE_AVOID_FORCE, avoidanceLookahead, avoidanceDistance, *obstacleArraySize,
		obstacleArray);
#endif

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoFlocking(float deltaTime)
{
#if DEBUG
	alignment = {};
	cohesion = {};
	separation = {};

	int neighbourCount = 0;
	int boidSize = *boidArraySize;

	for (int i = 0; i < boidSize; ++i)
	{
		if (&boidArray[i] == this)
		{
			// Do not count myself
			//SDL_Log("I found me!");
		}
		else
		{
			// Is neighbour?
			if (Vector2D::Distance(position, boidArray[i].position) < K_FLOCK_NEIGHBOURHOOD)
			{
				alignment += boidArray[i].speed;
				cohesion += boidArray[i].position;
				separation += position - boidArray[i].position;
				++neighbourCount;
			}
		}
	}

	if (neighbourCount != 0)
	{
		// Alignment Calculation
		alignment /= neighbourCount;
		// Normalize and scale by K_FLOCK_ALIGNMENT
		alignment.Normalize();
		alignment *= K_FLOCK_ALIGNMENT;

		// Cohesion Calculation
		cohesion /= neighbourCount;
		cohesion -= position;
		// Normalize and scale by K_FLOCK_COHESION
		cohesion.Normalize();
		cohesion *= K_FLOCK_COHESION;

		// Separation Calculation
		separation /= neighbourCount;
		// Normalize and scale by K_FLOCK_SEPARATION
		separation.Normalize();
		separation *= K_FLOCK_SEPARATION;
	}

	// Add all forces together
	flockingForce = alignment + cohesion + separation;

	// Convert to Force
	flockingForce.Normalize();
	flockingForce *= K_MAX_FLOCK_FORCE;
#else
	flockingForce = FlockingUtils::DoFlocking(boidArraySize, boidArray, this, position, K_MAX_FLOCK_FORCE,
		K_FLOCK_NEIGHBOURHOOD, K_FLOCK_ALIGNMENT, K_FLOCK_COHESION, K_FLOCK_SEPARATION);
#endif

	SDL_Log("Flocking Force = %f, %f", flockingForce.x, flockingForce.y);

	// Modify according to mass to get acceleration
	acceleration = flockingForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;

	// Force Limit Speed
	if (speed.Length() > K_MAX_SPEED)
	{
		speed = speed.Normalize() * K_MAX_SPEED;
	}
}

void Boid::DoPerimeterAvoidance(float deltaTime)
{
#if DEBUG
	desiredVelocity = {};

	if (position.x < K_PERIMETER_BORDER)
	{
		desiredVelocity.x = K_MAX_SPEED;
	}
	else if (position.x > FIXED_WIDTH - K_PERIMETER_BORDER)
	{
		desiredVelocity.x = -K_MAX_SPEED;
	}

	if (position.y < K_PERIMETER_BORDER)
	{
		desiredVelocity.y = K_MAX_SPEED;
	}
	else if (position.y > FIXED_HEIGHT - K_PERIMETER_BORDER)
	{
		desiredVelocity.y = -K_MAX_SPEED;
	}

	if (desiredVelocity.Length() > 0.0f)
	{
		// Calculate Steering Force
		steeringForce = desiredVelocity - speed;

		// Convert to Force
		// Divide by K_MAX_SPEED to get the speed factor
		steeringForce /= K_MAX_SPEED;
		// Scale this factor by K_MAX_STEER_FORCE
		steeringForce *= K_MAX_STEER_FORCE;
	}
	else
	{
		steeringForce = {};
	}
#else
	steeringForce = AdvancedSteeringUtils::DoPerimeterAvoidance(position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE,
		FIXED_WIDTH, FIXED_HEIGHT, K_PERIMETER_BORDER);
#endif
	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoFlowFieldObstacleAvoidance(float deltaTime)
{
#if DEBUG
 	Vector2D desiredVelocity = FlowField_GetForceForPosition(field, position);
	desiredVelocity *= K_MAX_SPEED;

	if (desiredVelocity.Length() == 0.0f)
		return;

	// Calculate Steering Force
	Vector2D steeringForce = desiredVelocity - speed;

	// Convert to Force
	// Divide by K_MAX_SPEED to get the speed factor
	steeringForce /= K_MAX_SPEED;
	// Scale this factor by K_MAX_STEER_FLOW_FORCE
	steeringForce *= K_MAX_STEER_FORCE;
#else
	// Calculate steering
	steeringForce = FlowFieldUtils::DoFlowFieldFollowing(position, speed, K_MAX_SPEED,
		K_MAX_FLOW_FIELD_FORCE, field);
#endif
	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoPriorityFlowFlockingSeek(float deltaTime)
{
	// Calculate steering
	group1.calculatedForces[0] = FlowFieldUtils::DoFlowFieldFollowing(position, speed, K_MAX_SPEED,
		K_MAX_FLOW_FIELD_FORCE, field);

	// Calculate steering
	group2.calculatedForces[0] = AdvancedSteeringUtils::DoPerimeterAvoidance(position, speed,
		K_MAX_SPEED, K_MAX_STEER_FORCE,
		FIXED_WIDTH, FIXED_HEIGHT, K_PERIMETER_BORDER);

	group3.calculatedForces[0] = FlockingUtils::DoFlocking(boidArraySize, boidArray, 
		this, position, K_MAX_FLOCK_FORCE, K_FLOCK_NEIGHBOURHOOD, 
		K_FLOCK_ALIGNMENT, K_FLOCK_COHESION, K_FLOCK_SEPARATION);

	group4.calculatedForces[0] = SteeringUtils::DoSteeringSeek(*targetPosition, position,
		speed, K_MAX_SPEED, K_MAX_STEER_FORCE);

	steeringForce = CombiningSteering::DoPriorityBlending(4, (SteeringGroup*)groupArray, 
		K_EPSILON_THRESHOLD, usedGroup);

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;

	// Force Limit Speed
	if (speed.Length() > K_MAX_SPEED)
	{
		speed = speed.Normalize() * K_MAX_SPEED;
	}
}

void Boid::DoPriorityObstacleSteering(float deltaTime)
{
	// Calculate steering
	group1.calculatedForces[0] = AdvancedSteeringUtils::DoObstacleAvoidance(position, speed, K_MAX_SPEED,
		K_MAX_STEER_OBSTACLE_AVOID_FORCE, avoidanceLookahead, avoidanceDistance, *obstacleArraySize,
		obstacleArray);

	// Calculate steering
	group2.calculatedForces[0] = SteeringUtils::DoSteeringWander(position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE,
		angle, &wanderAngle, wanderMaxChange, wanderCircleOffset, wanderCircleRadius);
	steeringForce = CombiningSteering::DoPriorityBlending(2, (SteeringGroup*)groupArray, K_EPSILON_THRESHOLD, usedGroup);

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;

}

void Boid::DoPriorityCollisionSteering(float deltaTime)
{
	// Calculate steering
	group1.calculatedForces[0] = AdvancedSteeringUtils::DoCollisionAvoidance(position, speed, angle, K_MAX_SPEED,
		K_MAX_STEER_FORCE, (DibObject**)targets, numTargets, coneHeight, coneHalfAngle, collisionDetected);

	// Calculate steering
	group2.calculatedForces[0] = SteeringUtils::DoSteeringWander(position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE,
		angle, &wanderAngle, wanderMaxChange, wanderCircleOffset, wanderCircleRadius);
	steeringForce = CombiningSteering::DoPriorityBlending(2, (SteeringGroup*)groupArray, K_EPSILON_THRESHOLD, usedGroup);

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoPriorityCollisionPathFollowing(float deltaTime)
{
	// Calculate steering
	group1.calculatedForces[0] = AdvancedSteeringUtils::DoCollisionAvoidance(position, speed, angle, K_MAX_SPEED,
		K_MAX_STEER_FORCE, (DibObject**)targets, numTargets, coneHeight, coneHalfAngle, collisionDetected);

	// Calculate steering
	group2.calculatedForces[0] = AdvancedSteeringUtils::DoPathFollowing(position, speed, 
		K_MAX_SPEED, K_MAX_STEER_FORCE, path, pathFollowingStarted,
		currentPathTarget, pathOffset, K_PATH_ARRIVAL_DISTANCE, loopPath);

	steeringForce = CombiningSteering::DoPriorityBlending(2, (SteeringGroup*)groupArray, K_EPSILON_THRESHOLD, usedGroup);

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

// Debug Functions from here till end of file
void Boid::InitDebug(FC_Font* font)
{
	informationText1.font = font;
	informationText1.renderer = Game::Instance().renderer;
	RotatedText_SetText(&informationText1, (char*)"Current Velocity");

	informationText2.font = font;
	informationText2.renderer = Game::Instance().renderer;
	RotatedText_SetText(&informationText2, (char*)"Desired Velocity");

	informationText3.font = font;
	informationText3.renderer = Game::Instance().renderer;
	RotatedText_SetText(&informationText3, (char*)"Steering Force");

	informationText4.font = font;
	informationText4.renderer = Game::Instance().renderer;
	RotatedText_SetText(&informationText4, (char*)"Speed = 0.00");
}

void Boid::InitFlockingDebug(SDL_Renderer* renderer, FC_Font* font)
{
	informationText1.font = font;
	informationText1.renderer = renderer;
	RotatedText_SetText(&informationText1, (char*)"Cohesion");

	informationText2.font = font;
	informationText2.renderer = renderer;
	RotatedText_SetText(&informationText2, (char*)"Alignment");

	informationText3.font = font;
	informationText3.renderer = renderer;
	RotatedText_SetText(&informationText3, (char*)"Separation");

	informationText4.font = font;
	informationText4.renderer = renderer;
	RotatedText_SetText(&informationText4, (char*)"Flocking");
}

void Boid::DrawDebug()
{
	if (currentBehavior == Behavior::OBSTACLE_AVOIDANCE)
	{
		if (obstacleAvoidanceCollision == true)
		{
			filledCircleColor(Game::Instance().renderer, avoidTarget.x, avoidTarget.y, 10, Colors::TURQUOISE);
		}
	}
	else if (currentBehavior == Behavior::COLLISION_AVOIDANCE
		|| currentBehavior == Behavior::PRIORITY_COLLISION
		|| currentBehavior == Behavior::PRIORITY_COLLISION_PATH_FOLLOWING)
	{
		static int timerColor = 0;
		int maxTime = 10;
		Uint32 collisionColor = Colors::BELIZE_HOLE;
		if (collisionDetected == true || timerColor > 0)
		{
			collisionColor = Colors::ALIZARIN;
			if (collisionDetected == true)
			{
				timerColor = maxTime;
			}
		}
		--timerColor;
		Vector2D triangleX1 = { position.x + coneHeight, position.y - tanf(coneHalfAngle * DEG2RAD) * coneHeight };
		Vector2D triangleX2 = { position.x + coneHeight, position.y + tanf(coneHalfAngle * DEG2RAD) * coneHeight };
		Vector2D rotatedTriangle1 = Vector2DUtils::RotatePoint(position, triangleX1, angle);
		Vector2D rotatedTriangle2 = Vector2DUtils::RotatePoint(position, triangleX2, angle);
		filledTrigonColor(Game::Instance().renderer, position.x, position.y,
			rotatedTriangle1.x, rotatedTriangle1.y,
			rotatedTriangle2.x, rotatedTriangle2.y,
			collisionColor);
		return;
	}
	else if (currentBehavior == Behavior::ARRIVE)
	{
		circleRGBA(Game::Instance().renderer, 
			targetPosition->x, targetPosition->y, slowingRadius, 255, 255, 0, 255);
		sprintf(textBuffer, "Speed = %.2f", desiredVelocity.Length());
		RotatedText_SetText(&informationText4, textBuffer);
		RotatedText_DrawText(&informationText4, position.x + 10.0f,
			position.y + 10.0f, 0.0f,
			0, 0, 255, 255);
	}
	else if (currentBehavior == Behavior::WANDER)
	{
		circleRGBA(Game::Instance().renderer, 
			wanderCircleCenter.x, wanderCircleCenter.y, wanderCircleRadius, 255, 255, 0, 255);
		filledCircleRGBA(Game::Instance().renderer, 
			wanderDisplacementVector.x, wanderDisplacementVector.y, 10, 128, 0, 128, 255);

		SDL_SetRenderDrawColor(Game::Instance().renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(Game::Instance().renderer, position.x, position.y, 
			wanderDisplacementVector.x, wanderDisplacementVector.y);
	}
	else if (currentBehavior != Behavior::SIMPLE_PATH_FOLLOWING
		&& currentBehavior != Behavior::PATH_FOLLOWING
		&&  currentBehavior != Behavior::PRIORITY_OBSTACLE)
	{
		int currentTextAngle;

		SDL_SetRenderDrawColor(Game::Instance().renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(Game::Instance().renderer, 
			position.x, position.y, position.x + speed.x, position.y + speed.y);

		if (isSteering == true)
		{
			SDL_SetRenderDrawColor(Game::Instance().renderer, 255, 255, 0, 255);
			SDL_RenderDrawLine(Game::Instance().renderer, 
				position.x, position.y, position.x + desiredVelocity.x,
				position.y + desiredVelocity.y);

			SDL_SetRenderDrawColor(Game::Instance().renderer, 255, 0, 255, 255);
			SDL_RenderDrawLine(Game::Instance().renderer, 
				position.x + speed.x, 
				position.y + speed.y, 
				position.x + desiredVelocity.x,
				position.y + desiredVelocity.y);

			currentTextAngle = atan2f(desiredVelocity.y, desiredVelocity.x) * RAD2DEG;
			if (currentTextAngle < -90 || (currentTextAngle > 90 && currentTextAngle < 180))
			{
				currentTextAngle = currentTextAngle - 180;
			}
			RotatedText_DrawText(&informationText2,
				(2 * position.x + desiredVelocity.x) / 2,
				(2 * position.y + desiredVelocity.y) / 2,
				currentTextAngle,
				255, 255, 0, 255);

			currentTextAngle = atan2f((desiredVelocity.y - speed.y), (desiredVelocity.x - speed.x)) * RAD2DEG;
			if (currentTextAngle < -90 || (currentTextAngle > 90 && currentTextAngle < 180))
			{
				currentTextAngle = currentTextAngle - 180;
			}
			RotatedText_DrawText(&informationText3,
				(position.x + speed.x + position.x + desiredVelocity.x) / 2,
				(position.y + speed.y + position.y + desiredVelocity.y) / 2,
				currentTextAngle,
				255, 0, 255, 255);

			if (currentBehavior == Behavior::PURSUE || currentBehavior == Behavior::EVADE)
			{
				filledCircleRGBA(Game::Instance().renderer, 
					futurePosition.x, futurePosition.y, 20, 255, 255, 0, 255);
			}
		}

		currentTextAngle = angle;
		if (currentTextAngle < -90 || (currentTextAngle > 90 && currentTextAngle < 180))
		{
			currentTextAngle = currentTextAngle - 180;
		}
		RotatedText_DrawText(&informationText1,
			(2 * position.x + speed.x) / 2,
			(2 * position.y + speed.y) / 2,
			currentTextAngle,
			255, 0, 0, 255);
	}
}

void Boid::DrawFlockingDebug(SDL_Renderer* renderer)
{
	float drawFactor = 100.0f;

	circleColor(renderer, position.x, position.y, K_FLOCK_NEIGHBOURHOOD, Colors::PUMPKIN);

	thickLineColor(renderer, position.x, position.y,
		position.x + alignment.x * K_FLOCK_ALIGNMENT * drawFactor,
		position.y + alignment.y * K_FLOCK_ALIGNMENT * drawFactor, 3, Colors::YELLOW);

	thickLineColor(renderer, position.x, position.y,
		position.x + cohesion.x * K_FLOCK_COHESION * drawFactor,
		position.y + cohesion.y * K_FLOCK_COHESION * drawFactor, 3, Colors::RED);

	thickLineColor(renderer, position.x, position.y,
		position.x + separation.x * K_FLOCK_SEPARATION * drawFactor,
		position.y + separation.y * K_FLOCK_SEPARATION * drawFactor, 3, Colors::PURPLE);
}

void Boid::CleanDebug()
{
	RotatedText_Free(&informationText1);
	RotatedText_Free(&informationText2);
	RotatedText_Free(&informationText3);
	RotatedText_Free(&informationText4);
}

