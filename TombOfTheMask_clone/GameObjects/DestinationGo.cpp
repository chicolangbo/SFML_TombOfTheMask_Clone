#include "stdafx.h"
#include "DestinationGo.h"
#include "ResourceMgr.h"
#include "Framework.h"

void DestinationGo::Init()
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Exit.csv"));

	animation.SetTarget(&sprite);
	SetOrigin(Origins::MC);
	sprite.setColor(sf::Color::Yellow);
}

void DestinationGo::Reset()
{
	animation.Play("Exit");
	SetOrigin(origin);
}

void DestinationGo::Update(float dt)
{
	animation.Update(dt);
}

void DestinationGo::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
