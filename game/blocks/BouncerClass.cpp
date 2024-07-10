#include "BouncerClass.h"
#include "../MapClass.h"
#include "../DisplayClass.h"

#include <synchapi.h>
using namespace std;

Bouncer::Bouncer()
{}


void Bouncer::Power(Map & map, Display & display, Coords cen)
{
    int color_aux = color;
    Coords coords = facing;
    
    
    color = 11;
    display.UpdateMap(map, cen);
    Sleep(100);
    
    already_powered = true;
    
    for(int i = 1; i > -2; i -= 2)
    {
        coords.y += i;
        if(!map.GetBlock(coords)->already_powered && map.InMap(coords))
            map.tiles[coords.x][coords.y]->Power(map, display, cen);
        coords.y -= i;
    }
    
    for(int i = 1; i > -2; i -= 2)
    {
        coords.x += i;
        if(!map.GetBlock(coords)->already_powered && map.InMap(coords))
            map.tiles[coords.x][coords.y]->Power(map, display, cen);
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
