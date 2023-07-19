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
	}
}

UIGame::~UIGame()
{
}

void UIGame::Init()
{
	dieAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/DieUI.csv"));
	dieAnimation.SetTarget(&dieUiChar.sprite);
	dieUiChar.SetOrigin(Origins::MC);

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
		scoreCoin.sortLayer = 100;

		// SCORE TEXT
		std::stringstream ss;
		ss << this->score;
		scoreText.text.setString(ss.str());
		scoreText.text.setCharacterSize(30);
		scoreText.text.setFillColor(sf::Color::Yellow);
		scoreText.SetOrigin(Origins::TL);
		scoreText.SetPosition(scoreCoin.GetPosition().x + 50.f ,10.f);
		scoreText.sortLayer = 100;
	}

	// STAR ICON
	{
		for (int i = 0; i < 3; ++i)
		{
			starIcon[i].SetOrigin(Origins::TC);
			starIcon[i].SetPosition(screenSize.x * 0.5f - 60.f + i*60.f, 0.f); // ��ġ���� ���ҽ� ���� �� �缼��
			starIcon[i].sprite.setColor(sf::Color::Magenta);
			starIcon[i].sprite.setScale(2.f, 2.f);
			starIcon[i].sortLayer = 100;
		}
	}

	// UI WINDOW
	SetPauseWindow();
	SetDieWindow();
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
	button1.Release();
	button1Text.Release();
	button2.Release();
	button2Text.Release();
}

void UIGame::Reset()
{
	sf::Vector2f screenSize = FRAMEWORK.GetWindowSize();

	// INGAME UI
	scoreCoin.Reset();
	scoreText.Reset();
	pauseIcon.Reset();
	for (int i = 0; i < 3; ++i)
	{
		starIcon[i].Reset();
	}

	// PAUSE UI
	uiBox.Reset();
	uiBox.SetOrigin(Origins::MC);
	uiBox.setScale(sf::Vector2f(1.f, 0.f));

	uiText1.Reset();
	uiText1.SetOrigin(Origins::MC);
	uiText1.text.setScale(sf::Vector2f(1.f, 0.f));

	uiText2.Reset();
	uiText2.SetOrigin(Origins::MC);
	uiText2.text.setScale(sf::Vector2f(1.f, 0.f));

	button1.Reset();
	button1.SetOrigin(Origins::MC);
	button1.sprite.setScale(sf::Vector2f(2.5f, 0.f));
	button1.sprite.setColor(sf::Color::Yellow);

	button1Text.Reset();
	button1Text.SetOrigin(Origins::MC);
	button1Text.text.setScale(sf::Vector2f(1.f, 0.f));

	button2.Reset();
	button2.SetOrigin(Origins::MC);
	button2.sprite.setScale(sf::Vector2f(2.5f, 0.f));
	button2.sprite.setColor(sf::Color::Yellow);

	button2Text.Reset();
	button2Text.SetOrigin(Origins::MC);
	button2Text.text.setScale(sf::Vector2f(1.f, 0.f));

	dieAnimation.Play("DieUI");
	dieUiChar.sprite.setScale(2.f, 2.f);
	dieUiChar.sprite.setColor(sf::Color::White);
	dieUiChar.SetOrigin(Origins::MC);
	dieUiChar.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.5f - 100.f);
}

void UIGame::Update(float dt)
{
	StarIconUpdate();
	ScoreTextUpdate();

	button1.Update(dt);
	button2.Update(dt);
	pauseIcon.Update(dt);
	dieUiChar.SetOrigin(Origins::MC);
	dieAnimation.Update(dt);

	if (isPause)
	{
		Yopen(uiBox);
		Yopen(button1.sprite, 2.f);
		Yopen(button2.sprite, 2.f);
		Yopen(button1Text.text);
		Yopen(button2Text.text);
		Yopen(uiText1.text);
		Yopen(uiText2.text);
		Yopen(dieUiChar.sprite);

		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
		{
			isPause = false;
			pauseWindowClose = true;
		}

		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
		{
			// Ÿ��Ʋ�� �̵��ϴ� �ڵ�
		}
	}
	else if (pauseWindowClose)
	{
		Yclose(uiBox);
		Yclose(button1.sprite);
		Yclose(button2.sprite);
		Yclose(button1Text.text);
		Yclose(button2Text.text);
		Yclose(uiText1.text);
		Yclose(uiText2.text);
		Yclose(dieUiChar.sprite);

		Ycheck(button1.sprite);
	}
	else
	{
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
		{
			isPause = true;
			Reset();
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

	// PAUSE UI
	if (isPause || (!isPause && pauseWindowClose))
	{
		uiBox.Draw(window);
		uiText1.Draw(window);
		uiText2.Draw(window);
		button1.Draw(window);
		button1Text.Draw(window);
		button2.Draw(window);
		button2Text.Draw(window);
		dieUiChar.Draw(window);
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
	uiBox.sortLayer = 100;

	// PAUSE TEXT
	StringTable* stringTable1 = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	uiText1.SetString(stringTable1->Get("CONTINUE"));
	uiText1.text.setCharacterSize(60);
	uiText1.text.setFillColor(sf::Color::Black);
	uiText1.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.5f);
	uiText1.sortLayer = 100;

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
		isPause = false;
		pauseWindowClose = true;
	};
	button1.OnExit = [this]() {
		std::cout << "Exit" << std::endl;
		button1.sprite.setColor(sf::Color::Yellow);
	};
	button1.sortLayer = 100;

	// YES TEXT
	StringTable* stringTable2 = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	button1Text.SetString(stringTable2->Get("ENTER"));
	button1Text.text.setCharacterSize(20);
	button1Text.text.setFillColor(sf::Color::Black);
	button1Text.SetPosition(button1.GetPosition());
	button1Text.sortLayer = 100;

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
		isPause = false;
		pauseWindowClose = true;
	};
	button2.OnExit = [this]() {
		std::cout << "Exit" << std::endl;
		button2.sprite.setColor(sf::Color::Yellow);
	};
	button2.sortLayer = 100;

	// GIVE UP TEXT
	StringTable* stringTable3 = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	button2Text.SetString(stringTable3->Get("GIVE_UP"));
	button2Text.text.setCharacterSize(20);
	button2Text.text.setFillColor(sf::Color::Black);
	button2Text.SetPosition(button2.GetPosition());
	button2Text.sortLayer = 100;
}

void UIGame::SetClearWindow()
{
	sf::Vector2f screenSize = FRAMEWORK.GetWindowSize();

	// PAUSE BOX
	uiBox.setSize(sf::Vector2f(screenSize.x, 200.f)); // 200
	uiBox.setScale(sf::Vector2f(1.f, 0.f));
	uiBox.rect.setFillColor(sf::Color::Yellow);
	uiBox.SetOrigin(Origins::MC);
	uiBox.SetPosition(screenSize * 0.5f);
	uiBox.sortLayer = 100;

	// PAUSE TEXT
	StringTable* stringTable1 = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	uiText1.SetString(stringTable1->Get("CONTINUE"));
	uiText1.text.setCharacterSize(60);
	uiText1.text.setFillColor(sf::Color::Black);
	uiText1.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.5f);
	uiText1.sortLayer = 100;

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
		isPause = false;
		pauseWindowClose = true;
	};
	button1.OnExit = [this]() {
		std::cout << "Exit" << std::endl;
		button1.sprite.setColor(sf::Color::Yellow);
	};
	button1.sortLayer = 100;

	// YES TEXT
	StringTable* stringTable2 = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	button1Text.SetString(stringTable2->Get("ENTER"));
	button1Text.text.setCharacterSize(20);
	button1Text.text.setFillColor(sf::Color::Black);
	button1Text.SetPosition(button1.GetPosition());
	button1Text.sortLayer = 100;

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
		isPause = false;
		pauseWindowClose = true;
	};
	button2.OnExit = [this]() {
		std::cout << "Exit" << std::endl;
		button2.sprite.setColor(sf::Color::Yellow);
	};
	button2.sortLayer = 100;

	// GIVE UP TEXT
	StringTable* stringTable3 = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	button2Text.SetString(stringTable3->Get("GIVE_UP"));
	button2Text.text.setCharacterSize(20);
	button2Text.text.setFillColor(sf::Color::Black);
	button2Text.SetPosition(button2.GetPosition());
	button2Text.sortLayer = 100;
}

void UIGame::SetDieWindow()
{
	sf::Vector2f screenSize = FRAMEWORK.GetWindowSize();

	// DIE BOX
	uiBox.setSize(sf::Vector2f(screenSize.x, 300.f)); // ����.y + 100
	uiBox.setScale(sf::Vector2f(1.f, 0.f));
	uiBox.rect.setFillColor(sf::Color::Yellow);
	uiBox.SetOrigin(Origins::MC);
	uiBox.SetPosition(screenSize.x * 0.5f , screenSize.y * 0.5f - 50.f);
	uiBox.sortLayer = 100;

	// DIE TEXT
	StringTable* stringTable1 = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	uiText1.SetString(stringTable1->Get("DIE"));
	uiText1.text.setCharacterSize(50);
	uiText1.text.setFillColor(sf::Color::Black);
	uiText1.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.5f - 20.f);
	uiText1.sortLayer = 100;
	uiText2.SetString(stringTable1->Get("REPLAY"));
	uiText2.text.setCharacterSize(50);
	uiText2.text.setFillColor(sf::Color::Black);
	uiText2.SetPosition(screenSize.x * 0.5f, screenSize.y * 0.5f + 40.f);
	uiText2.sortLayer = 100;
}
