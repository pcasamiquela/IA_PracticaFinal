/* ========================================================================
   File: DynamicBlackboard.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "DynamicBlackboard.h"

using namespace std;

float DynamicBlackboard::GetFloat(string key, float defaultValue)
{
	FloatTable::iterator entry = dynamicFloatTable.find(key);
	if (entry != dynamicFloatTable.end())
	{
		return entry->second;
	}
	else
	{
		SDL_Log("Float with key %s not present in the DynamicBlackboard.", key.c_str());
		return defaultValue;
	}
}

void DynamicBlackboard::SetFloat(string key, float data)
{
	dynamicFloatTable.emplace(key, data);
}

int DynamicBlackboard::GetInt(string key, int defaultValue)
{
	IntTable::iterator entry = dynamicIntTable.find(key);
	if (entry != dynamicIntTable.end())
	{
		return entry->second;
	}
	else
	{
		SDL_Log("Int with key %s not present in the DynamicBlackboard.", key.c_str());
		return defaultValue;
	}
}

void DynamicBlackboard::SetInt(string key, int data)
{
	dynamicIntTable.emplace(key, data);
}

bool DynamicBlackboard::GetBool(string key, bool defaultValue)
{
	BoolTable::iterator entry = dynamicBoolTable.find(key);
	if (entry != dynamicBoolTable.end())
	{
		return entry->second;
	}
	else
	{
		SDL_Log("Bool with key %s not present in the DynamicBlackboard.", key.c_str());
		return defaultValue;
	}
}

void DynamicBlackboard::SetBool(string key, bool data)
{
	dynamicBoolTable.emplace(key, data);
}

void DynamicBlackboard::PrintData()
{
	SDL_Log("DynamicBlackboard Contents");

	// Print data from dynamicFloatTable 
	SDL_Log("Float Contents");
	for (FloatTable::iterator itr = dynamicFloatTable.begin(); 
		itr != dynamicFloatTable.end(); ++itr) 
	{
		SDL_Log("Key = %s - Value = %f", itr->first.c_str(), itr->second);
	} 

	// Print data from dynamicIntTable
	SDL_Log("Int Contents");
	for (IntTable::iterator itr = dynamicIntTable.begin(); 
		itr != dynamicIntTable.end(); ++itr)
	{
		SDL_Log("Key = %s - Value = %d", itr->first.c_str(), itr->second);
	}

	// Print data from dynamicBoolTable 
	SDL_Log("Bool Contents");
	for (BoolTable::iterator itr = dynamicBoolTable.begin(); 
		itr != dynamicBoolTable.end(); ++itr)
	{
		SDL_Log("Key = %s - Value = %d", itr->first.c_str(), itr->second);
	}
}
