#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "RectGo.h"
#include "TileMap.h"
#include "Spikes.h"
#include "DestinationGo.h"

enum class COLLIDE
{
    NONE = -1,
    L,
    R,
    T,
    B,
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
    bool isMoving = false;

public:
    bool isDie = false;
    bool isWin = false;
    int score = 0;

    Player(const std::string& textureId = "", const std::string& n = "") : SpriteGo(textureId, n) {};
    virtual ~Player() override { Release(); };

    virtual void Init() override;
    virtual void Reset() override;
    virtual void Update(float dt) override;
    virtual void Draw(sf::RenderWindow& window) override;

    bool GetFlipX() const { return flipX; };
    bool GetFlipY() const { return flipY; };
    void SetFlipX(bool filp);
    void SetFlipY(bool flip);
    void SetRotation(COLLIDE c);

    void SetMap(TileMap* tilemap);
    void SetSpikes(std::vector<Spikes*> spikes);
    void SetBCoins(std::vector<SpriteGo*> BCoins);
    void SetSCoins(std::vector<SpriteGo*> SCoins);
    void SetDestination(DestinationGo* des);
    
    int GetScore() { return score; }

    void MovePlayer(float dt, COLLIDE c);
    COLLIDE CheckTileCollide();
    void CheckSpikeCollide();
    void CheckCoinCollide();
    void CheckArrival();
    void MoveReset();
};

#define GETSCOIN 10
#define GETBCOIN 50