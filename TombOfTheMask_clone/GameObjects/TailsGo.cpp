#include "stdafx.h"
#include "TailsGo.h"
#include "Player.h"
#include "SceneMgr.h"

TailsGo::TailsGo(const std::string& textureId, const std::string& n)
	:SpriteGo(textureId, n)
{
}

void TailsGo::Init()
{
	sprite.setColor(sf::Color::Yellow);
	//SetPosition(player->GetPosition());
	SetOrigin(Origins::MR);
	sprite.setScale(0.f, 1.f);
}

void TailsGo::Reset()
{
	SpriteGo::Reset();

	//SetPosition(player->GetPosition());
	SetOrigin(Origins::MR);
	sprite.setScale(0.f, 1.f);
	reachMaxWidth = false;
}

void TailsGo::Update(float dt)
{
	SpriteGo::Update(dt);
	direction = player->GetDirection();

	SetPosition(player->GetPosition());
	if (player->isMoving)
	{
		if (!reachMaxWidth)
		{
			IncreaseTailWidth(dt);
		}
		else
		{
			DecreaseTailWidth(dt);
		}
		SetRotation(direction);
	}
	else
	{
		// 원래 위치에 고정
		SetPosition(position);

		DecreaseTailWidth(dt);

		//if (sprite.getScale().x <= 0.f)
		//{
			SCENE_MGR.GetCurrScene()->RemoveGo(this);
			pool->Return(this);
		//}
	}
}

void TailsGo::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void TailsGo::SetRotation(sf::Vector2f d)
{
	// 회전 초기화
	sprite.setRotation(0);

	if (d == sf::Vector2f(1.f, 0.f))
	{
		sprite.setRotation(0.f);
	}
	else if (d == sf::Vector2f(-1.f, 0.f))
	{
		sprite.setRotation(-180.f);
	}
	else if (d == sf::Vector2f(0.f, 1.f))
	{
		sprite.setRotation(90.f);
	}
	else if (d == sf::Vector2f(0.f, -1.f))
	{
		sprite.setRotation(-90.f);
	}
}

void TailsGo::IncreaseTailWidth(float dt)
{
	float x = sprite.getScale().x;
	if (x < 0.f)
	{
		x = 0.f;
	}

	x += dt*5;
	if (x < maxWidth - 0.01f)
	{
		sprite.setScale(x, 1.f);
	}
	if (x >= maxWidth - 0.01f && x <= maxWidth)
	{
		reachMaxWidth = true;
	}
}

void TailsGo::DecreaseTailWidth(float dt)
{
	float x = sprite.getScale().x;
	//if (x < 0.f)
	//{
	//	x = 0.f;
	//}

	if (player->isMoving)
	{
		x -= dt * 5;
	}
	else if(!player->isMoving)
	{
		x -= dt * 10;
	}

	if (x >= 0.8f && x <= 0.9f)
	{
		reachMaxWidth = false;
	}
	if (x <= 1.5f/* && x>=0.f*/)
	{
		sprite.setScale(x, 1.f);
	}
}
