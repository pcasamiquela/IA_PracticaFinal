/* ========================================================================
   File: StaticBlackboard.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "StaticBlackboard.h"

StaticBlackboard::StaticBlackboard()
{
	m_bAutoReload = false;
	m_fSeeDistance = 0.f;
	m_fHearDistance = 0.f;

	m_eAwareness = kAware_Relaxed;

	m_bTargetReachable = false;
	m_eTargetLocation = Vector2D();

	m_eTargetType = kTarget_None;
	m_bTargetVisibleFromEye = false;
	m_bTargetVisibleFromWeapon = false;
	m_fTargetLastVisibileTime = 0.f;

	m_nBodyCount = 0;
	m_nRoundsFired = 0;
	m_fLastDamage = 0.0f;
}

StaticBlackboard::~StaticBlackboard()
{
}

void StaticBlackboard::PrintData()
{
	SDL_Log("StaticBlackboard Contents");
	SDL_Log("m_bAutoReload = %d", m_bAutoReload);
	SDL_Log("m_fSeeDistance = %f", m_fSeeDistance);
	// Other fields
}