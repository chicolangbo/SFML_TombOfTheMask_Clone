#include "stdafx.h"
#include "LogoGo.h"
#include "ResourceMgr.h"
#include "Framework.h"

void LogoGo::Init()
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Logo.csv"));

	animation.SetTarget(&sprite);
	SetOrigin(Origins::MC);
}

void LogoGo::Reset()
{
	sf::Vector2f centerPos = FRAMEWORK.GetWindowSize() * 0.5f;

	animation.Play("Logo");
	SetOrigin(origin);
	SetPosition(centerPos.x, centerPos.y - 100.f);
	sprite.setColor(sf::Color::Yellow);
	sprite.setScale(3.f, 3.f);
}

void LogoGo::Update(float dt)
{
	animation.Update(dt);
}

void LogoGo::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
