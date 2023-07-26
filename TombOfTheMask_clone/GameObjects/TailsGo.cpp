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

void TailsGo::Release()
{
	SpriteGo::Release();
}

void TailsGo::Reset()
{
	SpriteGo::Reset();

	SetOrigin(Origins::MR);
	sprite.setScale(0.f, 1.f);
	reachMaxWidth = false;
	maxWidth = 1.5f;
	directionChange = false;
	stop = false;
	count = 0;
}

void TailsGo::Update(float dt)
{
	/*std::cout << "꼬리　업뎃" << std::endl;*/
	SpriteGo::Update(dt);

	
	if (player->isMoving && !stop)
	{
		SetPosition(player->GetPosition());
		directionChange = true;
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
	else if(!player->isMoving || stop)
	{
		stop = true;
		startPos = player->GetStartPos();
		// 원래 위치에 고정
		SetPosition(startPos);

		if (player != nullptr && DecreaseTailWidth(dt))
		{
			count++;
			std::cout << "리턴" <<count<< std::endl;
			SCENE_MGR.GetCurrScene()->RemoveGo(this);
			pool->Return(this);
		}
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

bool TailsGo::DecreaseTailWidth(float dt)
{
	float x = sprite.getScale().x;
	if (x < 0.f)
	{
		return true;
	}

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
	if (x <= 1.5f)
	{
		sprite.setScale(x, 1.f);
	}
	return false;
}
