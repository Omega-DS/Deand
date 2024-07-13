#pragma once
#include <iostream>
#include "../TileClass.h"
#include "BlockClass.h"
using namespace std;

class Producer: public Tile 
{
    public:
        Producer();
        void Power(Map &, Display & display, Coords, Coords c);
        void DrawName() const;
        void Draw() const {cout << char(ch);}
        Coords GetFacing() {return facing;}
        
        static const int INV_SIZE = 16;
        Inventory inv{INV_SIZE};
        Inventory GetInventory() const;
        
        Inventory crafting_inventory{INV_SIZE};
        Inventory GetCraftingInventory() const {return crafting_inventory;}
        
        void SetInventory(Inventory i) {inv = i;}
        
        void MoveInvIndex(int n) {inv.MoveIndex(n);}
        bool PutInInv(Item & i) {return inv.PutIn(i);}
        void MoveCraftingInvIndex(int n) {crafting_inventory.MoveIndex(n);}
        void SetSlot(Item item) {inv.SetSlot(item);}
        void SetColor(int i){color = i;}
        
        void SetCraftingInventoryInd(int i) {crafting_inventory.SetInd(i);}
        
        Producer (Block block)
        {
            id = block.GetId();
            name = block.GetName();
            color = block.GetColor();
            ch = int(block.GetChar());
            can_be_interacted = block.GetInteracted();
            can_be_stepped = block.GetStepped();
            type = block.GetType();
            description = block.GetDescription();
            crafting_items = block.GetCraftingItems();
            Inventory c_inv{crafting_items};
            crafting_inventory = c_inv;
            facing = block.GetFacing();
        }
};