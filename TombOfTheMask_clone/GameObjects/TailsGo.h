#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class Player;

class TailsGo :
    public SpriteGo
{
protected:
	Player* player = nullptr;
	sf::Vector2f direction = { 0.f,0.f };
	float maxWidth = 1.5f;
	bool reachMaxWidth = false;

public:
	ObjectPool<TailsGo>* pool;

	TailsGo(const std::string& textureId = "", const std::string& n = "");
	virtual ~TailsGo() override { Release(); };

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetRotation(sf::Vector2f d);
	void IncreaseTailWidth(float dt);
	void DecreaseTailWidth(float dt);

	void SetPlayer(Player* p) { player = p; }
};

