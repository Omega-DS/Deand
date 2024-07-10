#include "PipeClass.h"
#include "../MapClass.h"
#include "../DisplayClass.h"

#include <synchapi.h>
using namespace std;

Pipe::Pipe()
{}


void Pipe::Power(Map & map, Display & display, Coords cen)
{
    int color_aux = color;
    Inventory inv_aux{inv.GetSize()};
    
    Item item = inv.GetSlot();
    
    color = 11;
    display.UpdateMap(map, cen);
    Sleep(50);
    color = color_aux;

    map.tiles[facing.x][facing.y]->PutInInv(item);
        
    already_powered = true;
    
    if(!map.GetBlock(facing)->already_powered && map.InMap(facing) && map.GetBlock(facing)->GetType() != "Storage")
        map.tiles[facing.x][facing.y]->Power(map, display, cen);
}

Inventory Pipe::GetInventory() const
{
    return inv;
}

void Pipe::DrawName() const
{
    textcolor(color);
    cout << name;
    textcolor(15);
}

void Pipe::CalculateChar()
{
    int i = east * 1 + west * 2 + north * 4 + south * 8;
    
    ch = list_char[i];
}

void Pipe::SetSide(string s, bool b)
{
    if(s == "east")
        east = b;
    else if(s == "west")
        west = b;
    else if(s == "north")
        north = b;
    else if(s == "south")
        south = b;
}

void Pipe::CalculateFaces(Map & map, Coords cen, bool b)
{
    Coords coords;
    coords.x = facing.x - cen.x;
    coords.y = facing.y - cen.y;
    
    if(coords.x == 1)
    {
        map.tiles[facing.x][facing.y]->SetSide("west", b);
        east = true;
    }
    else if(coords.x == -1)
    {
        map.tiles[facing.x][facing.y]->SetSide("east", b);
        west = true;
    }
    else if(coords.y == 1)
    {
        map.tiles[facing.x][facing.y]->SetSide("north", b);
        south = true;
    }
    else if(coords.y == -1)
    {
        map.tiles[facing.x][facing.y]->SetSide("south", b);
        north = true;
    }
    
    cen.x --;
    if(map.GetBlock(cen)->GetFacing().x == cen.x + 1 && map.GetBlock(cen)->GetFacing().y == cen.y)
        west = true;
    cen.x +=2;
    if(map.GetBlock(cen)->GetFacing().x == cen.x - 1 && map.GetBlock(cen)->GetFacing().y == cen.y)
        east = true;
    cen.x --;
    cen.y --;
    if(map.GetBlock(cen)->GetFacing().x == cen.x && map.GetBlock(cen)->GetFacing().y == cen.y + 1)
        north = true;
    cen.y +=2;
    if(map.GetBlock(cen)->GetFacing().x == cen.x && map.GetBlock(cen)->GetFacing().y == cen.y - 1)
        south = true;
    cen.y --;
    
    CalculateChar();
    map.tiles[facing.x][facing.y]->CalculateChar();
}
