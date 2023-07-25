#pragma once
#include "Scene.h"
#include "TileMap.h"
#include "Player.h"
#include "Spikes.h"
#include "DestinationGo.h"
#include "UIGame.h"
#include "EntranceGo.h"
#include "TailsGo.h"

class SceneGame2 : public Scene
{
protected:
	Player* player = nullptr;
	TileMap* tileMap = nullptr;
	DestinationGo* destination = nullptr;
	EntranceGo* entrance = nullptr;
	UIGame* uiGame = nullptr;
	RectGo* backEffect = nullptr;
	RectGo* deathWater = nullptr;

	std::vector<Spikes*> spikes = {};
	std::vector<SpriteGo*> BCoins = {};
	std::vector<SpriteGo*> SCoins = {};

	int curScore = 0;
	int count = 3;
	float backEffectTime = 0.f;

public:
	SceneGame2();
	virtual ~SceneGame2() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	Player* GetPlayer() { return player; };

	void BackEffect(float dt, bool status);
};