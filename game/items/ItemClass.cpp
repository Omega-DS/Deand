#include "ItemClass.h"
#include <iostream>
using namespace std;

Item::Item()
{
    name = "Empty";
    id = 0;
    ch = ' ';
    can_be_placed = false;
    quant = 1;
    color = 15;
}

void Item::DrawChar(bool active)
{
    textcolor(color + (112 - 15 * (color == 15)) * active);
        
    cout << char(ch);
    textcolor(15);
}

void Item::DrawName()
{
    textcolor(color);
    cout << name;
    textcolor(15);
}

char Item::GetChar()
{
    return char(ch);
}
string Item::GetName() const
{
    return name;
}
int Item::GetId() const
{
    return id;
}
int Item::GetQuant() const
{
    return quant;
}

Recipe Item::GetRecipe() const
{
    return crafting_recipe;
}
string Item::GetDescription()
{
    return description;
}
void Item::SetId(int id_in)
{
    id = id_in;
}

void Item::SetQuant(int q)
{
    quant = q;
}

bool Item::GetPlaced() const
{
    return can_be_placed;
}

bool Item::ReadFromFile(ifstream & f)
{
    bool ok = false;
    int amount;
    Ingredient ing;
    vector<Ingredient> list;
    
    ing.id = 0;
    ing.quant = 0;
    
    f.ignore();
    if(f >> id)
    {  
        ok = true;
        f.ignore();
        getline(f, name);
        f >> ch;  
        f >> color;
        f >> can_be_placed;
        
        f >> amount;
        for(int i = 0; i < amount; i++)
        {
            f >> ing.id;
            f >> ing.quant;
            list.push_back(ing);
        }
        
        f.ignore();
        getline(f, description);
        
        Recipe recipe(list);
        crafting_recipe = recipe;
        
        quant = 1;
    }
    
    return ok;
}

bool operator< (Item i1, Item i2)
{
    return i1.id < i2.id;
}