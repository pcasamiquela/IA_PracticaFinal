/* ========================================================================
   File: DynamicBlackboard.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Engine/EngineFramework.h"

#include <unordered_map>
#include <string>

// Dynamic Blackboard
class DynamicBlackboard
{
public:
	// Constructor / Destructor
	DynamicBlackboard() {};
	~DynamicBlackboard() {};

	// Setters / Getters
	float GetFloat(std::string key, float defaultValue = 0.0f);
	void SetFloat(std::string key, float data);

	int GetInt(std::string key, int defaultValue = 0);
	void SetInt(std::string key, int data);

	bool GetBool(std::string key, bool defaultValue = false);
	void SetBool(std::string key, bool data);

	void PrintData();

protected:
	typedef std::unordered_map<std::string, int> IntTable;
	IntTable dynamicIntTable;

	typedef std::unordered_map<std::string, bool> BoolTable;
	BoolTable dynamicBoolTable;

	typedef std::unordered_map<std::string, float> FloatTable;
	FloatTable dynamicFloatTable;
};