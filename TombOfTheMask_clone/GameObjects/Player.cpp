#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "Framework.h"
#include "ResourceMgr.h"

void Player::Init()
{
	//RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/IdleF.csv");
	//RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/IdleLR.csv");
	//RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/IdleB.csv");
	//RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/MoveF.csv");
	//RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/MoveLR.csv");
	//RESOURCE_MGR.Load(ResourceTypes::AnimationClip, "animations/MoveB.csv");

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
			animation.Play("CharArrive");
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
		{
			animation.Play("CharFlight");
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num3))
		{
			animation.Play("CharIdle");
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num4))
		{
			animation.Play("CharLongJump");
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num5))
		{
			animation.Play("CharRun");
		}
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

		/* {
			if (animation.GetCurrentClipId() == "IdleF" ||
				animation.GetCurrentClipId() == "IdleLR" ||
				animation.GetCurrentClipId() == "IdleB")
			{
				if (direction.x != 0.f)
				{
					animation.Play("MoveLR");
				}
				if (direction.y < 0)
				{
					animation.Play("MoveB");
				}
				if (direction.y > 0)
				{
					animation.Play("MoveF");
				}
			}
			else if (animation.GetCurrentClipId() == "MoveLR")
			{
				if (direction.x == 0.f && direction.y == 0.f)
				{
					animation.Play("IdleLR");
				}
			}
			else if (animation.GetCurrentClipId() == "MoveB")
			{
				if (direction.x == 0.f && direction.y == 0.f)
				{
					animation.Play("IdleB");
				}
			}
			else if (animation.GetCurrentClipId() == "MoveF")
			{
				if (direction.x == 0.f && direction.y == 0.f)
				{
					animation.Play("IdleF");
				}
			}
		} */
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