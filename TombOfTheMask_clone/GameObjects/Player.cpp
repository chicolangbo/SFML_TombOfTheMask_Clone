#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "Framework.h"
#include "ResourceMgr.h"

void Player::Init()
{
	//RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/Idle.csv");

	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/CharArrive.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/CharFlight.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/CharIdle.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/CharLongJump.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/CharRun.csv"));

	animation.SetTarget(&sprite);
	SetOrigin(Origins::BC);
}

void Player::Reset()
{
	animation.Play("CharIdle");
	sprite.setScale(2.f, 2.f);
	sprite.setColor(sf::Color::Yellow);
	SetOrigin(origin);
	SetPosition(0,0);
	SetFlipX(false);
}

void Player::Update(float dt)
{
	// TEST CODE
	{
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
		{
			animation.Play("CharArrive"); // 지상
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
		{
			animation.Play("CharFlight"); // 이동 시
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num3))
		{
			animation.Play("CharIdle"); // 좌,우,위
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num4))
		{
			animation.Play("CharLongJump"); // 마지막
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num5))
		{
			animation.Play("CharRun"); // 이동 시
		}
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::D))
	{

	}

	animation.Update(dt);
	direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
	direction.y = INPUT_MGR.GetAxis(Axis::Vertical);

	// USING CODE
	{
		if (direction.x != 0.f)
		{
			bool flip = direction.x > 0.f;
			if (GetFlipX() != flip)
			{
				SetFlipX(flip);
			}
		}

		position += direction * speed * dt;

		SetPosition(position);

		{
			//이동
			direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
			direction.y = INPUT_MGR.GetAxis(Axis::Vertical);
			float magnitude = Utils::Magnitude(direction);
			if (magnitude > 1.f)
			{
				direction /= magnitude;
			}

			position += direction * speed * dt;
			SetPosition(position);



			if (direction.x != 0 || direction.y != 0)
			{
				if (animation.GetCurrentClipId() != "Move_Side" &&
					INPUT_MGR.GetAxisRaw(Axis::Horizontal) != 0.f)
				{
					animation.Play("Move_Side");
				}
				else if (animation.GetCurrentClipId() != "Move_Down" &&
					INPUT_MGR.GetAxisRaw(Axis::Vertical) > 0.f &&
					INPUT_MGR.GetAxisRaw(Axis::Horizontal) == 0.f)
				{
					animation.Play("Move_Down");
				}
				else if (animation.GetCurrentClipId() != "Move_Up" &&
					INPUT_MGR.GetAxisRaw(Axis::Vertical) < 0.f &&
					INPUT_MGR.GetAxisRaw(Axis::Horizontal) == 0.f)
				{
					animation.Play("Move_Up");
				}
			}
			else
			{
				if (animation.GetCurrentClipId() == "Move_Side" &&
					animation.GetCurrentClipId() != "Idle_Side")
				{
					animation.Play("Idle_Side");
				}
				else if (animation.GetCurrentClipId() == "Move_Down" &&
					animation.GetCurrentClipId() != "Idle_Down")
				{
					animation.Play("Idle_Down");
				}
				else if (animation.GetCurrentClipId() == "Move_Up" &&
					animation.GetCurrentClipId() != "Idle_Up")
				{
					animation.Play("Idle_Up");
				}
			}
			if (INPUT_MGR.GetAxisRaw(Axis::Horizontal) > 0)
			{
				animation.GetTarget()->setScale(-1, 1);
			}
			else if (INPUT_MGR.GetAxisRaw(Axis::Horizontal) < 0)
			{
				animation.GetTarget()->setScale(1, 1);
			}
			animation.Update(dt);
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

bool Player::GetFlipX() const
{
	return flipX;
}

void Player::SetFlipX(bool flip)
{
	flipX = flip;

	sf::Vector2f scale = sprite.getScale();
	scale.x = !flip ? abs(scale.x) : -abs(scale.x);
	sprite.setScale(scale);
}