#include "stdafx.h"
#include "TileMap.h"
#include "rapidcsv.h"
#include "Spikes.h"

TileMap::TileMap(const std::string& textureId, const std::string& n)
	: VertexArrayGo(textureId, n)
{
}

TileMap::~TileMap()
{
}

bool TileMap::Load(const std::string& filePath)
{
    rapidcsv::Document map(filePath, rapidcsv::LabelParams(-1, -1));
    sf::Vector2i size = { (int)map.GetColumnCount(), (int)map.GetRowCount() };

    for (int i = 0; i < size.y; ++i)
    {
        for (int j = 0; j < size.x; ++j)
        {
            Tile tile;
            tile.x = j;
            tile.y = i;
            std::string cell = map.GetCell<std::string>(j, i);
            size_t str = cell.find('/');
            if (str != std::string::npos) // '/'�� �����ϸ�
            {
                if (stoi(cell.substr(str + 1)) == 18)
                {
                    std::cout << j << "," << i << std::endl;
                    tile.obstacleIndex = Obstacles::Spike;
                }
                tile.texIndex = stoi(cell.substr(0, str));
            }
            else
            {
                tile.texIndex = stoi(cell);
            }
            tiles.push_back(tile);
        }
    }

    sf::Vector2f tileSize = { 30.f, 30.f };
    sf::Vector2f texSize = { 30.f, 30.f };
    sf::Vector2f texOffsets[4] =
    {
        { 0.f, 0.f },
        { texSize.x, 0.f },
        { texSize.x, texSize.y },
        { 0.f, texSize.y }
    };


    // resize the vertex array to fit the level size
    vertexArray.setPrimitiveType(sf::Quads);
    vertexArray.resize(size.x * size.y * 4);
    sf::Vector2f startPos = { 0, 0 };
    sf::Vector2f currPos = startPos;

    sf::Vector2f offsets[4] =
    {
        { 0.f, 0.f },
        { tileSize.x, 0.f },
        { tileSize.x, tileSize.y },
        { 0.f, tileSize.y }
    };

    int sIndex = 0;
    for (int i = 0; i < size.y; ++i) // ��
    {
        for (int j = 0; j < size.x; ++j) // ��
        {
            int tileIndex = size.x * i + j;
            int texIndex = tiles[tileIndex].texIndex;
            int vertexIndex;
            for (int k = 0; k < 4; ++k)
            {
                vertexIndex = tileIndex * 4 + k;
                vertexArray[vertexIndex].position = currPos + offsets[k];
                vertexArray[vertexIndex].texCoords = texOffsets[k];
                vertexArray[vertexIndex].texCoords.y += texSize.y * texIndex;
            }
            if (tiles[tileIndex].obstacleIndex != Obstacles::None)
            {
                if (spikes[sIndex] == nullptr)
                {
                    continue;
                }
                spikes[sIndex]->SetPosition(vertexArray[vertexIndex].position.x, vertexArray[vertexIndex].position.y - 15.f);
                spikes[sIndex]->SetTileIndex(tileIndex);
                ++sIndex;
            }
            if (texIndex == 12 || texIndex == 13 || texIndex == 14 || texIndex == 15)
            {
                tiles[tileIndex].obstacleIndex = Obstacles::SpikeWall;
            }
            currPos.x += tileSize.x;
        }
        currPos.x = startPos.x;
        currPos.y += tileSize.y;
    }

    return true;
}

void TileMap::SetSpikes(std::vector<Spikes*> spikes)
{
    this->spikes = spikes;
}
