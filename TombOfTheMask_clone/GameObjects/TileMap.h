#pragma once
#include "VertexArrayGo.h"


class Spikes;

enum class Obstacles
{
	None,
	SpikeWall,
	Spike,
	Bat,
};

struct Tile
{
	int x = 0;
	int y = 0;
	int texIndex = -1;
	Obstacles obstacleIndex = Obstacles::None;
};

class TileMap : public VertexArrayGo
{
protected:
	std::vector<Spikes*> spikes = {};

public:
	TileMap(const std::string& textureId = "", const std::string& n = "");
	virtual ~TileMap() override;

	bool Load(const std::string& filePath);

	void SetSpikes(std::vector<Spikes*> spikes);

	std::vector<Tile> tiles;
};

