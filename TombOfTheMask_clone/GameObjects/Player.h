#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "RectGo.h"
#include "TileMap.h"
#include "Spikes.h"
#include "DestinationGo.h"
#include "ObjectPool.h"
#include "TailsGo.h"

class SceneGame1;
class SceneGame2;

enum class COLLIDE
{
    NONE = -1,
    L,
    R,
    T,
    B,
    DIE,
    WIN
};

class Player :
    public SpriteGo
{
protected:
    AnimationController animation;
    sf::Vector2f direction;
    sf::Vector2f startPos = {0,0};
    

    TileMap* tileMap = nullptr;
    std::vector<Spikes*> spikes = {};
    std::vector<SpriteGo*> BCoins = {};
    std::vector<SpriteGo*> SCoins = {};
    DestinationGo* destination = nullptr;

    SceneGame1* sceneGame1 = nullptr;
    SceneGame2* sceneGame2 = nullptr;

    float speed = 500.f;
    bool flipX = false;
    bool flipY = false;
    bool wMove = false;
    bool aMove = false;
    bool sMove = false;
    bool dMove = false;

    float totalTime = 0.f;


public:
    sf::Vector2f initialPos = { 0,0 };
    ObjectPool <TailsGo> poolTails;
    bool isDie = false;
    bool isWin = false;
    int score = 0;
    bool isMoving = false;
    int sceneIndex = 0;

    Player(const std::string& textureId = "", const std::string& n = "") : SpriteGo(textureId, n) {};
    virtual ~Player() override { Release(); };

    virtual void Init() override;
    virtual void Release() override;
    virtual void Reset() override;
    virtual void Update(float dt) override;
    virtual void Draw(sf::RenderWindow& window) override;

    bool GetFlipX() const { return flipX; };
    bool GetFlipY() const { return flipY; };
    sf::Vector2f GetDirection() const { return direction; }
    sf::Vector2f GetStartPos() const { return startPos; }
    void SetFlipX(bool filp);
    void SetFlipY(bool flip);
    void SetRotation(COLLIDE c);
    void SetAnimation();

    void SetMap(TileMap* tilemap);
    void SetSpikes(std::vector<Spikes*> spikes);
    void SetBCoins(std::vector<SpriteGo*> BCoins);
    void SetSCoins(std::vector<SpriteGo*> SCoins);
    void SetDestination(DestinationGo* des);
    
    int GetScore() { return score; }

    void MovePlayer1(float dt);
    void MovePlayer2(float dt);
    void CheckCoinCollide();
    COLLIDE CheckTileCollide();
    COLLIDE CheckSpikeCollide();
    COLLIDE CheckArrival();
    void MoveReset();
};

#define GETSCOIN 10
#define GETBCOIN 50
//
//template <typename T>
//void MovePlayer(T& s, float dt) {
//	if (!this->isMoving)
//	{
//		//std::cout << "안움직이는중" << std::endl;
//		if (this->INPUT_MGR.GetKeyDown(sf::Keyboard::W))
//		{
//			TailsGo* tail = this->poolTails.Get();
//			if (s != nullptr)
//			{
//				s->AddGo(tail);
//				tail->SetPlayer(s->GetPlayer());
//				tail->sprite.setScale(0.f, 1.f);
//				tail->direction = { 0.f,-1.f };
//			}
//			this->SetRotation(COLLIDE::T);
//			this->isMoving = true;
//			this->wMove = true;
//		}
//		if (this->INPUT_MGR.GetKeyDown(sf::Keyboard::A))
//		{
//			TailsGo* tail = this->poolTails.Get();
//			if (s != nullptr)
//			{
//				s->AddGo(tail);
//				tail->SetPlayer(s->GetPlayer());
//				tail->sprite.setScale(0.f, 1.f);
//				tail->direction = { -1.f,0.f };
//			}
//			this->SetRotation(COLLIDE::L);
//			this->isMoving = true;
//			this->aMove = true;
//		}
//		if (this->INPUT_MGR.GetKeyDown(sf::Keyboard::S))
//		{
//			TailsGo* tail = this->poolTails.Get();
//			if (s != nullptr)
//			{
//				s->AddGo(tail);
//				tail->SetPlayer(s->GetPlayer());
//				tail->sprite.setScale(0.f, 1.f);
//				tail->direction = { 0.f,1.f };
//			}
//			this->SetRotation(COLLIDE::B);
//			this->isMoving = true;
//			this->sMove = true;
//		}
//		if (this->INPUT_MGR.GetKeyDown(sf::Keyboard::D))
//		{
//			TailsGo* tail = this->poolTails.Get();
//			if (s != nullptr)
//			{
//				s->AddGo(tail);
//				tail->SetPlayer(s->GetPlayer());
//				tail->sprite.setScale(0.f, 1.f);
//				tail->direction = { 1.f,0.f };
//			}
//			this->SetRotation(COLLIDE::R);
//			this->isMoving = true;
//			this->dMove = true;
//		}
//	}
//	else
//	{
//		//std::cout << "무빙" << std::endl;
//		if (this->wMove)
//		{
//			this->direction = { 0,-1 };
//		}
//		else if (this->aMove)
//		{
//			this->direction = { -1,0 };
//		}
//		else if (this->sMove)
//		{
//			this->direction = { 0,1 };
//		}
//		else if (this->dMove)
//		{
//			this->direction = { 1,0 };
//		}
//		this->position += this->direction * this->speed * dt;
//		this->SetPosition(this->position);
//	}
//}
