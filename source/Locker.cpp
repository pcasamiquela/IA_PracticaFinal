#include "Locker.h"

Locker::~Locker()
{
}

void Locker::Update(float deltaTime) {
	DibSprite::Update(deltaTime);
}

void Locker::Render() {
	DibSprite::Render();
}

void Locker::Clean() {
	DibSprite::Clean();
}

void Locker::ChangeSprite() {
	std::string assetPath = PathUtils::GetResourcesPath("images/LockerOpened.png");
	LoadGraphic(assetPath, "Locker", 32, 64, false);
}