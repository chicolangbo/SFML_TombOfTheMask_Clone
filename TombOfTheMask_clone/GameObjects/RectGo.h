#pragma once
#include "GameObject.h"
class RectGo :
    public GameObject
{
protected:

public:
    sf::RectangleShape rect;

	RectGo(const std::string& n = "");
	virtual ~RectGo() override;

	sf::RectangleShape& GetRect();
	sf::Vector2f getSize();
	sf::Vector2f getScale();
	void setSize(sf::Vector2f size);
	void setScale(sf::Vector2f scale);

	virtual void SetPosition(const sf::Vector2f& p);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

