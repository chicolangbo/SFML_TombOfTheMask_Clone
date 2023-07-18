#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "TileMap.h"

class Spikes :
    public SpriteGo
{
protected:
    AnimationController animation;
    //int tileIndex;

public:
    bool collide;

    Spikes(const std::string& textureId = "", const std::string& n = "") : SpriteGo(textureId, n) {};
    virtual ~Spikes() override { Release(); };

    virtual void Init() override;
    virtual void Reset() override;
    virtual void Update(float dt) override;
    virtual void Draw(sf::RenderWindow& window) override;

   int GetCurFrame() { return animation.GetCurFrame(); }
   //void SetTileIndex(int t) { this->tileIndex = t; }
   //int GetTileIndex() { return this->tileIndex; }
};

