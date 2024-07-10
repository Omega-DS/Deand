#include "ChestClass.h"
#include "../MapClass.h"
#include "../DisplayClass.h"

#include <synchapi.h>
using namespace std;

Chest::Chest()
{}


void Chest::Power(Map & map, Display & display, Coords cen)
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
    
    already_powered = true;
    
    if(!map.GetBlock(facing)->already_powered && map.InMap(facing))
        map.tiles[facing.x][facing.y]->Power(map, display, cen);
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
