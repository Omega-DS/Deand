#include "ChestClass.h"
#include "../MapClass.h"
#include "../DisplayClass.h"

#include <synchapi.h>
using namespace std;

Chest::Chest()
{}


void Chest::Power(Map & map, Display & display, Coords cen, Coords c)
{
    int color_aux = color,
        i = 0;
    Item item = inv.GetSlot();
    
    
    color = 11;
    display.UpdateMap(map, cen);
    Sleep(100);
    color = color_aux;
    
    while (i < INV_SIZE && inv.GetSlot(i).GetId() == 0)
        i++;
    
    if(i != INV_SIZE)
    {
        item = inv.GetSlot(i);
        map.tiles[facing.x][facing.y]->PutInInv(item);
        inv.SetSlot(item, i);
    }

    if(!map.GetBlock(facing)->AlreadyPowered(c) && map.InMap(facing))
    {
        map.tiles[facing.x][facing.y]->AddPowered(c);
        map.tiles[facing.x][facing.y]->Power(map, display, cen, facing);
    }
}

Inventory Chest::GetInventory() const
{
    return inv;
}

void Chest::DrawName() const
{
    textcolor(color);
    cout << name;
    textcolor(15);
}
