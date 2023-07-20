#pragma once
#include "Scene.h"
#include "TileMap.h"
#include "Player.h"
#include "Spikes.h"
#include "DestinationGo.h"
#include "UIGame.h"

class SceneGame : public Scene
{
protected:
	Player* player = nullptr;
	TileMap* tileMap = nullptr;
	DestinationGo* destination = nullptr;
	UIGame* uiGame = nullptr;

	std::vector<Spikes*> spikes = {};
	std::vector<SpriteGo*> BCoins = {};
	std::vector<SpriteGo*> SCoins = {};

	int score = 0;
	//bool isPause = false;

public:
	SceneGame();
	virtual ~SceneGame() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

