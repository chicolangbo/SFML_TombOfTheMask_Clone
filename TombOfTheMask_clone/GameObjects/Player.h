#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "RectGo.h"
#include "TileMap.h"

class Player :
    public SpriteGo
{
protected:
    AnimationController animation;
    sf::Vector2f direction;

    TileMap* tileMap = nullptr;

    float speed = 300.f;
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
    void SetMap(TileMap* tilemap);
    void MovePlayer(float dt);

    void CheckCollide();
};
