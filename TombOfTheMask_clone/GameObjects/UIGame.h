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
	// ��� ���� UI
	SpriteGo scoreCoin;
	TextGo scoreText;
	std::vector<SpriteGo> starIcon;
	UIButton pauseIcon;

	// â UI : PAUSE, CLEAR, DIE
	RectGo uiBox;
	TextGo uiText1;
	TextGo uiText2;
	UIButton button1;
	TextGo button1Text;
	UIButton button2;
	TextGo button2Text;

	// UI �ִϸ��̼�
	AnimationController dieAnimation;
	SpriteGo dieUiChar;
	std::vector<AnimationController> starGetAnimation;
	std::vector<SpriteGo> starGet;
	std::vector<SpriteGo> starEmpty;

	int score = 0;
	int maxScore = 0;
	bool pauseWindowClose = false;

public:
	bool replay = false;
	bool isPause = false;
	bool pauseWindow = false;
	bool dieWindow = false;
	bool winWindow = false;

	UIGame(const std::string& n = "");
	virtual ~UIGame() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void StarIconUpdate();
	void ScoreTextUpdate();
	void Yupdate(bool open);

	bool GetPause();
	void SetScore(int s);
	void SetMaxScore(int s);

	void SetPauseWindow();
	void SetWinWindow();
	void SetDieWindow();

	template <typename T>
	void Yopen(T& rec, float maxHeight = 1.f) {
		float height = rec.getScale().y;
		if (height <= maxHeight)
		{
			height += 0.01f; // ���ݾ� ������ų ��
		}
		rec.setScale(sf::Vector2f(rec.getScale().x, height));
	}

	template <typename T>
	void Yclose(T& rec) {
		float height = rec.getScale().y;
		if (height >= 0)
		{
			height -= 0.01f; // ���ݾ� ���ҽ�ų ��
		}
		rec.setScale(sf::Vector2f(rec.getScale().x, height));
	}

	template <typename T>
	void Ycheck(T& rec) {
		if (rec.getScale().y <= 0)
		{
			this->pauseWindowClose = false;
			this->isPause = false;
		}
	}
};