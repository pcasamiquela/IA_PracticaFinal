/* ========================================================================
   File: MenuState.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "MenuState.h"

void MenuState::Init()
{
	// Initialize Font
	std::string assetPath = PathUtils::GetResourcesPath("Fonts/nokiafc22.ttf");
	fontSmall = FC_CreateFont();
	FC_LoadFont(fontSmall, Game::Instance().renderer, 
		assetPath.c_str(), 30, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
	fontBig = FC_CreateFont();
	FC_LoadFont(fontBig, Game::Instance().renderer, 
		assetPath.c_str(), 40, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
}

void MenuState::Clean()
{
	// Free fonts
	FC_FreeFont(fontBig);
	FC_FreeFont(fontSmall);
}

void MenuState::Update(float deltaTime)
{
	// Menu Input
	if (Input::Instance().GetKeyDown(KeyCode::Escape))
	{
		Game::Instance().Quit();
		return;
	}

	if (Input::Instance().GetKeyDown(KeyCode::Space))
	{
		Game::Instance().ChangeState(PRACTICAFINAL_STATE);
		return;
	}



	//drawPromptCounter += deltaTime;
	//if (drawPromptCounter > K_DRAW_PROMPT_THRESHOLD)
	//{
	//	drawPrompt = !drawPrompt;
	//	drawPromptCounter = 0.0f;
	//}
}

void MenuState::Render()
{
	// Render Text
	FC_DrawAlign(fontSmall, Game::Instance().renderer,
		FIXED_WIDTH,
		0,
		FC_ALIGN_RIGHT,
		(char*)" > Practica Final");

	FC_DrawAlign(fontBig, Game::Instance().renderer,
		FIXED_WIDTH / 2,
		FIXED_HEIGHT / 2 - 100,
		FC_ALIGN_CENTER,
		(char*)"Aii ke te pilloooo");

	//if (drawPrompt == true)
	//{
		FC_DrawAlign(fontSmall, Game::Instance().renderer,
			FIXED_WIDTH / 2,
			FIXED_HEIGHT / 2 + 50,
			FC_ALIGN_CENTER,
			(char*)"Press SPACE to Play");
	//}	
}


