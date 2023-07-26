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
    sf::Sound sound;

    TileMap* tileMap = nullptr;
    RectGo* deathWater = nullptr;
    std::vector<Spikes*> spikes = {};
    std::vector<SpriteGo*> BCoins = {};
    std::vector<SpriteGo*> SCoins = {};
    DestinationGo* destination = nullptr;

    SceneGame1* sceneGame1 = nullptr;
    SceneGame2* sceneGame2 = nullptr;

    float speed = 600.f;
    bool flipX = false;
    bool flipY = false;
    bool wMove = false;
    bool aMove = false;
    bool sMove = false;
    bool dMove = false;

public:
    float totalTime = 0.f;
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
    void SetWater(RectGo* deathWater);
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
    COLLIDE CheckWaterCollide();
    void MoveReset();
};

#define GETSCOIN 10
#define GETBCOIN 50