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
	tileMap = (TileMap*)AddGo(new TileMap("graphics/item/tile_Map.png", "TileMap"));
	player = (Player*)AddGo(new Player("player"));

	for (int i = 0; i < 2; i++)
	{
		std::string num = std::to_string(i+1);
		Spikes* spikes = (Spikes*)AddGo(new Spikes("spikes"+num));
		this->spikes.push_back(spikes);
	}
	tileMap->SetSpikes(spikes);
	player->SetMap(tileMap);
	player->SetSpikes(spikes);
	// BUTTON TEST CODE
	{
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
	Scene::Update(dt);
	worldView.setCenter(player->GetPosition());	
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}