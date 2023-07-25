#include "stdafx.h"
#include "SceneTitle.h"
#include "Framework.h"
#include "GameObject.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "StringTable.h"
#include "DataTableMgr.h"

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
	playText = (TextGo*)AddGo(new TextGo("playText", "fonts/GalmuriMono7.ttf"));

	for (auto go : gameObjects)
	{
		go->Init();
	}

	sf::Vector2f centerPos = FRAMEWORK.GetWindowSize() * 0.5f;

	// 타이틀 뷰
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	titleView.setSize(windowSize);
	titleView.setCenter(centerPos);

	StringTable* stringTable1 = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	playText->SetString(stringTable1->Get("PLAY"));
	playText->text.setCharacterSize(30);
	playText->text.setFillColor(sf::Color::Magenta);
	playText->SetPosition(centerPos.x, centerPos.y);
	playText->SetOrigin(Origins::MC);
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

	sf::Vector2f centerPos = FRAMEWORK.GetWindowSize() * 0.5f;

	// 타이틀 뷰
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	titleView.setSize(windowSize);
	titleView.setCenter(centerPos);

	// 플레이 텍스트
	StringTable* stringTable1 = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	playText->SetString(stringTable1->Get("PLAY"));
	playText->text.setCharacterSize(30);
	playText->text.setFillColor(sf::Color::Magenta);
	playText->SetPosition(centerPos.x, centerPos.y);
	playText->SetOrigin(Origins::MC);

	SCENE_MGR.GetCurrScene()->score = 0;
}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);

	blinkTime -= dt;
	if (blinkTime <= 0.f)
	{
		playText->SetActive(true);
		if (blinkTime <= -0.5f)
		{
			blinkTime = 0.5f;
		}
	}
	else
	{
		playText->SetActive(false);
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
	{
		SCENE_MGR.ChangeScene(SceneId::Game1);
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
