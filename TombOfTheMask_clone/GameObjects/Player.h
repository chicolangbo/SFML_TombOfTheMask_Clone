#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "RectGo.h"
#include "TileMap.h"
#include "Spikes.h"
#include "DestinationGo.h"
#include "ObjectPool.h"
#include "TailsGo.h"

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

    TileMap* tileMap = nullptr;
    std::vector<Spikes*> spikes = {};
    std::vector<SpriteGo*> BCoins = {};
    std::vector<SpriteGo*> SCoins = {};
    DestinationGo* destination = nullptr;

    float speed = 500.f;
    bool flipX = false;
    bool flipY = false;
    bool wMove = false;
    bool aMove = false;
    bool sMove = false;
    bool dMove = false;

    float totalTime = 0.f;

    ObjectPool <TailsGo> poolTails;

public:
    bool isDie = false;
    bool isWin = false;
    int score = 0;
    bool isMoving = false;

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

    void MovePlayer(float dt);
    void CheckCoinCollide();
    COLLIDE CheckTileCollide();
    COLLIDE CheckSpikeCollide();
    COLLIDE CheckArrival();
    void MoveReset();
};

#define GETSCOIN 10
#define GETBCOIN 50