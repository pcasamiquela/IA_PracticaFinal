#pragma once
#include "Engine/EngineFramework.h"
class Locker : public DibSprite
{
public:
	Locker(int x, int y, int width, int height) : DibSprite(x, y, width, height) {};
	~Locker();
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Clean() override;
	bool playerInside = false;
	void ChangeSprite();
};

