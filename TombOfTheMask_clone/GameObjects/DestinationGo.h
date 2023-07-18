#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "TileMap.h"

class DestinationGo :
    public SpriteGo
{
protected:
    AnimationController animation;

public:
    DestinationGo(const std::string& textureId = "", const std::string& n = "") : SpriteGo(textureId, n) {};
    virtual ~DestinationGo() override { Release(); };

    virtual void Init() override;
    virtual void Reset() override;
    virtual void Update(float dt) override;
    virtual void Draw(sf::RenderWindow& window) override;
};

