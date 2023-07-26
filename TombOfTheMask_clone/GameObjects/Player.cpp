#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "Framework.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneGame1.h"
#include "SceneGame2.h"

void Player::Init()
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/CharFlight.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/CharIdle.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/CharEnter.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/CharRun.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/CharWinDie.csv"));

	animation.SetTarget(&sprite);
	sprite.setColor(sf::Color::Yellow);
	SetOrigin(Origins::MC);
	//SetPosition(tileMap->GetPosition(4, 5));

	poolTails.OnCreate = [this](TailsGo* tails)
	{
		tails->textureId = "graphics/character/tails.png";
		tails->pool = &poolTails;
		tails->SetPosition(position);
		tails->SetOrigin(Origins::MR);
		tails->sprite.setScale(0.f, 1.f);
		tails->reachMaxWidth = false;
	};
	poolTails.Init();
}

void Player::Release()
{
	SpriteGo::Release();
	poolTails.Release();
}

void Player::Reset()
{
	// 플레이어 세팅값 초기화
	totalTime = 0.f;
	sprite.setRotation(0.f);
	animation.Play("CharEnter");
	SetPosition(initialPos);
	sprite.setScale(2.f, 2.f);
	SetOrigin(origin);
	isMoving = false;
	direction = { 0,0 };
	MoveReset();
	isDie = false;
	isWin = false;
	score = 0;

	Scene* scene = SCENE_MGR.GetCurrScene();
	if (sceneIndex == 1)
	{
		sceneGame1 = dynamic_cast<SceneGame1*>(scene);
	}
	else if (sceneIndex == 2)
	{
		sceneGame2 = dynamic_cast<SceneGame2*>(scene);
	}

	// 타일 세팅값 초기화
	for (int i = 0; i < BCoins.size(); ++i)
	{
		BCoins[i]->SetActive(true);
	}
	for (int i = 0; i < SCoins.size(); ++i)
	{
		SCoins[i]->SetActive(true);
	}
	for (int i = 0; i < spikes.size(); ++i)
	{
		spikes[i]->collide = false;
	}

	poolTails.OnCreate = [this](TailsGo* tails)
	{
		tails->textureId = "graphics/character/tails.png";
		tails->pool = &poolTails;
		tails->SetPosition(position);
		tails->SetOrigin(Origins::MR);
		tails->sprite.setScale(0.f, 1.f);
		tails->reachMaxWidth = false;
	};

	// tails 초기화
	for (auto tail : poolTails.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(tail);
	}
	poolTails.Clear();
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

	// USING CODE
	totalTime += dt;
	if (totalTime <= 2.f)
	{
		if (sound.getStatus() != sf::Sound::Playing)
		{
			sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/Shield.wav"));
			sound.play();
		}

		sprite.setScale(totalTime, totalTime);
		sprite.setColor(sf::Color(255, 255, 0, 0.f + totalTime * 127.5f));
		if (animation.GetCurrentClipId() != "CharEnter")
		{
			animation.Play("CharEnter");
		}
	}
	else
	{
		if (sceneIndex == 1)
		{
			MovePlayer1(dt);
		}
		else if (sceneIndex == 2)
		{
			MovePlayer2(dt);
		}
		SetAnimation();
	}
	
	animation.Update(dt);

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
	sprite.setRotation(0);

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

void Player::SetAnimation()
{
	CheckCoinCollide();
	CheckTileCollide();
	CheckSpikeCollide();
	CheckArrival();

	if (!isMoving)
	{
		if ((isWin || isDie) && animation.GetCurrentClipId() != "CharWinDie")
		{
			animation.Play("CharWinDie");
		}
		else if(!isWin && !isDie && animation.GetCurrentClipId() != "CharIdle")
		{
			animation.Play("CharIdle");
		}
	}
	else
	{
		if (animation.GetCurrentClipId() != "CharFlight")
		{
			animation.Play("CharFlight");
		}
	}

	SetOrigin(Origins::MC);
}

void Player::SetMap(TileMap* tilemap)
{
	this->tileMap = tilemap;
}

void Player::SetWater(RectGo* deathWater)
{
	this->deathWater = deathWater;
}

void Player::SetSpikes(std::vector<Spikes*> spikes)
{
	this->spikes = spikes;
}

void Player::SetBCoins(std::vector<SpriteGo*> BCoins)
{
	this->BCoins = BCoins;
}

void Player::SetSCoins(std::vector<SpriteGo*> SCoins)
{
	this->SCoins = SCoins;
}

void Player::SetDestination(DestinationGo* des)
{
	this->destination = des;
}

void Player::MovePlayer1(float dt)
{
	if (!isMoving)
	{
		//std::cout << "안움직이는중" << std::endl;
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::W))
		{
			sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/PopUp.wav"));
			sound.play();

			TailsGo* tail = poolTails.Get();
			if (sceneGame1 != nullptr)
			{
				sceneGame1->AddGo(tail);
				tail->SetPlayer(sceneGame1->GetPlayer());
				tail->sprite.setScale(0.f, 1.f);
				tail->direction = { 0.f,-1.f };
			}
			SetRotation(COLLIDE::T);
			isMoving = true;
			wMove = true;
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::A))
		{
			sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/PopUp.wav"));
			sound.play();

			TailsGo* tail = poolTails.Get();
			if (sceneGame1 != nullptr)
			{
				sceneGame1->AddGo(tail);
				tail->SetPlayer(sceneGame1->GetPlayer());
				tail->sprite.setScale(0.f, 1.f);
				tail->direction = { -1.f,0.f };
			}
			SetRotation(COLLIDE::L);
			isMoving = true;
			aMove = true;
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::S))
		{
			sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/PopUp.wav"));
			sound.play();

			TailsGo* tail = poolTails.Get();
			if (sceneGame1 != nullptr)
			{
				sceneGame1->AddGo(tail);
				tail->SetPlayer(sceneGame1->GetPlayer());
				tail->sprite.setScale(0.f, 1.f);
				tail->direction = { 0.f,1.f };
			}
			SetRotation(COLLIDE::B);
			isMoving = true;
			sMove = true;
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::D))
		{
			sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/PopUp.wav"));
			sound.play();

			TailsGo* tail = poolTails.Get();
			if (sceneGame1 != nullptr)
			{
				sceneGame1->AddGo(tail);
				tail->SetPlayer(sceneGame1->GetPlayer());
				tail->sprite.setScale(0.f, 1.f);
				tail->direction = { 1.f,0.f };
			}
			SetRotation(COLLIDE::R);
			isMoving = true;
			dMove = true;
		}
	}
	else
	{
		//std::cout << "무빙" << std::endl;
		if (wMove)
		{
			direction = { 0,-1 };
		}
		else if (aMove)
		{
			direction = { -1,0 };
		}
		else if (sMove)
		{
			direction = { 0,1 };
		}
		else if (dMove)
		{
			direction = { 1,0 };
		}
		position += direction * speed * dt;
		SetPosition(position);
	}
}

void Player::MovePlayer2(float dt)
{
	CheckWaterCollide();
	if (!isMoving)
	{
		//std::cout << "안움직이는중" << std::endl;
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::W))
		{
			sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/PopUp.wav"));
			sound.play();

			TailsGo* tail = poolTails.Get();
			if (sceneGame2 != nullptr)
			{
				sceneGame2->AddGo(tail);
				tail->SetPlayer(sceneGame2->GetPlayer());
				tail->sprite.setScale(0.f, 1.f);
				tail->direction = { 0.f,-1.f };
			}
			SetRotation(COLLIDE::T);
			isMoving = true;
			wMove = true;
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::A))
		{
			sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/PopUp.wav"));
			sound.play();

			TailsGo* tail = poolTails.Get();
			if (sceneGame2 != nullptr)
			{
				sceneGame2->AddGo(tail);
				tail->SetPlayer(sceneGame2->GetPlayer());
				tail->sprite.setScale(0.f, 1.f);
				tail->direction = { -1.f,0.f };
			}
			SetRotation(COLLIDE::L);
			isMoving = true;
			aMove = true;
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::S))
		{
			sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/PopUp.wav"));
			sound.play();

			TailsGo* tail = poolTails.Get();
			if (sceneGame2 != nullptr)
			{
				sceneGame2->AddGo(tail);
				tail->SetPlayer(sceneGame2->GetPlayer());
				tail->sprite.setScale(0.f, 1.f);
				tail->direction = { 0.f,1.f };
			}
			SetRotation(COLLIDE::B);
			isMoving = true;
			sMove = true;
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::D))
		{
			sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/PopUp.wav"));
			sound.play();

			TailsGo* tail = poolTails.Get();
			if (sceneGame2 != nullptr)
			{
				sceneGame2->AddGo(tail);
				tail->SetPlayer(sceneGame2->GetPlayer());
				tail->sprite.setScale(0.f, 1.f);
				tail->direction = { 1.f,0.f };
			}
			SetRotation(COLLIDE::R);
			isMoving = true;
			dMove = true;
		}
	}
	else
	{
		//std::cout << "무빙" << std::endl;
		if (wMove)
		{
			direction = { 0,-1 };
		}
		else if (aMove)
		{
			direction = { -1,0 };
		}
		else if (sMove)
		{
			direction = { 0,1 };
		}
		else if (dMove)
		{
			direction = { 1,0 };
		}
		position += direction * speed * dt;
		SetPosition(position);
	}
}

void Player::CheckCoinCollide()
{
	for (int i = 0; i < SCoins.size(); ++i)
	{
		if (SCoins[i]->sprite.getGlobalBounds().intersects(sprite.getGlobalBounds()))
		{
			if (SCoins[i]->GetActive())
			{
				sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/Coin_1.wav"));
				sound.play();

				SCoins[i]->SetActive(false);
				score += GETSCOIN;
				std::cout << score << std::endl;
			}
		}
	}
	for (int i = 0; i < BCoins.size(); ++i)
	{
		if (BCoins[i]->sprite.getGlobalBounds().intersects(sprite.getGlobalBounds()))
		{
			if (BCoins[i]->GetActive())
			{
				sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/Coin_1.wav"));
				sound.play();

				BCoins[i]->SetActive(false);
				score += GETBCOIN;
				std::cout << score << std::endl;
			}
		}
	}
}

COLLIDE Player::CheckTileCollide()
{
	// 플레이어가 속한 타일의 인덱스
	sf::Vector2i playerTileIndex = (sf::Vector2i)(GetPosition() / 30.f);

	int tileSize = tileMap->tiles.size();

	for (int i = 0; i < tileSize; i++)
	{
		if (tileMap->tiles[i].texIndex == 17 && tileMap->tiles[i].obstacleIndex == Obstacles::None)
		{
			continue;
		}
		if (tileMap->tiles[i].x == playerTileIndex.x && tileMap->tiles[i].y == playerTileIndex.y) // 인덱스가 같으면
		{
			// SpikeWall : 바로 죽음
			if (tileMap->tiles[i].obstacleIndex == Obstacles::SpikeWall)
			{
				if (sound.getBuffer() != RESOURCE_MGR.GetSoundBuffer("sounds/Death.wav"))
				{
					sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/Death.wav"));
					sound.play();
				}

				isDie = true;
				isMoving = false;
				startPos = position;
				return COLLIDE::DIE;
			}

			// 충돌 시 set position
			if (direction.x == 1 && tileMap->tiles[i].obstacleIndex == Obstacles::None)
			{
				sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/Landing.wav"));
				sound.play();

				SetPosition(position.x -15.f, position.y);
				MoveReset();
				SetRotation(COLLIDE::R);
				startPos = position;
				return COLLIDE::R;
			}
			else if (direction.x == -1 && tileMap->tiles[i].obstacleIndex == Obstacles::None)
			{
				sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/Landing.wav"));
				sound.play();

				SetPosition(position.x +15.f, position.y);
				MoveReset();
				SetRotation(COLLIDE::L);
				startPos = position;
				return COLLIDE::L;
			}
			else if (direction.y == 1 && tileMap->tiles[i].obstacleIndex == Obstacles::None)
			{
				sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/Landing.wav"));
				sound.play();

				SetPosition(position.x, position.y -15.f);
				MoveReset();
				SetRotation(COLLIDE::B);
				startPos = position;
				return COLLIDE::B;
			}
			else if (direction.y == -1 && tileMap->tiles[i].obstacleIndex == Obstacles::None)
			{
				if (sound.getBuffer() != RESOURCE_MGR.GetSoundBuffer("sounds/Landing.wav"))
				{
					sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/Landing.wav"));
					sound.play();
				}

				SetPosition(position.x, position.y +15.f);
				MoveReset();
				SetRotation(COLLIDE::T);
				startPos = position;
				return COLLIDE::T;
			}
		}
	}
	return COLLIDE::NONE;
}

COLLIDE Player::CheckSpikeCollide()
{
	for (int i = 0; i < spikes.size(); ++i)
	{
		if (spikes[i]->sprite.getGlobalBounds().intersects(sprite.getGlobalBounds()))
		{
			if (spikes[i]->collide == false)
			{
				spikes[i]->collide = true;
			}
			else if (spikes[i]->GetCurFrame() != 0)
			{
				if (sound.getBuffer() != RESOURCE_MGR.GetSoundBuffer("sounds/Death.wav"))
				{
					sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/Death.wav"));
					sound.play();
				}

				isDie = true;
				MoveReset();
				startPos = position;
			}
			return COLLIDE::DIE;
		}
	}
	return COLLIDE::NONE;
}

COLLIDE Player::CheckArrival()
{
	if (Utils::Distance(position, destination->GetPosition()) <= 1.f)
	{
		if (sound.getBuffer() != RESOURCE_MGR.GetSoundBuffer("sounds/Win.wav"))
		{
			sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/Win.wav"));
			sound.play();
		}

		isWin = true;
		MoveReset();
		startPos = position;
		return COLLIDE::WIN;
	}
	return COLLIDE::NONE;
}

COLLIDE Player::CheckWaterCollide()
{
	if (sprite.getGlobalBounds().intersects(deathWater->rect.getGlobalBounds()))
	{
		if (sound.getBuffer() != RESOURCE_MGR.GetSoundBuffer("sounds/Death.wav"))
		{
			sound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/Death.wav"));
			sound.play();
		}

		isDie = true;
		MoveReset();
		startPos = position;
		return COLLIDE::DIE;
	}
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
