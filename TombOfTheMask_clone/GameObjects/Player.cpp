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
	SetPosition(120.f+15.f,150.f+15.f);
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
	MovePlayer(dt, CheckCollide());
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

void Player::SetRotation(COLLIDE c)
{
	// 회전 초기화
	float tempR = sprite.getRotation();
	sprite.setRotation(-tempR);
	sprite.setScale(2.f, 2.f);

	switch (c)
	{
	case COLLIDE::L:
		sprite.setRotation(90.f);
		break;
	case COLLIDE::R:
		sprite.setRotation(-90.f);
		break;
	case COLLIDE::T:
		sprite.setRotation(-180.f);
		break;
	case COLLIDE::B:
		sprite.setRotation(0.f);
		break;
	}
}

void Player::SetMap(TileMap* tilemap)
{
	this->tileMap = tilemap;
}

void Player::MovePlayer(float dt, COLLIDE c)
{
	// flip
	if (direction.x != 0.f && sprite.getRotation() == 0.f)
	{
		bool flip = direction.x < 0.f;
		if (GetFlipX() != flip)
		{
			SetFlipX(flip);
		}
	}
	if (direction.y != 0.f && sprite.getRotation() == 0.f)
	{
		bool flip = direction.y < 0.f;
		if (GetFlipY() != flip)
		{
			SetFlipY(flip);
		}
	}

	// move
	if (!isMoving)
	{
		//std::cout << "키 입력" << std::endl;
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::W))
		{
			std::cout << "WWWWWWWWWWWW" << std::endl;
			isMoving = true;
			wMove = true;
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::A))
		{
			std::cout << "AAAAAAAAAAAAA" << std::endl;
			isMoving = true;
			aMove = true;
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::S))
		{
			std::cout << "SSSSSSSSSSSSS" << std::endl;
			isMoving = true;
			sMove = true;
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::D))
		{
			std::cout << "DDDDDDDDDDDD" << std::endl;
			isMoving = true;
			dMove = true;
		}
	}
	else
	{
		std::cout << "이동" << std::endl;
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
		position += direction * speed * dt;
		SetPosition(position);
	}

	// rotation
}

COLLIDE Player::CheckCollide()
{
	// 플레이어가 속한 타일의 인덱스
	sf::Vector2i playerTileIndex = (sf::Vector2i)(GetPosition() / 30.f);

	int tileSize = tileMap->tiles.size();
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Return))
	{
		std::cout << playerTileIndex.x << "," << playerTileIndex.y << std::endl;
		std::cout << GetPosition().x << "," << GetPosition().y << std::endl;
		std::cout << tileSize << std::endl;
	}
	for (int i = 0; i < tileSize; i++)
	{
		if (tileMap->tiles[i].texIndex == 17)
		{
			continue;
		}
		if (tileMap->tiles[i].x == playerTileIndex.x && tileMap->tiles[i].y == playerTileIndex.y) // 인덱스가 같으면
		{
			//std::cout << "충돌" << std::endl;
			if (direction.x == 1)
			{
				SetPosition(position.x - 1.f-15.f, position.y);
				MoveReset();
				SetRotation(COLLIDE::R);
				return COLLIDE::R;
			}
			if (direction.x == -1)
			{
				SetPosition(position.x + 1.f+15.f, position.y);
				MoveReset();
				SetRotation(COLLIDE::L);
				return COLLIDE::L;
			}
			if (direction.y == 1)
			{
				SetPosition(position.x, position.y -1.f-15.f);
				MoveReset();
				SetRotation(COLLIDE::B);
				return COLLIDE::B;
			}
			if (direction.y == -1)
			{
				SetPosition(position.x, position.y + 1.f + 15.f);
				MoveReset();
				SetRotation(COLLIDE::T);
				return COLLIDE::T;
			}
		}
	}
	isCollide = false;
	return COLLIDE::NONE;
}

void Player::MoveReset()
{
	wMove = false;
	aMove = false;
	sMove = false;
	dMove = false;
	isMoving = false;
}
