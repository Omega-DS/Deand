#include "BouncerClass.h"
#include "../MapClass.h"
#include "../DisplayClass.h"

#include <synchapi.h>
using namespace std;

Bouncer::Bouncer()
{}


void Bouncer::Power(Map & map, Display & display, Coords cen, Coords c)
{
    int color_aux = color;
    Coords coords = facing;
    
    color = 11;
    display.UpdateMap(map, cen);
    Sleep(65);
    
    for(int i = 1; i > -2; i -= 2)
    {
        coords.y += i;
        if(!map.GetBlock(coords)->AlreadyPowered(c) && map.InMap(coords))
        {
            map.tiles[coords.x][coords.y]->AddPowered(c);
            map.tiles[coords.x][coords.y]->Power(map, display, cen, coords);
        }
        coords.y -= i;
        
        coords.x += i;
        if(!map.GetBlock(coords)->AlreadyPowered(c) && map.InMap(coords))
        {
            map.tiles[coords.x][coords.y]->AddPowered(c);
            map.tiles[coords.x][coords.y]->Power(map, display, cen,  coords);
        }
        coords.x -= i;
    }
    
    color = color_aux;
}

Inventory Bouncer::GetInventory() const
{
    return inv;
}

void Bouncer::DrawName() const
{
    textcolor(color);
    cout << name;
    textcolor(15);
}
