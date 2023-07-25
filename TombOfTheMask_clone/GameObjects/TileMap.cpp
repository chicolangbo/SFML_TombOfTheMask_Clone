#include "stdafx.h"
#include "TileMap.h"
#include "rapidcsv.h"
#include "Spikes.h"
#include "DestinationGo.h"

TileMap::TileMap(const std::string& textureId, const std::string& n)
	: VertexArrayGo(textureId, n)
{
}

TileMap::~TileMap()
{
}

void TileMap::Release()
{
    tiles.clear();
}

bool TileMap::Load(const std::string& filePath)
{
    rapidcsv::Document map(filePath, rapidcsv::LabelParams(-1, -1));
    size = { (int)map.GetColumnCount(), (int)map.GetRowCount() };

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
            size_t thirdSlashIndex = cell.find('/', secondSlashIndex + 1);
            //size_t lastSlashIndex = cell.rfind('/');
            if (firstSlashIndex != std::string::npos) // '/'�� 1�� �̻� �����ϸ�
            {
                // ù��° �Ҵ�
                int first = stoi(cell.substr(0, firstSlashIndex));
                tile.texIndex = first;
                SetEnum(tile, first);

                // �ι�° �Ҵ� : '/'�� 1���� �����ϸ�
                if (secondSlashIndex == std::string::npos)
                {
                    int second = stoi(cell.substr(firstSlashIndex + 1));
                    SetEnum(tile, second);
                }
                // ����° �Ҵ� : '/'�� 2���� �����ϸ�
                else if(secondSlashIndex != std::string::npos && thirdSlashIndex == std::string::npos)
                {
                    int second = stoi(cell.substr(firstSlashIndex + 1, secondSlashIndex - firstSlashIndex - 1));
                    SetEnum(tile, second);

                    int third = stoi(cell.substr(secondSlashIndex + 1, thirdSlashIndex - secondSlashIndex - 1));
                    SetEnum(tile, third);
                }
                // �׹�° �Ҵ� : '/'�� 3�� �� �����ϸ�
                else if(secondSlashIndex != std::string::npos && thirdSlashIndex != std::string::npos)
                {
                    int second = stoi(cell.substr(firstSlashIndex + 1, secondSlashIndex - firstSlashIndex - 1));
                    SetEnum(tile, second);

                    int third = stoi(cell.substr(secondSlashIndex + 1, thirdSlashIndex - secondSlashIndex - 1));
                    SetEnum(tile, third);

                    int fourth = stoi(cell.substr(thirdSlashIndex + 1));
                    if (fourth == 1 || fourth == 2 || fourth == 3 || fourth == 4)
                    {
                        tile.rotation = fourth;
                    }
                }
            }
            else // '/'�� �������� ������
            {
                tile.texIndex = stoi(cell);
                SetEnum(tile, stoi(cell));
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
            if (tiles[tileIndex].obstacleIndex == Obstacles::Spike)
            {
                if (spikes[sIndex] == nullptr)
                {
                    continue;
                }
                //spikes[sIndex]->sprite.setRotation(0.f);
                std::cout << tiles[tileIndex].rotation << std::endl;
                switch (tiles[tileIndex].rotation)
                {
                case 1:
                    spikes[sIndex]->sprite.setRotation(90.f);
                    break;
                case 2:
                    spikes[sIndex]->sprite.setRotation(180.f);
                    break;
                case 3:
                    spikes[sIndex]->sprite.setRotation(-90.f);
                    break;
                case 4:
                    spikes[sIndex]->sprite.setRotation(0.f);
                    break;
                }
                spikes[sIndex]->SetPosition(vertexArray[vertexIndex].position.x, vertexArray[vertexIndex].position.y - 15.f);
                ++sIndex;
            }
            if (tiles[tileIndex].itemIndex == Item::SCoin)
            {
                if (SCoins[scIndex] == nullptr)
                {
                    continue;
                }
                SCoins[scIndex]->SetPosition(GetPosition(j, i));
                ++scIndex;
            }
            if (tiles[tileIndex].itemIndex == Item::BCoin)
            {
                if (BCoins[bcIndex] == nullptr)
                {
                    continue;
                }
                BCoins[bcIndex]->SetPosition(GetPosition(j, i));
                ++bcIndex;
            }
            if (tiles[tileIndex].itemIndex == Item::Destination)
            {
                destination->SetPosition(GetPosition(j, i));
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
    case 12:
        t.obstacleIndex = Obstacles::SpikeWall;
        break;
    case 13:
        t.obstacleIndex = Obstacles::SpikeWall;
        break;
    case 14:
        t.obstacleIndex = Obstacles::SpikeWall;
        break;
    case 15:
        t.obstacleIndex = Obstacles::SpikeWall;
        break;
    case 18:
        t.obstacleIndex = Obstacles::SpikeWall;
        break;
    case 19:
        t.obstacleIndex = Obstacles::SpikeWall;
        break;
    case 20:
        t.obstacleIndex = Obstacles::SpikeWall;
        break;
    case 21:
        t.obstacleIndex = Obstacles::SpikeWall;
        break;
    case 22:
        t.obstacleIndex = Obstacles::Spike;
        break;
    case 23:
        t.itemIndex = Item::SCoin;
        break;
    case 24:
        t.itemIndex = Item::BCoin;
        break;
    case 25:
        t.obstacleIndex = Obstacles::Bat;
        break;
    case 26:
        t.itemIndex = Item::Destination;
        break;
    default:
        break;
    }
}

void TileMap::RotateSpikes()
{
    int sIndex = 0;
    for (int i = 0; i < tiles.size(); i++)
    {
        if (tiles[i].obstacleIndex == Obstacles::Spike)
        {
            switch (tiles[i].rotation)
            {
            case 1:
                spikes[sIndex]->sprite.setRotation(90.f);
                break;
            case 2:
                spikes[sIndex]->sprite.setRotation(180.f);
                break;
            case 3:
                spikes[sIndex]->sprite.setRotation(-180.f);
                break;
            case 4:
                spikes[sIndex]->sprite.setRotation(0.f);
                break;
            }
            ++sIndex;
        }
    }
}

void TileMap::SetSpikes(std::vector<Spikes*> spikes)
{
    this->spikes = spikes;
}

sf::Vector2f TileMap::GetPosition(int x, int y)
{
    int tileIndex = size.x * y + x;
    int vertexIndex = tileIndex * 4;
    sf::Vector2f pos = vertexArray[vertexIndex].position + sf::Vector2f(15.f, 15.f);

    return pos;
}

void TileMap::SetDestination(DestinationGo* des)
{
    this->destination = des;
}

void TileMap::SetBCoins(std::vector<SpriteGo*> BCoins)
{
    this->BCoins = BCoins;
}

void TileMap::SetSCoins(std::vector<SpriteGo*> SCoins)
{
    this->SCoins = SCoins;
}