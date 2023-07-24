#include "stdafx.h"
#include "SceneTitle.h"
#include "Framework.h"
#include "GameObject.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"

SceneTitle::SceneTitle(SceneId id) : Scene(SceneId::Title)
{
	resourceListPath = "script/defaultResourceList.csv";
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	Release();

	logo = (LogoGo*)AddGo(new LogoGo("logo"));

	for (auto go : gameObjects)
	{
		go->Init();
	}

	// Å¸ÀÌÆ² ºä
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;
	titleView.setSize(windowSize);
	titleView.setCenter(centerPos);
}

void SceneTitle::Release()
{
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneTitle::Enter()
{
	Scene::Enter();
}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
	{
		SCENE_MGR.ChangeScene(SceneId::Game);
	}
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	//Scene::Draw(window);

	window.setView(titleView);
	for (auto go : gameObjects)
	{
		if (go->sortLayer >= 100)
			continue;

		if (go->GetActive())
		{
			go->Draw(window);
		}
	}
}
