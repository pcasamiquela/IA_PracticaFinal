/* ========================================================================
   File: GenericPool.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

template<typename T>
class GenericPool
{
public:
	GenericPool(int numElements) :
		numElements{ numElements }
	{
		pool = (T*)SDL_calloc(numElements, sizeof(T));

		Reset();
	};

	void Update(float deltaTime)
	{
		for (int i = 0; i < numElements; ++i)
		{
			pool[i].Update(deltaTime);
		}
	}

	void Render()
	{
		for (int i = 0; i < numElements; ++i)
		{
			pool[i].Render();
		}
	}

	void Clean()
	{
		SDL_free(pool);
	}

	void Reset()
	{
		for (int i = 0; i < numElements; ++i)
		{
			new(&pool[i]) T();
			pool[i].Setup();
			SDL_Log("Created Element %d", i);
		}
	}

	T* GetElement()
	{
		for (int i = 0; i < numElements; ++i)
		{
			if (pool[i].GetActive() == false)
			{
				return &pool[i];
			}
		}
		SDL_Log("No elements available in the pool");
		return nullptr;
	}

	// Data
	T* pool;
	int numElements;
};