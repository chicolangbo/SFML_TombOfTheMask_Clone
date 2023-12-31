#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

class LogoGo :
    public SpriteGo
{
protected:
    AnimationController animation;

public:
    LogoGo(const std::string& textureId = "", const std::string& n = "") : SpriteGo(textureId, n) {}
    virtual ~LogoGo() override { Release(); }

    virtual void Init() override;
    virtual void Reset() override;
    virtual void Update(float dt) override;
    virtual void Draw(sf::RenderWindow& window) override;
};

