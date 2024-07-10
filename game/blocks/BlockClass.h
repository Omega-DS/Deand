#pragma once

#include "../InventoryClass.h"
#include "../TileClass.h"
using namespace std;

class Block : public Tile
{
    public:
        Block();
        void Interacted();
        void SetFacing(Coords); 
        Coords GetFacing() const;
        string GetName() const;
        void DrawName() const;
        char GetChar() const;
        bool GetStepped() const;
        bool GetInteracted() const;
        int GetId() const;
        int GetColor() const;
        string GetDescription() const;
        string GetType() {return type;}
        void SetId(int);
        Recipe GetCraftingItems() const;
        
        void SetInteracted(bool);
        void SetStepped(bool);
        
        bool ReadFromFile(ifstream &);
        
        friend bool operator<(Block, Block);
        
    protected:
        string name, description, type;
        int id, color, ch;
        bool can_be_stepped, can_be_interacted;
        Coords facing;
        Recipe crafting_items;
};
