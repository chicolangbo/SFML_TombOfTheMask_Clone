#pragma once
#include "VertexArrayGo.h"

class Spikes;
class SpriteGo;
class DestinationGo;

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
	Destination
};

enum Index
{
	ST = 12,
	SB = 13,
	SL = 14,
	SR = 15,
	STL = 18,
	STR = 19,
	SBL = 20,
	SBR = 21,
	SPIKE = 22,
	SCOIN = 23,
	BCOIN = 24,
	BAT = 25,
	EXIT = 26
};

struct Tile
{
	int x = 0;
	int y = 0;
	int texIndex = -1;
	Obstacles obstacleIndex = Obstacles::None;
	Item itemIndex = Item::None;
	int rotation = 0;
};

class TileMap : public VertexArrayGo
{
protected:
	std::vector<Spikes*> spikes = {};
	std::vector<SpriteGo*> BCoins = {};
	std::vector<SpriteGo*> SCoins = {};
	DestinationGo* destination = nullptr;

	sf::Vector2i size = {0,0};

public:
	TileMap(const std::string& textureId = "", const std::string& n = "");
	virtual ~TileMap() override;
	void Release();

	bool Load(const std::string& filePath);

	void SetSpikes(std::vector<Spikes*> spikes);
	void SetBCoins(std::vector<SpriteGo*> BCoins);
	void SetSCoins(std::vector<SpriteGo*> SCoins);
	void SetDestination(DestinationGo* des);
	void SetEnum(Tile& t, int i);
	void RotateSpikes();

	sf::Vector2f GetPosition(int x, int y);
	sf::Vector2i GetSize() { return size; }

	std::vector<Tile> tiles;
};