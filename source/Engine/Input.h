/* ========================================================================
   File: Input.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include <SDL.h>
#include "Vector2D.h"

struct ButtonState
{
	Uint8 wasDown;
	Uint8 isDown;
};

struct GamepadState
{
	ButtonState dPadUp;
	ButtonState dPadDown;
	ButtonState dPadLeft;
	ButtonState dPadRight;
	ButtonState start;
	ButtonState select;
	ButtonState buttonA;
	ButtonState buttonB;
	ButtonState buttonX;
	ButtonState buttonY;
	ButtonState leftShoulder;
	ButtonState rightShoulder;
};

enum KeyCode
{
	Unknown = 0,

	A = 4,
	B = 5,
	C = 6,
	D = 7,
	E = 8,
	F = 9,
	G = 10,
	H = 11,
	I = 12,
	J = 13,
	K = 14,
	L = 15,
	M = 16,
	N = 17,
	O = 18,
	P = 19,
	Q = 20,
	R = 21,
	S = 22,
	T = 23,
	U = 24,
	V = 25,
	W = 26,
	X = 27,
	Y = 28,
	Z = 29,

	Alpha1 = 30,
	Alpha2 = 31,
	Alpha3 = 32,
	Alpha4 = 33,
	Alpha5 = 34,
	Alpha6 = 35,
	Alpha7 = 36,
	Alpha8 = 37,
	Alpha9 = 38,
	Alpha0 = 39,

	Return = 40,
	Escape = 41,
	Backspace = 42,
	Tab = 43,
	Space = 44,

	F1 = 58,
	F2 = 59,
	F3 = 60,
	F4 = 61,
	F5 = 62,
	F6 = 63,
	F7 = 64,
	F8 = 65,
	F9 = 66,
	F10 = 67,
	F11 = 68,
	F12 = 69,

	Home = 74,
	PageUp = 75,
	Delete = 76,
	End = 77,
	PageDown = 78,
	RightArrow = 79,
	LeftArrow = 80,
	DownArrow = 81,
	UpArrow = 82,

	LeftControl = 224,
	LeftShift = 225,
	LeftAlt = 226, /* alt, option */
	LeftCommand = 227, /* windows, command (apple), meta */
	RightControl = 228,
	RightShift = 229,
	RightAlt = 230, /* alt gr, option */
	RightCommand = 231, /* windows, command (apple), meta */
};

class Input
{
public:
	// Singleton Pattern - Accessor
	static Input& Instance()
	{
		static Input instance;	// Guaranteed to be destroyed.
								// Instantiated on first use.
		return instance;
	}
	// Singleton Pattern - Delete Copy constructor and operator=
	Input(Input const&) = delete;
	void operator=(Input const&) = delete;

	// Member variables

	// Mouse
	Vector2D mousePosition;
	// Gamepad
	GamepadState gamepadButtonsState;

	// Member functions
	void Init();
	void HandleEvents();
	void Clean();

	// Keyboard Functions
	bool GetKey(KeyCode key); // Returns whether the given key is held down.
	bool GetKeyDown(KeyCode key); // Returns true during the frame the user pressed the key.
	bool GetKeyUp(KeyCode key); // Returns true during the frame the user releases the given key.

	// Mouse Functions
	// Button values are 0 for left button, 1 for right button, 2 for the middle button
	bool GetMouseButton(int button); // Returns whether the given mouse button is held down.
	bool GetMouseButtonDown(int button); // Returns true during the frame the user pressed the given mouse button.
	bool GetMouseButtonUp(int button); // Returns true during the frame the user releases the given mouse button.

private:
	// Singleton Pattern - Constructor is private
	Input() {}

	// Keyboard State
	int keyboardLength;
	Uint8 *oldKeyboardState;
	const Uint8 *keyboardState;

	// Mouse
	bool mouseButtonDown[3];
	bool mouseButtonUp[3];
	bool mouseButton[3];

	// Gamepad
	SDL_GameController* gamepad;
};