#include "PracticaFinalState.h"
static int LevelArray[LEVEL_WIDTH][LEVEL_HEIGHT]{ 0 };



void PracticaFinalState::Init()
{
	//Grid init
	Grid_Init(&grid);

	std::string assetPath;

	//Texture
	arrowTexture = {};
	assetPath = PathUtils::GetResourcesPath("Images/Arrow.png");

	numSoldiers = 0;
	//Init level 01
	ReadFromFile(LEVEL_01, LevelArray);
	assetPath = PathUtils::GetResourcesPath("images/Floor_Tile.png");
	level_01 = new Level();
	level_01->LoadFromArray((int*)LevelArray, { 0.0f, 0.0f }, LEVEL_WIDTH, LEVEL_HEIGHT, assetPath, "Floor", CELL_WIDTH, CELL_HEIGHT, { GLOBAL_SCALE, GLOBAL_SCALE });
	//soldiersPool = new vector<ConeEnemyAgent>;

	obstacleNumber = level_01->solids.size();
	obstacle = new LOS_Obstacle[obstacleNumber];
	for (int i = 0; i < obstacleNumber; i++) {
		obstacle[i] = LOS_Obstacle_Create(level_01->solids[i]->GetPosition(), CELL_WIDTH, CELL_HEIGHT);
	}

	// Load player, enemies and gems
	LoadEntities((int*)LevelArray, { 0.0f, 0.0f },
		LEVEL_WIDTH, LEVEL_HEIGHT,
		CELL_WIDTH, CELL_HEIGHT,
		{ GLOBAL_SCALE, GLOBAL_SCALE });
	for (int i = 0; i < numSoldiers; i++) {
		CreateSoldier(i);
	}
	CreateGrid((int*)LevelArray);

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

	heuristicFunction = HeuristicUtils::ManhattanDistance;
	//steppedExecutionFinished = SteppedPathfindingUtils::SteppedPathfindAStar(&grid, path, heuristicFunction, useOptimization, allowDiagonals);
	//path = PathfindingUtils::PathfindAStar(&grid, startNode, endNode, heuristicFunction, useOptimization, allowDiagonals);

	for (int i = 0; i < soldiersPool.size(); i++) {
	////	/*ConeEnemyAgent *currentSoldiers = soldiersPool[i];
	////	currentSoldiers->Update(deltaTime);*/
		soldiersPool[i]->Update(deltaTime);
	}

	if (Input::Instance().GetKeyDown(KeyCode::Space)) {

		StartPathfinding(*soldiersPool[1], player->GetPosition());
		for (int i = 0; i < soldiersPool[1]->enemyPath.size(); i++) {
			SimplePath_AddPoint(&tempPath, soldiersPool[1]->enemyPath[i]->position*32);
		}

		soldiersPool[1]->simplePath = &tempPath;

	}
	player->Update(deltaTime);
}

void PracticaFinalState::Render()
{
	//cambiar por cases
	level_01->Render();
	for (int i = 0; i < grid.width; ++i) {
		for (int j = 0; j < grid.height; j++) {
			if (grid.array[i][j].cameFrom != nullptr)
			{
				int fromDirection = Node_GetDirection(&grid.array[i][j], grid.array[i][j].cameFrom);

				float angleToRender = 0.0f;
				switch (fromDirection) {
				case 0:
					angleToRender = 0.0f;
					break;
				case 1:
					angleToRender = 90.0f;
					break;
				case 2:
					angleToRender = 180.0f;
					break;
				case 3:
					angleToRender = 270.0f;
					break;
				case 4:
					angleToRender = 45.0f;
					break;
				case 5:
					angleToRender = 135.0f;
					break;
				case 6:
					angleToRender = 225.0f;
					break;
				case 7:
					angleToRender = 315.0f;
					break;

				}

				Texture_Render(&arrowTexture, Game::Instance().renderer,
					0.0f + i * grid.cellSize + grid.cellSize / 4,
					0.0f + j * grid.cellSize + grid.cellSize / 4,
					nullptr, 1.0f, 1.0f, angleToRender);
			}
		}
	}

	PathfindingUtils::RenderPathfindingPath(&grid, Game::Instance().renderer, soldiersPool[1]->enemyPath, Colors::WHITE, Vector2D(0));

	for (int i = 0; i < soldiersPool.size(); i++) {
		soldiersPool[i]->Render();
	}
	player->Render();
}

void PracticaFinalState::ReadFromFile(LevelState _levelState, int _LevelArray[LEVEL_WIDTH][LEVEL_HEIGHT]) {
	string fileName;
	switch (_levelState)	{
	case PracticaFinalState::LEVEL_01:
		fileName = "./../../../resources/Levels/Level_01.txt";
		break;
	default:
		string message = "The file " + string(fileName) + " doesn't exists";
		ErrorManagement::errorRunTime(message);
		break;
	}

	ifstream file;
	file.open(fileName);

	if (file.is_open()) {
		file >> numSoldiers;

		int temp;
		Vector2D tempVector;
		for (int i = 0; i < numSoldiers; i++) {
			file >> temp;
			numNodes.push_back(temp);
		}
		for (int i = 0; i < numSoldiers; i++) {
			SimplePath tempSimplePath;
			for (int j = 0; j < numNodes[i]; j++) {
				file >> tempVector.x  >> tempVector.y;
				SimplePath_AddPoint(&tempSimplePath, tempVector*CELL_WIDTH + 16.0f);
			}
			simplePathMap.insert(make_pair(i, tempSimplePath));
		}
	
		for (int i = 0; i < LEVEL_WIDTH; i++) {
			for (int j = 0; j < LEVEL_HEIGHT; j++) {
				file >> _LevelArray[i][j];
			}
		}

		file.close();
	}
	else {
		string message = "The file " + string(fileName) + " doesn't exists";
		ErrorManagement::errorRunTime(message);
	}
}
void PracticaFinalState::StartPathfinding(ConeEnemyAgent &currentEnemy, Vector2D targetPos) {
	
	ResetPathfinding(currentEnemy);
	int startX = currentEnemy.GetPosition().x / GRID_SIZE;
	int startY = currentEnemy.GetPosition().y / GRID_SIZE;
	int endX = targetPos.x / GRID_SIZE;
	int endY = targetPos.y / GRID_SIZE;

	Node* startNode = &grid.array[startX][startY];
	Node* endNode = &grid.array[endX][endY];

	currentEnemy.enemyPath = PathfindingUtils::PathfindAStar(&grid, startNode, endNode, heuristicFunction, false, false, 1.0f, 1.0f);
}

void PracticaFinalState::ResetPathfinding(ConeEnemyAgent& currentEnemy) {
	// Reset nodes
	for (int i = 0; i < grid.width; ++i)
	{
		for (int j = 0; j < grid.height; ++j)
		{
			Node_Reset(&grid.array[i][j]);
		}
	}
	// Reset path
	currentEnemy.enemyPath.clear();

	//isPathReset = true;
}



void PracticaFinalState::CreatePlayer(int x, int y) {
	//Init Player Collision
	std::string assetPath = PathUtils::GetResourcesPath("images/SnakeSpritesheet.png");
	player = new Player(x, y, 15, 26);
	player->LoadGraphic(assetPath, "Player", 15, 26, true);
	player->SetScale(GLOBAL_SCALE * 3, GLOBAL_SCALE * 3);
	//player->ShowCollisionBox(true);
	player->GetCollisionBox();
	player->SetSolidCollisions(level_01->solids);
	player->SetWidth(15);
	player->SetHeight(26/2);
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
	player->animationController->Add("Idle", 2, { 0 }, frameSpeed, true);
	player->animationController->Play("Idle");

}

void PracticaFinalState::CreateSoldier(int soldierNumber) {
	soldiersPool.push_back(new ConeEnemyAgent(0, 0, 0, 0, &questionTexture));
	if (soldiersPool[soldierNumber] != nullptr) {
		soldiersPool[soldierNumber]->Setup();
		soldiersPool[soldierNumber]->SetPosition(simplePathMap.find(soldierNumber)->second.pathArray[0].x, simplePathMap.find(soldierNumber)->second.pathArray[0].y);
		soldiersPool[soldierNumber]->SetActive(true);
		soldiersPool[soldierNumber]->targetPosition = &player->position;
		soldiersPool[soldierNumber]->SetBehavior(SIMPLE_PATH_FOLLOWING);
		soldiersPool[soldierNumber]->simplePath = &simplePathMap.find(soldierNumber)->second;
		//soldiersPool[soldierNumber]->SetSolidCollisions(level_01->solids);
		soldiersPool[soldierNumber]->losObstacleArraySize = &obstacleNumber;
		soldiersPool[soldierNumber]->losObstacleArray = obstacle;
			//(LOS_Obstacle*)level_01->solids[i];
	}
}

void PracticaFinalState::ReadPathFromFile(int cont) {

}
void PracticaFinalState::CreateGrid(int *levelArray) {
	Grid_Init(&grid);

	for (int i = 0; i < LEVEL_HEIGHT; i++)
	{
		for (int j = 0; j < LEVEL_WIDTH; j++)
		{
			if (*(levelArray + (i*LEVEL_WIDTH) + j) == 1 || *(levelArray + (i*LEVEL_WIDTH) + j) == 3 || *(levelArray + (i*LEVEL_WIDTH) + j) == 4)
				
			{
				grid.array[j][i].isWall = true;
			}
			else 
			{
				grid.array[j][i].isWall = false;
			}

		}
	}
}

void PracticaFinalState::LoadEntities(int* levelArray, Vector2D levelOrigin,
	int levelWidth, int levelHeight,
	int tileImageWidth, int tileImageHeight, Vector2D tileImageScale) {
	int currentSoldier = 0;
	// Loop to create player, enemies and gems objects
	for (int i = 0; i < levelHeight; ++i)
	{
		for (int j = 0; j < levelWidth; ++j)
		{
			if (*(levelArray + (i*levelWidth) + j) == 5)
			{
				CreatePlayer(levelOrigin.x + tileImageWidth * j * tileImageScale.x
					+ tileImageWidth * 0.5f * tileImageScale.x,
					levelOrigin.y + tileImageHeight * i * tileImageScale.y
					+ tileImageHeight * 0.5f * tileImageScale.y);
			}
			/*else if (*(levelArray + (i*levelWidth) + j) == 6)
			{
				CreateSoldier(currentSoldier);
				currentSoldier++;
			}*/

			//else if (*(levelArray + (i*levelWidth) + j) == 2)
			//{
			//	CreateEnemy(levelOrigin.x + tileImageWidth * j * tileImageScale.x
			//		+ tileImageWidth * 0.5f * tileImageScale.x,
			//		levelOrigin.y + tileImageHeight * i * tileImageScale.y
			//		+ tileImageHeight * 0.5f * tileImageScale.y, 0);
			//}
		}
	}

}