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
	auto size = FRAMEWORK.GetWindowSize();
	player = (Player*)AddGo(new Player("player"));

	// BUTTON TEST CODE
	//UIButton* testButton = (UIButton*)AddGo(new UIButton("graphics/button.png"));
	//testButton->SetOrigin(Origins::TR);
	//testButton->sortLayer = 100;
	//testButton->SetPosition(size.x, 0.f);
	//testButton->OnEnter = [testButton]() {
	//	std::cout << "Enter" << std::endl;
	//	sf::Texture* tex = RESOURCE_MGR.GetTexture("graphics/button2.png");
	//	testButton->sprite.setTexture(*tex);
	//};
	//testButton->OnClick = []() {
	//	std::cout << "Click" << std::endl;
	//};
	//testButton->OnExit = [testButton]() {
	//	std::cout << "Exit" << std::endl;
	//	sf::Texture* tex = RESOURCE_MGR.GetTexture(testButton->textureId);
	//	testButton->sprite.setTexture(*tex);
	//};

	tileMap = (TileMap*)AddGo(new TileMap("graphics/item/tileMap.png", "TileMap"));

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
	//worldView.setCenter(tileMap->GetPosition());

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
	worldView.setCenter(player->GetPosition());

	// test code
	//std::cout << player->GetPosition().x << "," << player->GetPosition().y << std::endl;
	//sf::Vector2i PlayerTileIndex = (sf::Vector2i)(player->GetPosition() / 30.f) - {};
	//std::cout << PlayerTileIndex.x << "," << PlayerTileIndex.y << std::endl;
	
	Scene::Update(dt);

	CheckCollide();
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneGame::CheckCollide() 
{
	// 플레이어 포지션에 해당하는 타일 찾기 !!!!!!!!!!!
	sf::Vector2i playerTileIndex = (sf::Vector2i)(player->GetPosition() / 30.f); // 플레이어가 속한 타일의 인덱스

	int tileSize = tileMap->tiles.size();
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Return))
	{
		std::cout << playerTileIndex.x << "," << playerTileIndex.y << std::endl;
		std::cout << player->GetPosition().x << "," << player->GetPosition().y << std::endl;
		std::cout << tileSize << std::endl;
	}
	for (int i = 0; i < tileSize; i++)
	{
		if (tileMap->tiles[i].texIndex == 4)
		{
			continue;
		}
		if (tileMap->tiles[i].x == playerTileIndex.x && tileMap->tiles[i].y == playerTileIndex.y) // 인덱스가 같으면
		{
			player->SpeedOnOff(false);
			break;
		}
	}	
	// 해당하는 타일의 텍스인덱스가 4가 아니면 충돌처리
}
