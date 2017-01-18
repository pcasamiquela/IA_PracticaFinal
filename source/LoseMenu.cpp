#include "LoseMenu.h"

void LoseMenu::Init()
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

void LoseMenu::Clean()
{
	// Free fonts
	FC_FreeFont(fontBig);
	FC_FreeFont(fontSmall);
}

void LoseMenu::Update(float deltaTime)
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


}

void LoseMenu::Render()
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
		(char*)"You Lose!");

	//if (drawPrompt == true)
	//{
	FC_DrawAlign(fontSmall, Game::Instance().renderer,
		FIXED_WIDTH / 2,
		FIXED_HEIGHT / 2 + 50,
		FC_ALIGN_CENTER,
		(char*)"Press SPACE to Play");
	//}	
}
