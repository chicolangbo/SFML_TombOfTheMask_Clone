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
            size_t firstSlashIndex = cell.find('/');
            size_t secondSlashIndex = cell.find('/', firstSlashIndex + 1);
            size_t lastSlashIndex = cell.rfind('/');
            if (firstSlashIndex != std::string::npos) // '/'를 한개 포함하면
            {
                // 첫번째 할당
                int first = stoi(cell.substr(0, firstSlashIndex));
                tile.texIndex = first;

                // 두번째 할당
                if (secondSlashIndex == std::string::npos)
                {
                    int second = stoi(cell.substr(firstSlashIndex + 1));
                    SetEnum(tile, second);
                }
                // 세번째 할당
                else
                {
                    int mid = stoi(cell.substr(firstSlashIndex + 1, secondSlashIndex - firstSlashIndex - 1));
                    SetEnum(tile, mid);

                    int last = stoi(cell.substr(lastSlashIndex + 1));
                    SetEnum(tile, last);
                }
            }
            else // '/'를 포함하지 않으면
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
    int scIndex = 0;
    int bcIndex = 0;
    for (int i = 0; i < size.y; ++i) // 행
    {
        for (int j = 0; j < size.x; ++j) // 열
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
            if (tiles[tileIndex].obstacleIndex == Obstacles::Spike)
            {
                if (spikes[sIndex] == nullptr)
                {
                    continue;
                }
                spikes[sIndex]->SetPosition(vertexArray[vertexIndex].position.x, vertexArray[vertexIndex].position.y - 15.f);
                spikes[sIndex]->SetTileIndex(tileIndex);
                ++sIndex;
            }
            if (tiles[tileIndex].itemIndex == Item::SCoin)
            {
                if (SCoins[scIndex] == nullptr)
                {
                    continue;
                }
                SCoins[scIndex]->SetPosition(vertexArray[vertexIndex].position.x + 15.f, vertexArray[vertexIndex].position.y - 15.f);
                //SCoins[scIndex]->SetTileIndex(tileIndex);
                ++scIndex;
            }
            if (tiles[tileIndex].itemIndex == Item::BCoin)
            {
                if (BCoins[bcIndex] == nullptr)
                {
                    continue;
                }
                BCoins[bcIndex]->SetPosition(vertexArray[vertexIndex].position.x + 15.f, vertexArray[vertexIndex].position.y - 15.f);
                //SCoins[scIndex]->SetTileIndex(tileIndex);
                ++bcIndex;
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

void TileMap::SetEnum(Tile& t, int i)
{
    switch (i)
    {
    case 18:
        t.obstacleIndex = Obstacles::Spike;
        break;
    case 19:
        t.itemIndex = Item::SCoin;
        break;
    case 20:
        t.itemIndex = Item::BCoin;
        break;
    case 21:
        t.obstacleIndex = Obstacles::Bat;
        break;
    }
}

void TileMap::SetSpikes(std::vector<Spikes*> spikes)
{
    this->spikes = spikes;
}

void TileMap::SetBCoins(std::vector<SpriteGo*> BCoins)
{
    this->BCoins = BCoins;
}

void TileMap::SetSCoins(std::vector<SpriteGo*> SCoins)
{
    this->SCoins = SCoins;
}