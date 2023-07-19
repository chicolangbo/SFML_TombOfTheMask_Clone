#include "stdafx.h"
#include "UIGame.h"
#include "ResourceMgr.h"
#include "Framework.h"
#include "InputMgr.h"
#include "StringTable.h"
#include "DataTableMgr.h"

UIGame::UIGame(const std::string& n)
	: GameObject(n), scoreCoin("graphics/item/Bonus_Coin.png", "scoreCoin"), pauseIcon("graphics/ui/Pause_button_tap.png", "pauseIcon"), scoreText("scoreText", "fonts/GalmuriMono7.ttf"), pauseBox("pauseBox"), pauseText("pauseText", "fonts/GalmuriMono7.ttf"), enterBox("graphics/ui/Button.png", "enter"), exitBox("graphics/ui/Button.png","exit"), enterText("enterText", "fonts/GalmuriMono7.ttf"), exitText("exitText", "fonts/GalmuriMono7.ttf")
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
			starIcon[i].SetPosition(screenSize.x * 0.5f - 60.f + i*60.f, 0.f); // 위치값은 리소스 수정 후 재세팅
			starIcon[i].sprite.setColor(sf::Color::Magenta);
			starIcon[i].sprite.setScale(2.f, 2.f);
			starIcon[i].sortLayer = 100;
		}
	}

	// PAUSE SCREEN
	{
		// PAUSE BOX
		pauseBox.setSize(sf::Vector2f(screenSize.x, 200.f )); // 200
		pauseBox.setScale(sf::Vector2f(1.f, 0.f));
		pauseBox.rect.setFillColor(sf::Color::Yellow);
		pauseBox.SetOrigin(Origins::MC);
		pauseBox.SetPosition(screenSize * 0.5f);
		pauseBox.sortLayer = 100;

		// PAUSE TEXT
		StringTable* stringTable1 = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
		pauseText.SetString(stringTable1->Get("CONTINUE"));
		pauseText.text.setCharacterSize(60);
		pauseText.text.setFillColor(sf::Color::Black);
		pauseText.SetPosition(screenSize.x*0.5f,screenSize.y*0.5f);
		pauseText.sortLayer = 100;

		// YES BOX
		enterBox.SetOrigin(Origins::MC);
		enterBox.SetPosition(screenSize.x / 4 * 1, screenSize.y - 200.f);
		enterBox.sprite.setScale(2.5f, 0.f); // 2
		enterBox.sprite.setColor(sf::Color::Yellow);
		enterBox.OnEnter = [this]() {
			std::cout << "Enter" << std::endl;
			enterBox.sprite.setColor(sf::Color::Magenta);
		};
		enterBox.OnClick = [this]() {
			std::cout << "Click" << std::endl;
			isPause = false;
			pauseScreenClose = true;
		};
		enterBox.OnExit = [this]() {
			std::cout << "Exit" << std::endl;
			enterBox.sprite.setColor(sf::Color::Yellow);
		};
		enterBox.sortLayer = 100;

		// YES TEXT
		StringTable* stringTable2 = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
		enterText.SetString(stringTable2->Get("ENTER"));
		enterText.text.setCharacterSize(20);
		enterText.text.setFillColor(sf::Color::Black);
		enterText.SetPosition(enterBox.GetPosition());
		enterText.sortLayer = 100;

		// GIVE UP BOX
		exitBox.SetOrigin(Origins::MC);
		exitBox.SetPosition(screenSize.x / 4 * 3, screenSize.y - 200.f);
		exitBox.sprite.setScale(2.5f, 0.f); // 2
		exitBox.sprite.setColor(sf::Color::Yellow);
		exitBox.OnEnter = [this]() {
			std::cout << "Enter" << std::endl;
			exitBox.sprite.setColor(sf::Color::Magenta);
		};
		exitBox.OnClick = [this]() {
			std::cout << "Click" << std::endl;
			isPause = false;
			pauseScreenClose = true;
		};
		exitBox.OnExit = [this]() {
			std::cout << "Exit" << std::endl;
			exitBox.sprite.setColor(sf::Color::Yellow);
		};
		exitBox.sortLayer = 100;

		// GIVE UP TEXT
		StringTable* stringTable3 = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
		exitText.SetString(stringTable3->Get("GIVE_UP"));
		exitText.text.setCharacterSize(20);
		exitText.text.setFillColor(sf::Color::Black);
		exitText.SetPosition(exitBox.GetPosition());
		exitText.sortLayer = 100;
	}
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
	pauseBox.Release();
	pauseText.Release();
	enterBox.Release();
	enterText.Release();
	exitBox.Release();
	exitText.Release();
}

void UIGame::Reset()
{
	// INGAME UI
	scoreCoin.Reset();
	scoreText.Reset();
	pauseIcon.Reset();
	for (int i = 0; i < 3; ++i)
	{
		starIcon[i].Reset();
	}

	// PAUSE UI
	pauseBox.Reset();
	pauseBox.SetOrigin(Origins::MC);
	pauseBox.setScale(sf::Vector2f(1.f, 0.f));

	pauseText.Reset();
	pauseText.SetOrigin(Origins::MC);
	pauseText.text.setScale(sf::Vector2f(1.f, 0.f));

	enterBox.Reset();
	enterBox.SetOrigin(Origins::MC);
	enterBox.sprite.setScale(sf::Vector2f(2.5f, 0.f));
	enterBox.sprite.setColor(sf::Color::Yellow);

	enterText.Reset();
	enterText.SetOrigin(Origins::MC);
	enterText.text.setScale(sf::Vector2f(1.f, 0.f));

	exitBox.Reset();
	exitBox.SetOrigin(Origins::MC);
	exitBox.sprite.setScale(sf::Vector2f(2.5f, 0.f));
	exitBox.sprite.setColor(sf::Color::Yellow);

	exitText.Reset();
	exitText.SetOrigin(Origins::MC);
	exitText.text.setScale(sf::Vector2f(1.f, 0.f));
}

void UIGame::Update(float dt)
{
	StarIconUpdate();
	ScoreTextUpdate();

	pauseIcon.Update(dt);
	enterBox.Update(dt);
	exitBox.Update(dt);

	if (isPause)
	{
		Yopen(pauseBox);
		Yopen(enterBox.sprite, 2.f);
		Yopen(exitBox.sprite, 2.f);
		Yopen(enterText.text);
		Yopen(exitText.text);
		Yopen(pauseText.text);

		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
		{
			isPause = false;
			pauseScreenClose = true;
		}

		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
		{
			// 타이틀로 이동하는 코드
		}
	}
	else if (pauseScreenClose)
	{
		Yclose(pauseBox);
		Yclose(enterBox.sprite);
		Yclose(exitBox.sprite);
		Yclose(enterText.text);
		Yclose(exitText.text);
		Yclose(pauseText.text);

		Ycheck(enterBox.sprite);
	}
	else
	{
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
		{
			isPause = true;
			Reset();
			return;
		}

		Reset();
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
	if (isPause || (!isPause && pauseScreenClose))
	{
		pauseBox.Draw(window);
		pauseText.Draw(window);
		enterBox.Draw(window);
		enterText.Draw(window);
		exitBox.Draw(window);
		exitText.Draw(window);
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