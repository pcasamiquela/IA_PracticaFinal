#include "Engine/EngineFramework.h"
#include "Player.h"
#include "Boid.h"
#include "ConeEnemyAgent.h"
#include "ErrorManagement.h"
#include <fstream>
#include <iostream>
#include <map>
#include "PathfindingUtils.h"

using namespace std;

class PracticaFinalState : public BaseState{
public:
	virtual void Init() override;
	virtual void Clean() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
private:
	//Level State
	enum LevelState
	{
		LEVEL_01
	};
	//PathFinding

	HeuristicFunction heuristicFunction = &HeuristicUtils::ManhattanDistance;

	bool steppedExecutionFinished = true;
	Grid grid;
	//Textures
	Texture arrowTexture;
	Texture questionTexture;
	//Obstacles
	LOS_Obstacle *obstacle;
	int obstacleNumber;
	//Entities
	Player *player;
	Player *playerToShow;
	//GenericPool<Boid>* soldiersPool
	vector<ConeEnemyAgent*> soldiersPool;
	Level *level_01;
	int numSoldiers;
	vector <int> numNodes;
	//Simple Path
	//vector <vector<SimplePath>> simplePathVector;
	SimplePath tempPath;

	map<int, SimplePath> simplePathMap;
	SimplePath path1;
	StateManager manager;
	void ReadFromFile(LevelState _levelState, int _LevelArray[LEVEL_WIDTH][LEVEL_HEIGHT]);
	void CreatePlayer(int x, int y);
	void CreateSoldier(int soldierNumber);
	void LoadEntities(int* levelArray, Vector2D levelOrigin,int levelWidth, int levelHeight,int tileImageWidth, int tileImageHeight, Vector2D tileImageScale);
	void ReadPathFromFile(int cont);
	void CreateGrid(int* levelArray);
	void StartPathfinding(ConeEnemyAgent &currentEnemy, Vector2D targetPos);
	void ResetPathfinding(ConeEnemyAgent& currentEnemy);
};