#pragma once
#include <iostream>
#include "../TileClass.h"
#include "BlockClass.h"
using namespace std;

class Bouncer: public Tile 
{
    public:
        Bouncer();
        void Power(Map &, Display & display, Coords);
        void DrawName() const;
        void Draw() const {cout << char(ch);}
        Coords GetFacing() {return facing;}
        
        static const int INV_SIZE = 0;
        Inventory inv{INV_SIZE};
        Inventory GetInventory() const;
        
        void MoveInvIndex(int n) {inv.MoveIndex(n);}
        bool PutInInv(Item & i) {return inv.PutIn(i);}
        void SetSlot(Item item) {inv.SetSlot(item);}
        void SetColor(int i){color = i;}
        
        Bouncer (Block block)
        {
            id = block.GetId();
            name = block.GetName();
            color = block.GetColor();
            ch = int(block.GetChar());
            can_be_interacted = block.GetInteracted();
            can_be_stepped = block.GetStepped();
            type = block.GetType();
            description = block.GetDescription();
            facing = block.GetFacing();
        }
        
};