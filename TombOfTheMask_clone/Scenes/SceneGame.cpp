#include "stdafx.h"
#include "SceneGame.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Player.h"
#include "Framework.h"
#include "RectGo.h"
#include "UIButton.h"

SceneGame::SceneGame() : Scene(SceneId::Game)
{
	resourceListPath = "script/SceneGameResourceList.csv";
}

void SceneGame::Init()
{
	Release();

	// GAME OBJECTS
	{
		for (int i = 0; i < 2; ++i)
		{
			std::string num = std::to_string(i + 1);
			Spikes* spikes = (Spikes*)AddGo(new Spikes("spikes" + num));
			this->spikes.push_back(spikes);
		}

		for (int i = 0; i < 78; ++i)
		{
			std::string num = std::to_string(i + 1);
			SpriteGo* scoins = (SpriteGo*)AddGo(new SpriteGo("graphics/item/Coin_2.png", "scoins" + num));
			scoins->SetOrigin(Origins::MC);
			scoins->sprite.setColor(sf::Color::Yellow);
			this->SCoins.push_back(scoins);
		}

		for (int i = 0; i < 5; ++i)
		{
			std::string num = std::to_string(i + 1);
			SpriteGo* bcoins = (SpriteGo*)AddGo(new SpriteGo("graphics/item/Coin_addict_1.png", "bcoins" + num));
			bcoins->SetOrigin(Origins::MC);
			bcoins->sprite.setColor(sf::Color::Yellow);
			this->BCoins.push_back(bcoins);
		}
		
		destination = (DestinationGo*)AddGo(new DestinationGo("destination"));

		tileMap = (TileMap*)AddGo(new TileMap("graphics/item/tile_Map.png", "TileMap"));
		player = (Player*)AddGo(new Player("player"));

		tileMap->SetSpikes(spikes);
		tileMap->SetBCoins(BCoins);
		tileMap->SetSCoins(SCoins);
		tileMap->SetDestination(destination);

		player->SetMap(tileMap);
		player->SetSpikes(spikes);
		player->SetBCoins(BCoins);
		player->SetSCoins(SCoins);
		player->SetDestination(destination);
	}

	// UI OBJECTS
	{
		// TEST CODE
		//{
		//	SpriteGo* uiCoin = (SpriteGo*)AddGo(new SpriteGo("graphics/item/Bonus_Coin.png", "uiCoin"));
		//	uiCoin->sprite.setColor(sf::Color::Yellow);
		//	uiCoin->SetOrigin(Origins::TL);
		//	uiCoin->SetPosition(0,0);
		//	uiCoin->sortLayer = 100;
		//}

		// USING CODE
		uiGame = (UIGame*)AddGo(new UIGame("uiGame"));
		uiGame->SetOrigin(Origins::MC);
		uiGame->SetMaxScore(920);
		uiGame->SetPosition(screenSize * 0.5f);
		uiGame->sortLayer = 100;
	}

	for (auto go : gameObjects)
	{
		go->Init();
	}

	tileMap->Load("map/map_1.csv");
	tileMap->SetOrigin(Origins::TL);

}

void SceneGame::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneGame::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = size * 0.5f;
	worldView.setSize(size);

	uiView.setSize(size);
	uiView.setCenter(centerPos);

	Scene::Enter();
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	if (!uiGame->isPause && !player->isDie)
	{
		Scene::Update(dt);
		worldView.setCenter(player->GetPosition());	
		score = player->GetScore();
		uiGame->SetScore(score);
	}
	else if (player->isDie)
	{
		uiGame->dieWindow = true;
		uiGame->Update(dt);
	}
	else if(player->isWin)
	{
		uiGame->winWindow = true;
		uiGame->Update(dt);
	}
	else if(uiGame->isPause)
	{
		uiGame->Update(dt);
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}