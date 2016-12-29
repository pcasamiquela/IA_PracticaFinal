#include "Engine/EngineFramework.h"
#include "Player.h"
#include "Boid.h"

class PracticaFinalState : public BaseState{
public:
	virtual void Init() override;
	virtual void Clean() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
private:
	//Entities
	Player *player;
	Level *level_01;


	StateManager manager;

};