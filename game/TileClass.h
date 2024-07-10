#pragma once
#include "InventoryClass.h"
#include "TileFwdClass.h"
#include <algorithm>
#include <iostream>
using namespace std;
class Display;
struct Coords
{
    int x, y;
};

class Map;

class Tile
{
    public:
        virtual ~Tile() {}
        virtual void Power(Map &, Display &, Coords, Coords) { }
        void Interacted();
        virtual void SetFacing(Coords) { } 
        Coords GetFacing() const;
        string GetName() const;
        void Draw() {textcolor(color);cout << char(ch);textcolor(15);}
        virtual void DrawName() const { }
        char GetChar() const;
        virtual bool GetStepped() const {return can_be_stepped;}
        bool GetInteracted() const {return can_be_interacted;};
        int GetId() const {return id;};
        int GetColor() const;
        string GetDescription() const {return description;};
        string GetType() const;
        void SetId(int);
        virtual void Draw() const {}
        virtual Inventory GetInventory() const {Inventory inv{0}; return inv;}
        virtual Inventory GetCraftingInventory() const {Inventory inv{0}; return inv;}
        virtual void SetInventory(Inventory i) {}
        virtual void SetCraftingInventory(Inventory) {}
        virtual void MoveInvIndex(int) {}
        virtual void MoveCraftingInvIndex(int){}
        virtual bool PutInInv(Item &) {return false;}
        virtual void SetSlot(Item) {}
        virtual string GetType() {return type;}
        virtual Coords GetFacing() {return facing;}
        Recipe GetCraftingItems() const {return crafting_items;}
        
        virtual void CalculateChar() {}
        virtual void CalculateFaces(Map &, Coords, bool) {}
        virtual void SetSide(string, bool) {}
        
        
        vector<Coords> powered_by;
        
        virtual bool AlreadyPowered(Coords c)
        {
            bool found = false;
            
            for(unsigned int i = 0; i < powered_by.size(); i++)
            {
                if(powered_by[i].x == c.x && powered_by[i].y == c.y)
                    found = true;
            }
            return found;
        }
        virtual void AddPowered(Coords c)
        {
            powered_by.push_back(c);
        }
        
        void SetInteracted(bool in) {can_be_interacted = in;}
        void SetStepped(bool);
        
        bool ReadFromFile(ifstream &);
        
    protected:
        string name, description, type;
        int id = 0, color, ch = int(' ');
        bool can_be_stepped, can_be_interacted;
        Coords facing;
        Recipe crafting_items;
};