/* ========================================================================
   File: SoundManager.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include <SDL.h>
#include <SDL_mixer.h>       
#include <string>
#include <unordered_map>

enum SoundType
{
	SOUND_MUSIC,
	SOUND_EFFECT,
};

class SoundManager
{
public:
	// Singleton Pattern - Accessor
	static SoundManager& Instance()
	{
		static SoundManager instance; // Guaranteed to be destroyed.
										// Instantiated on first use.
		return instance;
	}
	// Singleton Pattern - Delete Copy constructor and operator=
	SoundManager(SoundManager const&) = delete;
	void operator=(SoundManager const&) = delete;

	// Member functions
	bool Load(std::string fileName, std::string id, SoundType type);
	bool Unload(std::string id, SoundType type);
	void Clean();

	void PlaySound(std::string id, bool loop = false);
	void PlayMusic(std::string id, bool loop = false);
	void PauseMusic();
	void ResumeMusic();
	void StopMusic();

private: 
	typedef std::unordered_map<std::string, Mix_Chunk*> EffectsMap;
	typedef std::unordered_map<std::string, Mix_Music*> MusicMap;
	
	// Member variables
	EffectsMap effectsMap;
	MusicMap musicMap;

	// Singleton Pattern - Constructor is private
	SoundManager() {}
};

