#include "Engine/EngineFramework.h"
#include "Player.h"
#include "Boid.h"
#include "ErrorManagement.h"
#include <fstream>
#include <iostream>
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

	//Entities
	Player *player;
	Player *playerToShow;
	Level *level_01;


	StateManager manager;
	void ReadFromFile(LevelState _levelState, int _LevelArray[LEVEL_WIDTH][LEVEL_HEIGHT]);
	void CreatePlayer(int x, int y);
	void LoadEntities(int* levelArray, Vector2D levelOrigin,int levelWidth, int levelHeight,int tileImageWidth, int tileImageHeight, Vector2D tileImageScale);
};