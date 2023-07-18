#pragma once
#include "GameObject.h"
#include "SpriteGo.h"
#include "RectGo.h"
#include "UIButton.h"
#include "TextGo.h"

class UIGame :
    public GameObject
{
protected:
	SpriteGo scoreCoin;
	TextGo scoreText;
	std::vector<SpriteGo> starIcon;
	UIButton pauseIcon;

	RectGo pauseBox;
	TextGo pauseText;

	int score = 0;
	int maxScore = 0;

	bool pause = false;

public:
	UIGame(const std::string& n = "");
	virtual ~UIGame() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	
	void StarIconUpdate();
	void ScoreTextUpdate();

	bool GetPause();
	void SetScore(int s);
	void SetMaxScore(int s);
};

