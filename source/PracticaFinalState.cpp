#include "PracticaFinalState.h"
static const int LEVEL_WIDTH = 80;
static const int LEVEL_HEIGHT = 34;
static int LEVEL[LEVEL_WIDTH][LEVEL_HEIGHT]  { 0 };


void PracticaFinalState::Init()
{

	//Init Player
	std::string assetPath = PathUtils::GetResourcesPath("images/SnakeSpritesheet.png");
	player = new Player(FIXED_WIDTH/2, FIXED_HEIGHT/2, 15, 26);
	player->LoadGraphic(assetPath, "Player", 15, 26, true);
	player->SetScale(GLOBAL_SCALE*3, GLOBAL_SCALE*3);

	//Init level 01
	assetPath = PathUtils::GetResourcesPath("images/Floor_Tile.png");
	level_01 = new Level();
	level_01->LoadFromArray((int*)LEVEL, { 0.0f, 0.0f }, LEVEL_WIDTH, LEVEL_HEIGHT, assetPath, "Floor", 32, 32, { GLOBAL_SCALE, GLOBAL_SCALE });


	//Init Player Animation
	int frameSpeed = 10;
	player->animationController->Add("RunRight", 2, { 1,2,3,4 }, frameSpeed, true);
	player->animationController->Add("RunLeft", 6, { 0,1,2,3 }, frameSpeed, true);
	player->animationController->Add("RunUp", 0, { 1,2,3,4 }, frameSpeed, true);
	player->animationController->Add("RunDown", 4, { 1,2,3,4 }, frameSpeed, true);
	//Diagonal
	player->animationController->Add("RunDownDiagonalRight", 3, { 0,1,2,3 }, frameSpeed, true);
	player->animationController->Add("RunDownDiagonalLeft", 5, { 1,2,3,4 }, frameSpeed, true);
	player->animationController->Add("RunUpDiagonalRight", 1, { 1,2,3,4 }, frameSpeed, true);
	player->animationController->Add("RunUpDiagonalLeft", 7, { 0,1,2,3 }, frameSpeed, true);
	player->animationController->Add("Idle", 2, {0}, frameSpeed, true);
	player->animationController->Play("Idle");
}

void PracticaFinalState::Clean()
{
}

void PracticaFinalState::Update(float deltaTime)
{
	if (Input::Instance().GetKeyDown(KeyCode::Escape))
	{
		Game::Instance().ChangeState(MENU_STATE);
		return;
	}
	player->Update(deltaTime);
}

void PracticaFinalState::Render()
{
	//cambiar por cases
	level_01->Render();
	player->Render();
}