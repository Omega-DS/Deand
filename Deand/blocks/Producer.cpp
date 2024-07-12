#include "Producer.h"
#include "../MapClass.h"
#include "../DisplayClass.h"

#include <synchapi.h>
using namespace std;

Producer::Producer()
{}


void Producer::Power(Map & map, Display & display, Coords cen, Coords c)
{
    int color_aux = color;
    Inventory inv_aux{inv.GetSize()};
    Item item = crafting_inventory.GetSlot();
    
    color = 11;
    display.UpdateMap(map, cen);
    Sleep(200);
    color = color_aux;
    
    if(type == "Crafting")
    {
        inv_aux = inv;
        if(!inv.RecipeCheck(item.GetRecipe()))
        {
            inv = inv_aux;
        }
        else if(!map.tiles[facing.x][facing.y]->PutInInv(item))
            PutInInv(item);
    }
    else
    {
        if(!map.tiles[facing.x][facing.y]->PutInInv(item))
            PutInInv(item); 
    }
    
    if(!map.GetBlock(facing)->AlreadyPowered(c) && map.InMap(facing))
    {
        map.tiles[facing.x][facing.y]->AddPowered(c);
        map.tiles[facing.x][facing.y]->Power(map, display, cen, facing);
    }
}

Inventory Producer::GetInventory() const
{
    return inv;
}

void Producer::DrawName() const
{
    textcolor(color);
    cout << name;
    textcolor(15);
}
