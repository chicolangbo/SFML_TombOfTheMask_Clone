#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "RectGo.h"
#include "TileMap.h"
#include "Spikes.h"

enum class COLLIDE
{
    NONE = -1,
    L,
    R,
    T,
    B
};

class Player :
    public SpriteGo
{
protected:
    AnimationController animation;
    sf::Vector2f direction;

    TileMap* tileMap = nullptr;
    std::vector<Spikes*> spikes = {};

    float speed = 500.f;
    bool flipX = false;
    bool flipY = false;
    bool wMove = false;
    bool aMove = false;
    bool sMove = false;
    bool dMove = false;
    bool isMoving = false;
    bool isCollide = false;

public:
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

    void MovePlayer(float dt, COLLIDE c);
    COLLIDE CheckCollide();
    void MoveReset();
};
