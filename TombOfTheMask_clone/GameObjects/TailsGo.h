#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class Player;

class TailsGo :
    public SpriteGo
{
protected:
	Player* player = nullptr;
	float maxWidth = 1.5f;
	bool directionChange = false;
	bool stop = false;
	int count = 0;

	sf::Vector2f startPos = { 0,0 };

public:
	sf::Vector2f direction = { 0.f,0.f };
	bool reachMaxWidth = false;
	ObjectPool<TailsGo>* pool;

	TailsGo(const std::string& textureId = "", const std::string& n = "");
	virtual ~TailsGo() override { Release(); };

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetRotation(sf::Vector2f d);
	void IncreaseTailWidth(float dt);
	bool DecreaseTailWidth(float dt);

	void SetPlayer(Player* p) { player = p; }
};

