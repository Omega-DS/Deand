#include "PlayerClass.h"
using namespace std;

const int MAX_MAP = 500;

Player::Player()
{
    name = "Daniel";
    ch = '&';
    facing.x = 0;
    facing.y = 1;
    coords.x = coords.y = 100;
}

Coords Player::GetCoords() const
{
    return coords;
}

Coords Player::GetFacing() const
{
    Coords c;
    c.x = facing.x + coords.x;
    c.y = facing.y + coords.y;
    return c;
}

char Player::GetChar() const
{
    return ch;
}

void Player::SetFacing(Coords coords)
{
    facing = coords;
}

bool Player::Move(const Map & map)
{
    bool ok = true;
    
    coords.x += facing.x;
    coords.y += facing.y;
    
    if(!map.InMap(coords) || !map.GetBlock(coords)->GetStepped())
    {
        ok = false;
        coords.x -= facing.x;
        coords.y -= facing.y;
    }
    
    return ok;
}
        