#include "InventoryClass.h"
#include "aux_func/FuncAux.h"
#include "globals/database.h"
#include <iostream>
using namespace std;

const int MAX_SLOT = 99;
        
Inventory::Inventory(int size_in)
{
    size = size_in;
    ind = 0;
    cont.reserve(10);
    for (int i = 0; i < size; i++)
    {
        Item item;
        cont.push_back(item);
    }
}

Inventory::Inventory(Recipe rec)
{
    DataBase data_base;
    size = int(rec.GetSize());
    ind = 0;
    cont.reserve(10);
    for (int i = 0; i < size; i++)
    {
        Item item = data_base.SearchItem(rec.GetIngredient(i).id);
        item.SetQuant(rec.GetIngredient(i).quant);
        cont.push_back(item);
    }
}

void Inventory::MoveIndex(int n)
{
    ind = (ind + n) % size;
    if (ind < 0)
        ind += size;
}

int Inventory::GetSize() const
{
    return size;
}

Item Inventory::GetSlot(int i)
{
    if (i == -1)
        i = ind;
    return cont[i];
}

void Inventory::SetSlot(Item item, int i)
{
    if (i == -1)
        i = ind;
    cont[i] = item;
    ClearEmpty();
}
void Inventory::ResetIndex()
{
    ind = 0;
}
bool Inventory::IsEmpty()
{
    bool empty = true;
    int i = 0;
    
    while(i < size && empty)
    {
        if (cont[i].GetId() != 0)
            empty = false;
        i++;
    }
    return empty;
}
void Inventory::DelSlot()
{
    Item item;
    cont[ind] = item;
}

bool Inventory::SearchItem(int id, int & pos, int & quant, int starting_pos) const
{
    int i = starting_pos;
    bool found = false;
    
    while(i < size && cont[i].GetId() != id)
        i++;
    
    
    if(i < size)
    {
        pos = i;
        quant = cont[i].GetQuant();
        found = true;
    }
    
    return found;
    
}

bool Inventory::PutIn(Item & item)
{
    int pos, quant, can_place, to_place, start = 0;
    bool placed_all = true;
    
    // Fill filled spaces
    while(start < size && SearchItem(item.GetId(), pos, quant, start) && item.GetQuant() > 0)
    {
        if(quant < MAX_SLOT)
        {
            can_place = MAX_SLOT - quant;
            to_place = min(can_place, item.GetQuant());
            item.SetQuant(item.GetQuant() - to_place);
            cont[pos].SetQuant(quant + to_place);
        }
        start = pos + 1;
    }
    
    // Fill emtpy spaces
    if(item.GetQuant() > 0)
    {
        start = 0;
        
        while(start < size && SearchItem(0, pos, quant, start) && item.GetQuant() > 0)
        {
            to_place = min(MAX_SLOT, item.GetQuant());
            
            SetSlot(item, pos);
            GetSlot(pos).SetQuant(to_place);
            
            item.SetQuant(item.GetQuant() - to_place);
            start = pos + 1;
        }
    }
    
    if(item.GetQuant() > 0)
        placed_all = false;
        
    return placed_all;
}

int Inventory::GetInd()
{
    return ind;
}

void Inventory::SetInd(int i)
{
    ind = i;
}


bool Inventory::RecipeCheck(Recipe rec)
{
    int pos = -1,
        q, quant, start,
        aux_q;
    unsigned int i = 0;
    bool ok = true;
    
    while(i < rec.GetSize() && ok)
    {
        start = 0;
        quant = 0;
        while(quant < rec.GetIngredient(i).quant 
            && SearchItem(rec.GetIngredient(i).id, pos, q, start))
        {
            aux_q = min(q, rec.GetIngredient(i).quant - quant);
            cont[pos].SetQuant(q - aux_q);
            start = pos + 1;
            quant += q;
        }
        
        if (quant < rec.GetIngredient(i).quant)
            ok = false;
        
        i ++;
    }
    
    ClearEmpty();
    return ok;    
}

void Inventory::ClearEmpty()
{
    Item empty;
    
    for(unsigned int i = 0; i < cont.size(); i++)
        if(cont[i].GetQuant() <= 0)
            cont[i] = empty;
}

void Inventory::Draw()
{
    int x, y, active;
    const char hor = char(196), vert = char(179),
              hor_b = char(194), hor_t = char(193),
              vert_r = char(195), vert_l = char(180),
              cross = char(197),
              r_b = char(218), l_b = char(191), r_t = char(192), l_t = char(217);
              
    active = ind;
    
    getCursorPos(x, y);
    gotoxy(x, y);
    cout << "                                               ";
    
    if(GetSlot().GetId() != 0)
    {
        gotoxy(x, y);
        cout.width(MAX_ROW * 2 - 4);
        cout.fill(' ');
        GetSlot().DrawName();
        cout << " x" << GetSlot().GetQuant();
    }
    
    y++;
    gotoxy(x, y);
    
    cout << r_b << hor;
    for(int i = 1; i < size && i < MAX_ROW; i++)
        cout << hor_b << hor;
    cout << l_b;
    gotoxy(x, y + 1);
    for(int i = 0; i < size && i < MAX_ROW; i++)
    {
        ind = i;
        cout << vert;
        GetSlot().DrawChar(ind == active);
    }
    cout << vert;
    
    int n = 0;
    while(n < (size - 1)/MAX_ROW)
    {
        n++;
        y += 2;
        gotoxy(x, y);
        
        cout << vert_r << hor;
        for(int j = 1; j < size && j < MAX_ROW; j ++)
        {
            if(j < size - (n * MAX_ROW) + 1)
                cout << cross;
            else
                cout << hor_t;
            cout << hor;
        }
        if(n < (size - 1) / MAX_ROW || size % MAX_ROW == 0)
            cout << vert_l;
        else
            cout << l_t;
        
        gotoxy(x, y + 1);
        
        for (int j = 0; j < size - (n * MAX_ROW) && j < MAX_ROW; j++)
        {
            ind++;
            cout << vert;
            GetSlot().DrawChar(active == ind);
        }
        cout << vert;
    }
    
    gotoxy(x, y + 2);
    cout << r_t << hor;
    for(int i = 0; i < size - (n * MAX_ROW) - 1; i++)
        cout << hor_t << hor;
    cout << l_t;
    
    
    ind = active;
}