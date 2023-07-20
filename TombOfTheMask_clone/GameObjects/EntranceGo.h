#pragma once
#include "AnimationController.h"
#include "SpriteGo.h"

class EntranceGo :
    public SpriteGo
{
protected:
    AnimationController animation;
    float totalTime = 0.f;

public:
    EntranceGo(const std::string& textureId = "", const std::string& n = "") : SpriteGo(textureId, n) {};
    virtual ~EntranceGo() override { Release(); };

    virtual void Init() override;
    virtual void Reset() override;
    virtual void Update(float dt) override;
    virtual void Draw(sf::RenderWindow& window) override;

};

