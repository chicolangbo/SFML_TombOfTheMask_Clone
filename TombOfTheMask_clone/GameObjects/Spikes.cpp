#include "stdafx.h"
#include "Spikes.h"
#include "ResourceMgr.h"
#include "Framework.h"

void Spikes::Init()
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Spikes.csv"));

	animation.SetTarget(&sprite);
	animation.SetSpeed(0.3f);
	SetOrigin(Origins::ML);
}

void Spikes::Reset()
{
	animation.Play("Spikes");
	animation.SetSpeed(0.f);
	SetOrigin(origin);
	/*SetPosition(100.f, 100.f);*/
}

void Spikes::Update(float dt)
{
	if (collide)
	{
		animation.SetSpeed(0.3f);
	}
	animation.Update(dt);
}

void Spikes::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}