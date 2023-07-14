#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "Framework.h"
#include "ResourceMgr.h"

void Player::Init()
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/CharFlight.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/CharIdle.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/CharLongJump.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/CharRun.csv"));

	animation.SetTarget(&sprite);
	SetOrigin(Origins::MC);
}

void Player::Reset()
{
	animation.Play("CharIdle");
	sprite.setScale(2.f, 2.f);
	sprite.setColor(sf::Color::Yellow);
	SetOrigin(origin);
	SetPosition(90.f+15.f,180.f+15.f);
	SetFlipX(false);
}

void Player::Update(float dt)
{
	// TEST CODE
	{
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
		{
			animation.Play("CharFlight"); // 이동 시
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
		{
			animation.Play("CharIdle"); // 좌,우,위
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num3))
		{
			animation.Play("CharLongJump"); // 마지막
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num4))
		{
			animation.Play("CharRun"); // 이동 시
		}
	}

	animation.Update(dt);
	//direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
	//direction.y = INPUT_MGR.GetAxis(Axis::Vertical);

	// USING CODE
	{
		if (direction.x != 0.f)
		{
			bool flip = direction.x < 0.f;
			if (GetFlipX() != flip)
			{
				SetFlipX(flip);
			}
		}
		if (direction.y != 0.f)
		{
			bool flip = direction.y < 0.f;
			if (GetFlipY() != flip)
			{
				SetFlipY(flip);
			}
		}

		if (!isMoving)
		{
			if (INPUT_MGR.GetKeyDown(sf::Keyboard::W))
			{
				isMoving = true;
				wMove = true;
			}
			if (INPUT_MGR.GetKeyDown(sf::Keyboard::A))
			{
				isMoving = true;
				aMove = true;
			}
			if (INPUT_MGR.GetKeyDown(sf::Keyboard::S))
			{
				isMoving = true;
				sMove = true;
			}
			if (INPUT_MGR.GetKeyDown(sf::Keyboard::D))
			{
				isMoving = true;
				dMove = true;
			}
		}
		else
		{
			if (wMove)
			{
				direction = { 0,-1 };
			}
			if (aMove)
			{
				direction = { -1,0 };
			}
			if (sMove)
			{
				direction = { 0,1 };
			}
			if (dMove)
			{
				direction = { 1,0 };
			}
		}
		position += direction * speed * dt;
		SetPosition(position);
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Player::SetFlipX(bool flip)
{
	flipX = flip;

	sf::Vector2f scale = sprite.getScale();
	scale.x = !flip ? abs(scale.x) : -abs(scale.x);
	sprite.setScale(scale);
}

void Player::SetFlipY(bool flip)
{
	flipY = flip;

	sf::Vector2f scale = sprite.getScale();
	scale.y = !flip ? abs(scale.y) : -abs(scale.y);
	sprite.setScale(scale);
}

void Player::SpeedOnOff(bool on)
{
	if (on)
	{
		speed = 300.f;
		return;
	}
	speed = 0.f;
}
