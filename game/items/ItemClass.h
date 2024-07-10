#pragma once

#include <string>
#include <fstream>
#include "../RecipeClass.h"
using namespace std;

class Item
{
    public:
        Item();
        Item(string name_in, int id_in, char ch_in, int color_in, bool cbp_in, vector<int> craft_recip_in);
        char GetChar();
        string GetName() const;
        int GetId() const;
        int GetQuant() const;
        bool GetPlaced() const;
        Recipe GetRecipe() const;
        void DrawChar(bool active = false);
        void DrawName();
        
        string GetDescription();
        
        void SetId(int);
        void SetQuant(int);
        
        bool ReadFromFile(ifstream &);
        
        friend bool operator<(Item, Item);
        
    private:
        string name, description;
        int id, quant, color, ch;
        bool can_be_placed;
        Recipe crafting_recipe;
        
};