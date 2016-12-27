/* ========================================================================
   File: SoundManager.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "SoundManager.h"

bool SoundManager::Load(std::string fileName, std::string id, SoundType type)
{
	if (type == SOUND_MUSIC)
	{
		if (musicMap.find(id) == musicMap.end())
		{
			Mix_Music* music = Mix_LoadMUS(fileName.c_str());
			if (music == nullptr)
			{
				SDL_Log("Could not load music %s.\nMix_Error %s",
					fileName.c_str(), Mix_GetError());
				return false;
			}
			musicMap[id] = music;
			return true;
		}
		else
		{
			SDL_Log("Music with id %s already exists in the SoundManager.", id.c_str());
			return false;
		}
	}
	else if (type == SOUND_EFFECT)
	{
		if (effectsMap.find(id) == effectsMap.end())
		{
			Mix_Chunk* chunk = Mix_LoadWAV(fileName.c_str());
			if (chunk == nullptr)
			{
				SDL_Log("Could not load effect %s.\nMix_Error %s",
					fileName.c_str(), Mix_GetError());
				return false;
			}
			effectsMap[id] = chunk;
			return true;
		}
		else
		{
			SDL_Log("Effect with id %s already exists in the SoundManager.", id.c_str());
			return false;
		}
	}
	return false;
}

bool SoundManager::Unload(std::string id, SoundType type)
{
	if (type == SOUND_MUSIC)
	{
		MusicMap::iterator entry = musicMap.find(id);
		if (entry != musicMap.end())
		{
			Mix_FreeMusic(musicMap[id]);
			musicMap.erase(entry);
			return true;
		}
		else
		{
			SDL_Log("Music with id %s not present in the SoundManager.", id.c_str());
			return false;
		}
	}
	else if (type == SOUND_EFFECT)
	{
		EffectsMap::iterator entry = effectsMap.find(id);
		if (entry != effectsMap.end())
		{
			Mix_FreeChunk(effectsMap[id]);
			effectsMap.erase(entry);
			return true;
		}
		else
		{
			SDL_Log("Effect with id %s not present in the SoundManager.", id.c_str());
			return false;
		}
	}
	return false;
}

void SoundManager::PlaySound(std::string id, bool loop)
{
	int loopFlag = loop ? -1 : 0;
	Mix_PlayChannel(-1, effectsMap[id], loopFlag);
}

void SoundManager::PlayMusic(std::string id, bool loop)
{
	int loopFlag = loop ? -1 : 0;
	Mix_PlayMusic(musicMap[id], loopFlag);
}

void SoundManager::PauseMusic()
{
	Mix_PauseMusic();
}

void SoundManager::ResumeMusic()
{
	Mix_ResumeMusic();
}

void SoundManager::StopMusic()
{
	Mix_HaltMusic();
}

void SoundManager::Clean()
{
	for (auto entry : effectsMap) 
	{
		Mix_FreeChunk(entry.second);
		effectsMap.erase(entry.first);
	}

	for (auto entry : musicMap)
	{
		Mix_FreeMusic(entry.second);
		musicMap.erase(entry.first);
	}
}

