/* ========================================================================
   File: BlackboardAgent.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "BlackboardAgent.h"
#include "StaticBlackboard.h"
#include "DynamicBlackboard.h"

void BlackboardAgent::Clean()
{
	FreeBlackboards();
}

void BlackboardAgent::InitBlackboards()
{
	staticBlackboard = new StaticBlackboard();
	dynamicBlackboard = new DynamicBlackboard();
}

void BlackboardAgent::FreeBlackboards()
{
	delete staticBlackboard;
	delete dynamicBlackboard;
}