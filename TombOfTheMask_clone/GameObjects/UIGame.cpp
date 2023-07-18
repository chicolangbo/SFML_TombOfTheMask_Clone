#include "stdafx.h"
#include "UIGame.h"
#include "ResourceMgr.h"
#include "Framework.h"
#include "InputMgr.h"
#include "StringTable.h"
#include "DataTableMgr.h"

UIGame::UIGame(const std::string& n)
	: GameObject(n), scoreCoin("graphics/item/Bonus_Coin.png", "scoreCoin"), pauseIcon("graphics/ui/Pause_button_tap.png", "pauseIcon"), scoreText("scoreText", "fonts/GalmuriMono7.ttf")
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
			pause = true;
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
}

void UIGame::Release()
{
	scoreCoin.Release();
	scoreText.Release();
	pauseIcon.Release();
	for (int i = 0; i < 3; ++i)
	{
		starIcon[i].Release();
	}
}

void UIGame::Reset()
{
	scoreCoin.Reset();
	scoreText.Reset();
	pauseIcon.Reset();
	for (int i = 0; i < 3; ++i)
	{
		starIcon[i].Reset();
	}
}

void UIGame::Update(float dt)
{
	StarIconUpdate();
	ScoreTextUpdate();
	pauseIcon.Update(dt);

	if (pause && INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
	{

	}
}

void UIGame::Draw(sf::RenderWindow& window)
{
	scoreCoin.Draw(window);
	scoreText.Draw(window);
	pauseIcon.Draw(window);
	for (int i = 0; i<starIcon.size(); ++i)
	{
		starIcon[i].Draw(window);
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
	return pause;
}

void UIGame::SetScore(int s)
{
	score = s;
}

void UIGame::SetMaxScore(int s)
{
	maxScore = s;
}
