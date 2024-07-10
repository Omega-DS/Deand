#include "MapClass.h"
#include "aux_func/FuncAux.h"
using namespace std;
#include <typeinfo>
Map::Map()
{
    for (int i = 0; i < MAX_MAP; i++)
        for (int j = 0; j < MAX_MAP; j++)
        {
            Block *p = new Block();
            tiles[i][j] = p;
        }
}

void Map::ResetPowered()
{
    for (int i = 0; i < MAX_MAP; i++)
        for (int j = 0; j < MAX_MAP; j++)
        {
            tiles[i][j]->powered_by.clear();
        }
}

int Map::GetSize() const
{
    return MAX_MAP;
}

bool Map::InMap(Coords coords) const
{
    return InRange(coords.x, 0, MAX_MAP - 1) && InRange(coords.y, 0, MAX_MAP - 1);
}

Tile* Map::GetBlock(Coords coords) const
{
    return tiles[coords.x][coords.y];
}


void Map::SetBlock(Coords coords, Block block)
{
    Block *p;
    p = new Block();
    *p = block;
    tiles[coords.x][coords.y] = p;
}