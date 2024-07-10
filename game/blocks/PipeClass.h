#pragma once
#include <iostream>
#include "../TileClass.h"
#include "BlockClass.h"
using namespace std;

class Pipe: public Tile 
{
    public:
        Pipe();
        void Power(Map &, Display & display, Coords);
        void DrawName() const;
        void Draw() const {cout << char(ch);}
        Coords GetFacing() {return facing;}
        
        static const int INV_SIZE = 1;
        Inventory inv{INV_SIZE};
        Inventory GetInventory() const;
        
        void MoveInvIndex(int n) {inv.MoveIndex(n);}
        bool PutInInv(Item & i) {return inv.PutIn(i);}
        void SetSlot(Item item) {inv.SetSlot(item);}
        void SetColor(int i){color = i;}
        void CalculateChar();
        void CalculateFaces(Map &, Coords, bool);
        void SetSide(string, bool);
        
        bool east, west, north, south;
        int list_char[16] = {206, 205, 205, 205, 186, 200, 188, 202, 186, 201, 187, 203, 186, 204, 185, 206};
        
        Pipe (Block block)
        {
            id = block.GetId();
            name = block.GetName();
            color = block.GetColor();
            ch = 206;
            can_be_interacted = false;
            can_be_stepped = block.GetStepped();
            type = block.GetType();
            description = block.GetDescription();
            facing = block.GetFacing();
            east = west = north = south = false;
        }
        
};