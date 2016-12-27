/* Copyright (C) Steve Rabin, 2000.
* All rights reserved worldwide.
*
* This software is provided "as is" without express or implied
* warranties. You may freely copy and compile this source into
* applications you distribute provided that the copyright text
* below is included in the resulting source code, for example:
* "Portions Copyright (C) Steve Rabin, 2000"
*/
//////////////////////////////////////////////////////////////
//
// Filename: fsmmacros.h
//
// Author: Steve Rabin
// E-mail: stevera@noa.nintendo.com
// From the book "Game Programming Gems"
// From the article "Designing a General Robust AI Engine"
//
// Brief Disclaimer:
// This code is free to use for commercial use and is in the
// public domain. You may distribute, copy, modify, or use as
// is as long as this information is present. This code makes
// no guarantees written or implied and is provided solely as
// an example. Have a nice day!
//
// Purpose:
// This file contains the macros that make up the state machine
// language. These macros are used in the file "fsm_drone.cpp".
// These macros are designed like a jigsaw puzzle to line up
// with each other. That's the reason there are if(0)'s and such.
// Try expanding the macros in the file "fsm_drone.cpp" to see
// why they work.
//
//////////////////////////////////////////////////////////////

#pragma once

#include "Engine/EngineFramework.h"

//This is the state machine language.
//To see the keywords, look in the file "fsmmacros.h".
//You can get MS Visual Studio to highlight these words by listing them in
//a text file called "usertype.dat" put in the same directory where Msdev.exe
//is stored (like C:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin).
//You'll find the "usertype.dat file in the same directory this file is in.
//Just copy it to the correct directory.

#define BeginStateMachine   if( state < 0 ) { if(0) {
#define EndStateMachine     return( true ); } } else { SDL_Log("Invalid State"); \
                            return( false );}  return( false );
#define State(a)            return( true ); } } else if( a == state ) { \
							char stateName[64] = #a; if(0) { 
#define OnEvent(a)          return( true ); } else if( a == event ) { \
							char eventName[64] = #a;
#define OnEnter             OnEvent(EVENT_Enter) \
							SDL_Log("State Machine State %s; Event %s", stateName, eventName);
#define OnUpdate            OnEvent(EVENT_Update) 
#define OnExit              OnEvent(EVENT_Exit) \
							SDL_Log("State Machine State %s; Event %s", stateName, eventName);
