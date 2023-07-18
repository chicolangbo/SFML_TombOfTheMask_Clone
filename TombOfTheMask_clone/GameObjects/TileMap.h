#pragma once
#include "VertexArrayGo.h"

class Spikes;
class SpriteGo;

enum class Obstacles
{
	None,
	SpikeWall,
	Spike,
	Bat,
};

enum class Item
{
	None,
	BCoin,
	SCoin,
};

struct Tile
{
	int x = 0;
	int y = 0;
	int texIndex = -1;
	Obstacles obstacleIndex = Obstacles::None;
	Item itemIndex = Item::None;
};

class TileMap : public VertexArrayGo
{
protected:
	std::vector<Spikes*> spikes = {};
	std::vector<SpriteGo*> BCoins = {};
	std::vector<SpriteGo*> SCoins = {};

	sf::Vector2i size = {0,0};

public:
	TileMap(const std::string& textureId = "", const std::string& n = "");
	virtual ~TileMap() override;

	bool Load(const std::string& filePath);

	void SetSpikes(std::vector<Spikes*> spikes);
	void SetBCoins(std::vector<SpriteGo*> BCoins);
	void SetSCoins(std::vector<SpriteGo*> SCoins);
	void SetEnum(Tile& t, int i);

	sf::Vector2f GetPosition(int x, int y);

	std::vector<Tile> tiles;
};