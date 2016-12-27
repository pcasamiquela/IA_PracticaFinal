/* ========================================================================
   File: Input.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Input.h"
#include "Game.h"

void Input::Init()
{
	// Reset mouse 
	mousePosition = Vector2D();
	for (int i = 0; i < 3; ++i)
	{
		mouseButtonDown[i] = false;
		mouseButtonUp[i] = false;
		mouseButton[i] = false;
	}

	// Initialize Keyboard State
	keyboardState = SDL_GetKeyboardState(&keyboardLength);
	oldKeyboardState = (Uint8*)SDL_malloc(keyboardLength);

	// Initialize Gamepad if present
	if (SDL_IsGameController(0))
	{
		gamepad = SDL_GameControllerOpen(0);
	}
}

void Input::HandleEvents()
{
	// Reset mouse 
	for (int i = 0; i < 3; ++i)
	{
		mouseButtonDown[i] = false;
		mouseButtonUp[i] = false;
	}

	// Update Keyboard State
	SDL_memcpy(oldKeyboardState, keyboardState, keyboardLength);
	keyboardState = SDL_GetKeyboardState(NULL);

	// Poll Events
	SDL_Event inputEvent;
	while (SDL_PollEvent(&inputEvent) != 0)
	{
		switch (inputEvent.type)
		{
		case SDL_QUIT:
			Game::Instance().Quit();
			break;
		case SDL_MOUSEMOTION:
			mousePosition = { (float)inputEvent.motion.x, (float)inputEvent.motion.y };
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (inputEvent.button.button == SDL_BUTTON_LEFT)
			{
				mouseButtonDown[0] = true;
				mouseButton[0] = true;
			}
			else if (inputEvent.button.button == SDL_BUTTON_RIGHT)
			{
				mouseButtonDown[1] = true;
				mouseButton[1] = true;
			}
			else if (inputEvent.button.button == SDL_BUTTON_MIDDLE)
			{
				mouseButtonDown[2] = true;
				mouseButton[2] = true;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (inputEvent.button.button == SDL_BUTTON_LEFT)
			{
				mouseButtonUp[0] = true;
				mouseButton[0] = false;
			}
			else if (inputEvent.button.button == SDL_BUTTON_RIGHT)
			{
				mouseButtonUp[1] = true;
				mouseButton[1] = false;
			}
			else if (inputEvent.button.button == SDL_BUTTON_MIDDLE)
			{
				mouseButtonUp[2] = true;
				mouseButton[2] = false;
			}
			break;
		default:
			break;
		}
	}

	if (gamepad != nullptr)
	{
		// Gamepad state
		gamepadButtonsState.dPadUp.wasDown = gamepadButtonsState.dPadUp.isDown;
		gamepadButtonsState.dPadUp.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_DPAD_UP);

		gamepadButtonsState.dPadDown.wasDown = gamepadButtonsState.dPadDown.isDown;
		gamepadButtonsState.dPadDown.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_DPAD_DOWN);

		gamepadButtonsState.dPadLeft.wasDown = gamepadButtonsState.dPadLeft.isDown;
		gamepadButtonsState.dPadLeft.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_DPAD_LEFT);

		gamepadButtonsState.dPadRight.wasDown = gamepadButtonsState.dPadRight.isDown;
		gamepadButtonsState.dPadRight.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

		gamepadButtonsState.start.wasDown = gamepadButtonsState.start.isDown;
		gamepadButtonsState.start.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_START);

		gamepadButtonsState.select.wasDown = gamepadButtonsState.select.isDown;
		gamepadButtonsState.select.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_BACK);

		gamepadButtonsState.buttonA.wasDown = gamepadButtonsState.select.isDown;
		gamepadButtonsState.buttonA.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_A);

		gamepadButtonsState.buttonB.wasDown = gamepadButtonsState.select.isDown;
		gamepadButtonsState.buttonB.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_B);

		gamepadButtonsState.buttonX.wasDown = gamepadButtonsState.select.isDown;
		gamepadButtonsState.buttonX.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_X);

		gamepadButtonsState.buttonY.wasDown = gamepadButtonsState.select.isDown;
		gamepadButtonsState.buttonY.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_Y);

		gamepadButtonsState.leftShoulder.wasDown = gamepadButtonsState.select.isDown;
		gamepadButtonsState.leftShoulder.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);

		gamepadButtonsState.rightShoulder.wasDown = gamepadButtonsState.select.isDown;
		gamepadButtonsState.rightShoulder.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
	}
}

bool Input::GetKey(KeyCode key)
{
	return (keyboardState[key] == 1);
}

bool Input::GetKeyDown(KeyCode key)
{
	return (keyboardState[key] == 1 && oldKeyboardState[key] == 0);
}

bool Input::GetKeyUp(KeyCode key)
{
	return (keyboardState[key] == 0 && oldKeyboardState[key] == 1);
}

bool Input::GetMouseButton(int button)
{
	return mouseButton[button];
}

bool Input::GetMouseButtonDown(int button)
{
	return mouseButtonDown[button];
}

bool Input::GetMouseButtonUp(int button)
{
	return mouseButtonUp[button];
}

void Input::Clean()
{
	SDL_free(oldKeyboardState);
	oldKeyboardState = nullptr;

	// Close Gamepad
	if (gamepad != nullptr)
	{
		SDL_GameControllerClose(gamepad);
		gamepad = nullptr;
	}
}

