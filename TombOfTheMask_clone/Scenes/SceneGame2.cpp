#include "stdafx.h"
#include "SceneGame2.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Player.h"
#include "Framework.h"
#include "RectGo.h"
#include "UIButton.h"

SceneGame2::SceneGame2() : Scene(SceneId::Game2)
{
	resourceListPath = "script/defaultResourceList.csv";
}

void SceneGame2::Init()
{
	Release();

	// GAME OBJECTS
	{
		backEffect = (RectGo*)AddGo(new RectGo("backEffect"));
		backEffect->rect.setFillColor(sf::Color(255,255,0,255)); // YELLOW, 0%
		backEffect->rect.setSize(screenSize);
		backEffect->sortLayer = -1;
		backEffect->SetActive(false);

		for (int i = 0; i < 50; ++i)
		{
			std::string num = std::to_string(i + 1);
			Spikes* spikes = (Spikes*)AddGo(new Spikes("spikes" + num));
			this->spikes.push_back(spikes);
		}

		for (int i = 0; i < 179; ++i)
		{
			std::string num = std::to_string(i + 1);
			SpriteGo* scoins = (SpriteGo*)AddGo(new SpriteGo("graphics/item/Coin_2.png", "scoins" + num));
			scoins->SetOrigin(Origins::MC);
			scoins->sprite.setColor(sf::Color::Yellow);
			this->SCoins.push_back(scoins);
		}

		for (int i = 0; i < 53; ++i)
		{
			std::string num = std::to_string(i + 1);
			SpriteGo* bcoins = (SpriteGo*)AddGo(new SpriteGo("graphics/item/Coin_addict_1.png", "bcoins" + num));
			bcoins->SetOrigin(Origins::MC);
			bcoins->sprite.setColor(sf::Color::Yellow);
			this->BCoins.push_back(bcoins);
		}
		
		destination = (DestinationGo*)AddGo(new DestinationGo("destination"));
		entrance = (EntranceGo*)AddGo(new EntranceGo("entrance"));

		tileMap = (TileMap*)AddGo(new TileMap("graphics/item/tile_Map.png", "TileMap2"));
		player = (Player*)AddGo(new Player("player"));

		tileMap->Release();
		tileMap->SetSpikes(spikes);
		tileMap->SetBCoins(BCoins);
		tileMap->SetSCoins(SCoins);
		tileMap->SetDestination(destination);
		tileMap->Load("map/map_2.csv");
		tileMap->SetOrigin(Origins::TL);

		player->SetMap(tileMap);
		player->SetSpikes(spikes);
		player->SetBCoins(BCoins);
		player->SetSCoins(SCoins);
		player->SetDestination(destination);
		player->initialPos = { tileMap->GetPosition(9,50) };
		player->sceneIndex = 2;
	}

	// UI OBJECTS
	{
		// USING CODE
		uiGame = (UIGame*)AddGo(new UIGame("uiGame"));
		uiGame->SetOrigin(Origins::MC);
		uiGame->SetMaxScore(3700);
		uiGame->SetPosition(screenSize * 0.5f);
		uiGame->sortLayer = 100;
	}

	for (auto go : gameObjects)
	{
		go->Init();
	}

	//tileMap->RotateSpikes();

}

void SceneGame2::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneGame2::Enter()
{
	Scene::Enter();

	auto size = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = size * 0.5f;
	worldView.setSize(size);

	uiView.setSize(size);
	uiView.setCenter(centerPos);

	backView.setSize(size);
	backView.setCenter(centerPos);

	entrance->SetPosition(player->GetPosition().x, player->GetPosition().y + 70.f);

	curScore = player->GetScore();
	uiGame->SetMaxScore(3700); // 수정 필요
	uiGame->SetCurScore(curScore);
	uiGame->SetPastScore(SCENE_MGR.GetCurrScene()->score);

	backEffect->SetActive(false);

	count = 3;
}

void SceneGame2::Exit()
{
	Scene::Exit();
}

void SceneGame2::Update(float dt)
{
	// PLAYER-UI SETTING
	if (!uiGame->isPause && !player->isDie && !player->isWin)
	{
		Scene::Update(dt);
		worldView.setCenter(player->GetPosition());	
		curScore = player->GetScore();
		uiGame->SetCurScore(curScore);
		uiGame->SetPastScore(SCENE_MGR.GetCurrScene()->score);
	}
	else if(player->isWin || player->isDie)
	{
		if (count > 0)
		{
			BackEffect(dt, player->isWin);
			player->isMoving = false;
			player->Update(dt);
			auto tailUseList = player->poolTails.GetUseList();
			for (auto i : tailUseList)
			{
				i->Update(dt);
			}
			//tails->Update(dt);
		}
		else if(player->isWin)
		{
			uiGame->winWindow = true;
			uiGame->Update(dt);
			if (!uiGame->winWindow)
			{
				player->isWin = false;
			}
		}
		else if (player->isDie)
		{
			uiGame->dieWindow = true;
			uiGame->Update(dt);
			if (!uiGame->dieWindow)
			{
				player->isDie = false;
			}
		}
	}
	else if(uiGame->isPause)
	{
		uiGame->Update(dt);
	}

	// REPLAY
	if (uiGame->replay)
	{
		SCENE_MGR.GetCurrScene()->score = uiGame->GetPastScore();
		SCENE_MGR.ChangeScene(SceneId::Game2);
	}
}

void SceneGame2::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneGame2::BackEffect(float dt, bool status)
{
	backEffect->SetActive(true);
	totalTime += dt;
	if (totalTime <= 0.5f)
	{
		if (status)
		{
			backEffect->rect.setFillColor(sf::Color(255, 255, 0, 255.f - totalTime * 510.f));
		}
		else
		{
			backEffect->rect.setFillColor(sf::Color(255, 0, 0, 255.f - totalTime * 510.f));
		}
		if (totalTime <= 0.5f && totalTime >= 0.47f)
		{
			totalTime = 0.01f;
			count--;
		}
	}
}
