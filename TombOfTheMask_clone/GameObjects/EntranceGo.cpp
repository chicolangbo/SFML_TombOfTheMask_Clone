#include "stdafx.h"
#include "EntranceGo.h"
#include "ResourceMgr.h"
#include "Framework.h"

void EntranceGo::Init()
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/EnterDoor.csv"));

	animation.SetTarget(&sprite);
	SetOrigin(Origins::BC);
	sprite.setScale(2.f, 2.f);
}

void EntranceGo::Reset()
{
	animation.Play("EnterDoor");
	SetOrigin(origin);
	totalTime = 0.f;
}

void EntranceGo::Update(float dt)
{
	animation.Update(dt);

	totalTime += dt;
	if (totalTime <= 2.0f)
	{
		sprite.setColor(sf::Color(255, 255, 255, 255.f - totalTime * 70.f));
	}
}

void EntranceGo::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
