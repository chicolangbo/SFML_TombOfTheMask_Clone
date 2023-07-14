#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "RectGo.h"

class Player :
    public SpriteGo
{
protected:
    AnimationController animation;
    sf::Vector2f direction;

    float speed = 500.f;

    bool flipX = false;
    bool isMoving = false;
    bool wMove = false;
    bool aMove = false;
    bool sMove = false;
    bool dMove = false;

    //sf::RectangleShape floor;

public:
    Player(const std::string& textureId = "", const std::string& n = "") : SpriteGo(textureId, n) {};
    virtual ~Player() override { Release(); };

    virtual void Init() override;
    virtual void Reset() override;
    virtual void Update(float dt) override;
    virtual void Draw(sf::RenderWindow& window) override;

    bool GetFlipX() const;
    void SetFlipX(bool filp);

};
