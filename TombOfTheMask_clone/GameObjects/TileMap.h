#pragma once
#include "VertexArrayGo.h"


class Spikes;

struct Tile
{
	int x = 0;
	int y = 0;
	int texIndex;
	int obstacleIndex = 0;
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

