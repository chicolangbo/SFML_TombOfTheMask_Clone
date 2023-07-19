#pragma once
#include "GameObject.h"
#include "SpriteGo.h"
#include "RectGo.h"
#include "UIButton.h"
#include "TextGo.h"
#include "AnimationController.h"

class UIGame :
	public GameObject
{
protected:
	// 상단 고정 UI
	SpriteGo scoreCoin;
	TextGo scoreText;
	std::vector<SpriteGo> starIcon;
	UIButton pauseIcon;

	// 창 UI : PAUSE, CLEAR, DIE
	RectGo uiBox;
	TextGo uiText1;
	TextGo uiText2;
	UIButton button1;
	TextGo button1Text;
	UIButton button2;
	TextGo button2Text;
	AnimationController dieAnimation;
	SpriteGo dieUiChar;

	int score = 0;
	int maxScore = 0;
	bool isPause = false;
	bool pauseWindowClose = false;
	float totalTime = 0.f;

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

	void SetPauseWindow();
	void SetClearWindow();
	void SetDieWindow();

	template <typename T>
	void Yopen(T& rec, float maxHeight = 1.f) {
		float height = rec.getScale().y;
		if (height <= maxHeight)
		{
			height += 0.01f; // 조금씩 증가시킬 값
		}
		rec.setScale(sf::Vector2f(rec.getScale().x, height));
	}

	template <typename T>
	void Yclose(T& rec) {
		float height = rec.getScale().y;
		if (height >= 0)
		{
			height -= 0.01f; // 조금씩 감소시킬 값
		}
		rec.setScale(sf::Vector2f(rec.getScale().x, height));
	}

	template <typename T>
	void Ycheck(T& rec) {
		if (rec.getScale().y <= 0)
		{
			this->pauseWindowClose = false;
		}
	}
};