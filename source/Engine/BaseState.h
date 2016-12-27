/* ========================================================================
   File: BaseState.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

class BaseState {
public:
	// Virtual Functions
	virtual void Init() = 0;
	virtual void Clean() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
};



