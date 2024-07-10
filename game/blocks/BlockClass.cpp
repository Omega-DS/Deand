#include <string>
#include <iostream>
#include "BlockClass.h"
#include "../aux_func/FuncAux.h"
using namespace std;

Block::Block()
{
    name = "ground";
    id = 0;
    ch = ' ';
    can_be_interacted = false;
    can_be_stepped = true;
    color = 15;
}

void Block::SetInteracted(bool in)
{
    can_be_interacted = in;
}

void Block::SetStepped(bool in)
{
    can_be_stepped = in;
}

void Block::SetFacing(Coords coords)
{
    facing = coords;
}

bool Block::GetInteracted() const
{
    return can_be_interacted;
}
string Block::GetName() const
{
    return name;
}
int Block::GetColor() const
{
    return color;
}

string Block::GetDescription() const
{
    return description;
}
        
char Block::GetChar() const
{
    return char(ch);
}

int Block::GetId() const
{
    return id;
}

Coords Block::GetFacing() const
{
    return facing;
}

bool Block::GetStepped() const
{
    return can_be_stepped;
}

void Block::DrawName() const
{
    textcolor(color);
    cout << name;
    textcolor(15);
}

Recipe Block::GetCraftingItems() const
{
    return crafting_items;
}

void Block::SetId(int id_in)
{
    id = id_in;
}

bool Block::ReadFromFile(ifstream & f)
{
    bool ok = false;
    int amount;
    Ingredient ing;
    vector<Ingredient> list;
    
    f.ignore();
    if(f >> id)
    {  
        ok = true;
        f.ignore();
        getline(f, name);
        f >> ch;
        f >> color;
        f >> type;
        f >> can_be_stepped;
        f >> can_be_interacted;
        
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
        crafting_items = recipe;
        
        can_be_interacted = true;
        facing.x = facing.y = 0;
    }
    
    return ok;
}

bool operator<(Block b1, Block b2)
{
    return b1.id < b2.id;
}