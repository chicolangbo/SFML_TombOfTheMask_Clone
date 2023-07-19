#include "stdafx.h"
#include "UIGame.h"
#include "ResourceMgr.h"
#include "Framework.h"
#include "InputMgr.h"
#include "StringTable.h"
#include "DataTableMgr.h"

UIGame::UIGame(const std::string& n)
	: GameObject(n), scoreCoin("graphics/item/Bonus_Coin.png", "scoreCoin"), pauseIcon("graphics/ui/Pause_button_tap.png", "pauseIcon"), scoreText("scoreText", "fonts/GalmuriMono7.ttf"), uiBox("uiBox"), uiText1("uiText1", "fonts/GalmuriMono7.ttf"), uiText2("uiText2", "fonts/GalmuriMono7.ttf"), button1("graphics/ui/Button.png", "enter"), button2("graphics/ui/Button.png","exit"), button1Text("button1Text", "fonts/GalmuriMono7.ttf"), button2Text("button2Text", "fonts/GalmuriMono7.ttf"), dieUiChar("","dieUiChar")
{
	for (int i = 0; i < 3; ++i)
	{
		std::string num = std::to_string(i + 1);

		SpriteGo tempStar = SpriteGo("graphics/ui/Star_big_2.png", "star" + num);
		starIcon.push_back(tempStar);

		SpriteGo tempStarGet = SpriteGo("", "starGet" + num);
		starGet.push_back(tempStarGet);

		SpriteGo tempStarEmpty = SpriteGo("graphics/ui/Star_big_1.png", "starEmpty" + num);
		starEmpty.push_back(tempStarEmpty);
		
		AnimationController tempStarAnimation;
		starGetAnimation.push_back(tempStarAnimation);
	}
}

UIGame::~UIGame()
{
}

void UIGame::Init()
{
	dieAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/DieUI.csv"));
	dieAnimation.SetTarget(&dieUiChar.sprite);

	for (int i = 0; i < starGet.size(); ++i)
	{
		starGetAnimation[i].AddClip(*RESOURCE_MGR.GetAnimationClip("animations/ScoreStar.csv"));
		starGetAnimation[i].SetTarget(&starGet[i].sprite);
	}

	sf::Vector2f screenSize = FRAMEWORK.GetWindowSize();

	// PAUSE BUTTON
	{
		pauseIcon.SetOrigin(Origins::TR);
		pauseIcon.SetPosition(screenSize.x, 0.f);
		pauseIcon.sprite.setScale(3.f, 3.f);
		pauseIcon.sprite.setColor(sf::Color::Yellow);
		pauseIcon.OnEnter = [this]() {
			std::cout << "Enter" << std::endl;
			pauseIcon.sprite.setColor(sf::Color::Magenta);
		};
		pauseIcon.OnClick = [this]() {
			std::cout << "Click" << std::endl;
			isPause = true;
		};
		pauseIcon.OnExit = [this]() {
			std::cout << "Exit" << std::endl;
			pauseIcon.sprite.setColor(sf::Color::Yellow);
		};
		pauseIcon.sortLayer = 100;
	}

	// COIN SCORE
	{
		// COIN IMAGE
		scoreCoin.SetOrigin(Origins::TL);
		scoreCoin.SetPosition(0.f, 0.f);
		scoreCoin.sprite.setColor(sf::Color::Yellow);
		scoreCoin.sprite.setScale(2.5f, 2.5f);

		// SCORE TEXT
		std::stringstream ss;
		ss << this->score;
		scoreText.text.setString(ss.str());
		scoreText.text.setCharacterSize(30);
		scoreText.text.setFillColor(sf::Color::Yellow);
		scoreText.SetOrigin(Origins::TL);
		scoreText.SetPosition(scoreCoin.GetPosition().x + 50.f ,10.f);
	}

	// STAR ICON
	{
		for (int i = 0; i < 3; ++i)
		{
			starIcon[i].SetOrigin(Origins::TC);
			starIcon[i].SetPosition(screenSize.x * 0.5f - 60.f + i*60.f, 0.f);
			starIcon[i].sprite.setColor(sf::Color::Magenta);
			starIcon[i].sprite.setScale(2.f, 2.f);
		}
	}

	SetPauseWindow();
}

void UIGame::Release()
{
	// INGAME UI
	scoreCoin.Release();
	scoreText.Release();
	pauseIcon.Release();
	for (int i = 0; i < 3; ++i)
	{
		starIcon[i].Release();
	}

	// PAUSE UI
	uiBox.Release();
	uiText1.Release();
	uiText2.Release();
	button1.Release();
	button1Text.Release();
	button2.Release();
	button2Text.Release();
	dieUiChar.Release();
	for (int i = 0; i < starGet.size(); ++i)
	{
		starGet[i].Release();
		starEmpty[i].Release();
	}
}

void UIGame::Reset()
{
	//pauseWindow = false;
	//dieWindow = false;
	//winWindow = false;

	scoreCoin.Reset();
	scoreText.Reset();
	pauseIcon.Reset();
	for (int i = 0; i < starIcon.size(); ++i)
	{
		starIcon[i].Reset();
		starEmpty[i].Reset();
	}
	uiBox.Reset();
	uiText1.Reset();
	uiText2.Reset();
	button1.Reset();
	button1Text.Reset();
	button2.Reset();
	button2Text.Reset();

	SetPauseWindow();

	if (dieWindow)
	{
		SetDieWindow();
	}
	else if (winWindow)
	{
		SetWinWindow();
	}
}

void UIGame::Update(float dt)
{
	// 상단 고정 UI
	StarIconUpdate();
	ScoreTextUpdate();

	// 퍼즈 화면 UI
	button1.Update(dt);
	button2.Update(dt);
	pauseIcon.Update(dt);
	dieUiChar.SetOrigin(Origins::MC);
	dieAnimation.Update(dt);
	for (int i = 0; i<starGetAnimation.size(); ++i)
	{
		starGet[i].SetOrigin(Origins::MC);
		starGetAnimation[i].Update(dt);
	}
	bool wait = false;

	// 멈췄을 때
	if (isPause)
	{
		wait = true;
		Yupdate(pauseWindowClose);

		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
		{
			pauseWindowClose = true;
			wait = false;
		}

		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
		{
			// 타이틀로 이동하는 코드
			wait = false;
		}

		if (pauseWindowClose && !wait)
		{
			Yupdate(pauseWindowClose);
		}
	}
	else if(!isPause)
	{
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
		{
			pauseWindow = true;
			Reset();
			isPause = true;
		}
		else if (dieWindow)
		{
			Reset();
			isPause = true;

		}
		else if (winWindow)
		{
			Reset();
			isPause = true;
		}
	}
}

void UIGame::Draw(sf::RenderWindow& window)
{
	// INGAME UI
	scoreCoin.Draw(window);
	scoreText.Draw(window);
	pauseIcon.Draw(window);
	for (int i = 0; i<starIcon.size(); ++i)
	{
		starIcon[i].Draw(window);
	}

	// UI WINDOW
	if (isPause)
	{
		uiBox.Draw(window);
		uiText1.Draw(window);
		uiText2.Draw(window);
		button1.Draw(window);
		button1Text.Draw(window);
		button2.Draw(window);
		button2Text.Draw(window);
		dieUiChar.Draw(window);
		for (int i = 0; i < starGet.size(); ++i)
		{
			starEmpty[i].Draw(window);
			starGet[i].Draw(window);
		}
	}
}

void UIGame::StarIconUpdate()
{
	for (int i = 0; i < starIcon.size(); ++i)
	{
		if (starIcon[i].sprite.getColor() == sf::Color::Yellow)
			continue;

		if (score == maxScore)
		{
			starIcon[i].sprite.setColor(sf::Color::Yellow);
		}
		else if (score >= maxScore / 3 * 2 && score < maxScore)
		{
			if (i < 2)
			{
				starIcon[i].sprite.setColor(sf::Color::Yellow);
			}
			break;
		}
		else if (score < maxScore / 3 * 2 && score > maxScore / 3)
		{
			if (i < 1)
			{
				starIcon[i].sprite.setColor(sf::Color::Yellow);
			}
			break;
		}
	}
}

void UIGame::ScoreTextUpdate()
{
	std::stringstream ss;
	ss << this->score;
	scoreText.text.setString(ss.str());
}

void UIGame::Yupdate(bool open)
{
	if (!open)
	{
		Yopen(uiBox);
		Yopen(button1.sprite, 2.f);
		Yopen(button2.sprite, 2.f);
		Yopen(button1Text.text);
		Yopen(button2Text.text);
		Yopen(uiText1.text);
		Yopen(uiText2.text);
		Yopen(dieUiChar.sprite);
		for (int i = 0; i < starGet.size(); ++i)
		{
			Yopen(starGet[i].sprite);
			Yopen(starEmpty[i].sprite);
		}
	}
	else
	{
		Yclose(uiBox);
		Yclose(button1.sprite);
		Yclose(button2.sprite);
		Yclose(button1Text.text);
		Yclose(button2Text.text);
		Yclose(uiText1.text);
		Yclose(uiText2.text);
		Yclose(dieUiChar.sprite);
		for (int i = 0; i < starGet.size(); ++i)
		{
			Yclose(starGet[i].sprite);
			Yclose(starEmpty[i].sprite);
		}
		Ycheck(button1.sprite);
	}
}

bool UIGame::GetPause()
{
	return isPause;
}

void UIGame::SetScore(int s)
{
	score = s;
}

void UIGame::SetMaxScore(int s)
{
	maxScore = s;
}

void UIGame::SetPauseWindow()
{
	sf::Vector2f screenSize = FRAMEWORK.GetWindowSize();

	// PAUSE BOX
	uiBox.setSize(sf::Vector2f(screenSize.x, 200.f)); // 200
	uiBox.setScale(sf::Vector2f(1.f, 0.f));
	uiBox.rect.setFillColor(sf::Color::Yellow);
	uiBox.SetOrigin(Origins::MC);
	uiBox.SetPosition(screenSize * 0.5f);

	// PAUSE TEXT
	StringTable* stringTable1 = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	uiText1.SetString(stringTable1->Get("CONTINUE"));
	uiText1.text.setCharacterSize(60);
	uiText1.text.setFillColor(sf::Color::Black);
	uiText1.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.5f);
	uiText1.SetOrigin(Origins::MC);

	// YES BOX
	button1.SetOrigin(Origins::MC);
	button1.SetPosition(screenSize.x / 4 * 1, screenSize.y - 200.f);
	button1.sprite.setScale(2.5f, 0.f); // 2
	button1.sprite.setColor(sf::Color::Yellow);
	button1.OnEnter = [this]() {
		std::cout << "Enter" << std::endl;
		button1.sprite.setColor(sf::Color::Magenta);
	};
	button1.OnClick = [this]() {
		std::cout << "Click" << std::endl;
		pauseWindowClose = true;
	};
	button1.OnExit = [this]() {
		std::cout << "Exit" << std::endl;
		button1.sprite.setColor(sf::Color::Yellow);
	};

	// YES TEXT
	StringTable* stringTable2 = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	button1Text.SetString(stringTable2->Get("ENTER"));
	button1Text.text.setCharacterSize(20);
	button1Text.text.setFillColor(sf::Color::Black);
	button1Text.SetPosition(button1.GetPosition());
	button1Text.SetOrigin(Origins::MC);


	// GIVE UP BOX
	button2.SetOrigin(Origins::MC);
	button2.SetPosition(screenSize.x / 4 * 3, screenSize.y - 200.f);
	button2.sprite.setScale(2.5f, 0.f); // 2
	button2.sprite.setColor(sf::Color::Yellow);
	button2.OnEnter = [this]() {
		std::cout << "Enter" << std::endl;
		button2.sprite.setColor(sf::Color::Magenta);
	};
	button2.OnClick = [this]() {
		std::cout << "Click" << std::endl;
		pauseWindowClose = true;
	};
	button2.OnExit = [this]() {
		std::cout << "Exit" << std::endl;
		button2.sprite.setColor(sf::Color::Yellow);
	};

	// GIVE UP TEXT
	StringTable* stringTable3 = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	button2Text.SetString(stringTable3->Get("GIVE_UP"));
	button2Text.text.setCharacterSize(20);
	button2Text.text.setFillColor(sf::Color::Black);
	button2Text.SetPosition(button2.GetPosition());
	button2Text.SetOrigin(Origins::MC);
}

void UIGame::SetWinWindow()
{
	sf::Vector2f screenSize = FRAMEWORK.GetWindowSize();

	// CLEAR BOX
	uiBox.setSize(sf::Vector2f(screenSize.x, 300.f)); // 200
	uiBox.setScale(sf::Vector2f(1.f, 0.f));
	uiBox.rect.setFillColor(sf::Color::Yellow);
	uiBox.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.5f - 50.f);
	uiBox.SetOrigin(Origins::MC);

	// CLEAR TEXT
	StringTable* stringTable1 = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	uiText1.SetString(stringTable1->Get("CLEAR"));
	uiText1.text.setCharacterSize(50);
	uiText1.text.setFillColor(sf::Color::Black);
	uiText1.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.5f - 20.f);
	uiText1.SetOrigin(Origins::MC);
	uiText2.SetString(stringTable1->Get("NEXT"));
	uiText2.text.setCharacterSize(50);
	uiText2.text.setFillColor(sf::Color::Black);
	uiText2.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.5f + 40.f);
	uiText2.SetOrigin(Origins::MC);

	// STAR EMPTY
	for (int i = 0; i < starEmpty.size(); ++i)
	{
		starGetAnimation[i].Play("ScoreStar");
		starGetAnimation[i].SetTarget(&starGet[i].sprite);
		starGet[i].sprite.setScale(2.f, 2.f);
		starGet[i].sprite.setColor(sf::Color::Cyan);
		starGet[i].SetOrigin(Origins::MC);
		starGet[i].SetPosition((screenSize.x * 0.5f - 80.f) + i * 80.f, screenSize.y * 0.5f - 100.f);

		starEmpty[i].sprite.setScale(2.f, 2.f);
		starEmpty[i].SetOrigin(Origins::MC);
		starEmpty[i].SetPosition((screenSize.x * 0.5f - 80.f) + i * 80.f, screenSize.y * 0.5f - 105.f);
	}

	winWindow = true;
}

void UIGame::SetDieWindow()
{
	sf::Vector2f screenSize = FRAMEWORK.GetWindowSize();

	// DIE BOX
	uiBox.setSize(sf::Vector2f(screenSize.x, 300.f)); // 중점.y + 100
	uiBox.setScale(sf::Vector2f(1.f, 0.f));
	uiBox.rect.setFillColor(sf::Color::Yellow);
	uiBox.SetPosition(screenSize.x * 0.5f , screenSize.y * 0.5f - 50.f);
	uiBox.SetOrigin(Origins::MC);

	// DIE TEXT
	StringTable* stringTable1 = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	uiText1.SetString(stringTable1->Get("DIE"));
	uiText1.text.setCharacterSize(50);
	uiText1.text.setFillColor(sf::Color::Black);
	uiText1.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.5f - 20.f);
	uiText1.SetOrigin(Origins::MC);
	uiText2.SetString(stringTable1->Get("REPLAY"));
	uiText2.text.setCharacterSize(50);
	uiText2.text.setFillColor(sf::Color::Black);
	uiText2.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.5f + 40.f);
	uiText2.SetOrigin(Origins::MC);

	dieWindow = true;
}