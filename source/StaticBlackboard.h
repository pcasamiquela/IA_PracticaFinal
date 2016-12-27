/* ========================================================================
   File: StaticBlackboard.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

enum EnumAIAwareness
{
	kAware_Invalid = -1,		
	kAware_Relaxed,				
	kAware_Suspicious,			 
	kAware_Alert,				
	kAware_Count,				
};

enum EnumAITargetType
{
	kTarget_None = 0,
	kTarget_Berserker = (1 << 0),
	kTarget_Character = (1 << 1),
	kTarget_CombatOpportunity = (1 << 2),
	kTarget_Disturbance = (1 << 3),
	kTarget_Interest = (1 << 4),
	kTarget_Leader = (1 << 5),
	kTarget_Object = (1 << 6),
	kTarget_WeaponItem = (1 << 7),
	kTarget_Follower = (1 << 8),
	kTarget_All = 0xffff,
};

// Hard-coded Blackboard
class StaticBlackboard
{
public:
	// Constructor / Destructor
	StaticBlackboard();
	~StaticBlackboard();

	void PrintData();

	bool	GetAutoReload() { return m_bAutoReload; }
	void	SetAutoReload(bool autoReload) { m_bAutoReload = autoReload; }

	float	GetSeeDistance() { return m_fSeeDistance; }
	void	SetSeeDistance(float seeDistance) { m_fSeeDistance = seeDistance; }

	float	GetHearDistance() { return m_fHearDistance; }
	void	SetHearDistance(float hearDistance) { m_fHearDistance = hearDistance; }

	EnumAIAwareness	GetAwareness() { return m_eAwareness; }
	void			SetAwareness(EnumAIAwareness awareness) { m_eAwareness = awareness; }

	bool	GetTargetReachable() { return m_bTargetReachable; }
	void	SetTargetReachable(bool targetReachable) { m_bTargetReachable = targetReachable; }

	Vector2D	GetTargetLocation() { return m_eTargetLocation; }
	void		SetTargetLocation(Vector2D targetLocation) { m_eTargetLocation = targetLocation; }

	EnumAITargetType	GetTargetType() { return m_eTargetType; }
	void				SetTargetType(EnumAITargetType targetType) { m_eTargetType = targetType; }

	bool	GetTargetVisibleFromEye() { return m_bTargetVisibleFromEye; }
	void	SetTargetVisibleFromEye(bool targetVisibleFromEye) { m_bTargetVisibleFromEye = targetVisibleFromEye; }

	bool	GetTargetVisibleFromWeapon() { return m_bTargetVisibleFromWeapon; }
	void	SetTargetVisibleFromWeapon(bool targetVisibleFromWeapon) { m_bTargetVisibleFromWeapon = targetVisibleFromWeapon; }

	float	GetTargetLastVisibleTime() { return m_fTargetLastVisibileTime; }
	void	SetTargetLastVisibleTime(float targetLastVisibleTime) { m_fTargetLastVisibileTime = targetLastVisibleTime; }

	int		GetBodyCount() { return m_nBodyCount; }
	void	SetBodyCount(int cBodyCount) { m_nBodyCount = cBodyCount; }

	float	GetLastDamage() { return m_fLastDamage; }
	void	SetLastDamage(float damage) { m_fLastDamage = damage; }

	int		GetRoundsFired() { return m_nRoundsFired; }
	void	SetRoundsFired(int nRoundsFired) { m_nRoundsFired = nRoundsFired; }

protected:
	// Blackboard data
	bool m_bAutoReload;
	float m_fSeeDistance;
	float m_fHearDistance;

	EnumAIAwareness m_eAwareness;

	bool m_bTargetReachable;
	Vector2D m_eTargetLocation;

	EnumAITargetType m_eTargetType;
	bool m_bTargetVisibleFromEye;
	bool m_bTargetVisibleFromWeapon;
	float m_fTargetLastVisibileTime;

	int m_nBodyCount;
	float m_fLastDamage;
	int m_nRoundsFired;
};