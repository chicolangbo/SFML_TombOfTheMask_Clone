#pragma once
#include "Scene.h"
#include "AnimationController.h"
#include "LogoGo.h"
#include "TextGo.h"

class SceneTitle :
    public Scene
{
protected:
	sf::View titleView;
	LogoGo* logo = nullptr;
	TextGo* playText = nullptr;
	sf::Sound sound;

	bool once = true;
	float blinkTime = 0.5f;

public:
	SceneTitle(SceneId id = SceneId::Title);
	virtual ~SceneTitle() override;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

