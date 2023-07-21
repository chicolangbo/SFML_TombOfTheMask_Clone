#include "stdafx.h"
#include "TailsGo.h"

TailsGo::TailsGo(const std::string& textureId, const std::string& n)
	:SpriteGo(textureId, n)
{
}

void TailsGo::Init()
{
	sprite.setColor(sf::Color::Yellow);
	SetPosition(player->GetPosition());
	SetOrigin(Origins::MR);
	sprite.setScale(0.f, 1.f);
}

void TailsGo::Reset()
{
	SpriteGo::Reset();

	SetPosition(player->GetPosition());
	SetOrigin(Origins::MR);
	sprite.setScale(0.f, 1.f);
}

void TailsGo::Update(float dt)
{
	SpriteGo::Update(dt);
	direction = player->GetDirection();

	SetPosition(player->GetPosition());
	if (player->isMoving)
	{
		IncreaseTailWidth(dt);
		//sprite.setScale(1.f, 1.f);
		SetRotation(direction);
	}
	else
	{
		DecreaseTailWidth(dt);
		//sprite.setScale(0.f, 1.f);
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
	x += dt*5;
	if (x <= maxWidth)
	{
		sprite.setScale(x,1.f);
	}
}

void TailsGo::DecreaseTailWidth(float dt)
{
	float x = sprite.getScale().x;
	x -= dt * 20;
	if (x <= 1.5f && x>=0.f)
	{
		sprite.setScale(x, 1.f);
	}
}
