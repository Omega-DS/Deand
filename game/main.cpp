#include "aux_func/FuncAux.h"
#include "DisplayClass.h"
#include "blocks/Producer.h"
#include "blocks/ChestClass.h"
#include "blocks/BouncerClass.h"
#include "blocks/PipeClass.h"
#include "globals/database.h"
#include <synchapi.h>
#include <iostream>
#include <cctype>
#include <conio.h>
#include <fstream>
using namespace std;

const int WIDTH = 49;
const int HEIGHT = 25;
const string FILE_PATH = "save/save_file.bin";

bool MainLoop(Player player);
bool InvLoop(Display &, Player &, Inventory &, bool &);
bool interactedLoop(Display &, Player &, Map &, Coords, bool &);
bool PauseLoop(Display & display, Map & map, Player & player);
void SaveGame(Map & map, Player & player);
void LoadGame(Map & map, Player & player);

int main()
{
    Player player;
    DataBase data_base;
    
    cout << "Version beta. No esperes nada, texto provisional" << endl << endl;
    cout << "Eres un viajero del espacio cuya nave se ha estrellado contra la superficie de un planeta inospito" << endl;
    cout << "Tu mision es construir un cohete para volver con tu familila. Buena suerte" << endl;
    system("pause");
    system("cls");
    
    while(MainLoop(player));
    
    gotoxy(0, HEIGHT);
    
    return 0;
}

bool MainLoop(Player player)
{
    DataBase data_base;
    Inventory inv(32);
    Display display;
    Map map;
    char opt;
    bool crafting;
    Item item;
    Block block;
    vector<int> ids;
    Coords coords;
    
    coords.x = 2; coords.y = 2;
    int n;
    
    for(unsigned int i = 1; i < data_base.GetSize(); i++)
        if(data_base.SearchItem(i).GetRecipe().GetSize() > 0)
        {
            ids.push_back(i);
            n ++;
        }
            
    Inventory craft_inv(n);
    for(unsigned int i = 0; i < ids.size(); i++)
            craft_inv.SetSlot(data_base.SearchItem(ids[i]), i);
    
    item = data_base.SearchItem(1);
    item.SetQuant(1);
    player.inv.PutIn(item);
    
    /*
    for(int i = 1; i < player.INV_SIZE + 1; i++ )
    {
        item = data_base.SearchItem(i);
        item.SetQuant(15);
        player.inv.PutIn(item);
    }*/
    
    display.Initialize();
    display.UpdateMap(map, player.GetCoords());
    display.UpdateInv(player);
    
    while(true)
    {
        gotoxy(0, 0);
        while(kbhit()) getch();         // Clear buffer
        opt = getch();
        
        switch (opt)
        {
            case 'D':
            case 'A':
            case 'W':
            case 'S':
            case 'd':
            case 'a':
            case 'w':
            case 's':
                opt = tolower(opt);
                coords.x = (opt == 'd') - (opt == 'a');
                coords.y = (opt == 's') - (opt == 'w');
                player.SetFacing(coords);
                player.Move(map);
                    
                display.UpdateMap(map, player.GetCoords());
                break;
            case 'p':
            case 'P':
                while(PauseLoop(display, map, player));
                display.ClearText();
                break;
            case 'i':
            case 'I':
                crafting = false;
                while(InvLoop(display, player, craft_inv, crafting));
                crafting = false;
                display.ClearText();
                display.ClearMenu();
                break;
            case 'k':
            case 'K':
                item = player.inv.GetSlot();
                if(item.GetPlaced() && map.GetBlock(player.GetFacing())->GetId() == 0 && map.InMap(player.GetFacing()))
                {
                    //Problema con bloques con x = 0
                    if(opt == 'k')
                        coords = player.GetCoords();
                    else
                    {
                        coords.x = 2*player.GetFacing().x - player.GetCoords().x;
                        coords.y = 2*player.GetFacing().y - player.GetCoords().y;
                    }
                    
                    if(!map.InMap(coords))
                        coords.x = coords.y = 0;
                    block = data_base.SearchBlock(item.GetId());
                    block.SetFacing(coords);
                    
                    if(block.GetType() == "Production" || block.GetType() == "Crafting")
                    {
                        Producer *p;
                        p = new Producer(block);
                        
                        delete map.tiles[player.GetFacing().x][player.GetFacing().y];
                        map.tiles[player.GetFacing().x][player.GetFacing().y] = p;
                    }
                    else if(block.GetType() == "Storage")
                    {
                        Chest *p;
                        p = new Chest(block);
                        
                        delete map.tiles[player.GetFacing().x][player.GetFacing().y];
                        map.tiles[player.GetFacing().x][player.GetFacing().y] = p;
                    }
                    else if(block.GetType() == "Power")
                    {
                        block.SetFacing(player.GetFacing());
                        Bouncer *p;
                        p = new Bouncer(block);
                        
                        delete map.tiles[player.GetFacing().x][player.GetFacing().y];
                        map.tiles[player.GetFacing().x][player.GetFacing().y] = p;
                    }
                    else if(block.GetType() == "Logistics")
                    {
                        Pipe *p;
                        p = new Pipe(block);
                        
                        delete map.tiles[player.GetFacing().x][player.GetFacing().y];
                        map.tiles[player.GetFacing().x][player.GetFacing().y] = p;
                        
                        map.tiles[player.GetFacing().x][player.GetFacing().y]->CalculateFaces(map, player.GetFacing(), true);
                    }
                        
                    item.SetQuant(item.GetQuant() - 1);
                    player.inv.SetSlot(item);
                    
                    display.UpdateMap(map, player.GetCoords());
                    display.UpdateInv(player);
                }
                break;
            case 'j':
            case 'J':
                coords = player.GetFacing();
                item = data_base.SearchItem(map.GetBlock(coords)->GetId());
                if(item.GetId() != 0 && player.inv.PutIn(item))
                {
                    
                    if(map.GetBlock(coords)->GetType() == "Logistics")
                        map.tiles[coords.x][coords.y]->CalculateFaces(map, coords, false);
                        
                    Block *p;
                    p = new Block();
                    
                    for(int i = 0; i < map.GetBlock(coords)->GetInventory().GetSize(); i++)
                    {
                        item = map.GetBlock(coords)->GetInventory().GetSlot(i);
                        player.inv.PutIn(item);
                    }
                        
                    delete map.tiles[coords.x][coords.y];
                    
                    map.tiles[coords.x][coords.y] = p;
                    map.tiles[coords.x][coords.y]->SetInteracted(false);
                    display.UpdateMap(map, player.GetCoords());
                    display.UpdateInv(player);
                    Sleep(100);
                }
                break;
            case 'e':
            case 'E':
                if(map.GetBlock(player.GetFacing())->GetInteracted())
                {
                    while(interactedLoop(display, player, map, player.GetFacing(), crafting));
                    crafting = false;
                    map.GetBlock(player.GetFacing())->GetInventory().ResetIndex();
                    display.ClearText();
                }
                break;
            case 'l':
            case 'L':
                coords = player.GetFacing();
                map.tiles[coords.x][coords.y]->Power(map, display, player.GetCoords(), coords);
                map.ResetPowered();
                display.UpdateMap(map, player.GetCoords());
                break;
        }
    
    }
    return true;
}

bool interactedLoop(Display & display, Player & player, Map & map, Coords coords, bool & menu)
{
    char opt;
    int n;
    Item item;
    display.DrawBlockInfo(map, coords);
    
    gotoxy(0, 0);
    while(kbhit()) getch();         // Clear buffer
    opt = getch();
    opt = tolower(opt);
    
    if(!menu)
        switch(opt)
        {
            case 'd':
            case 'a':
            case 'w':
            case 's':
                n = ((opt == 's') - (opt == 'w')) * player.inv.MAX_ROW;
                n += (opt == 'd') - (opt == 'a');
                player.inv.MoveIndex(n);
                display.UpdateInv(player);
                break;
            case 'j':
                item = player.inv.GetSlot();
                map.GetBlock(coords)->PutInInv(item);
                player.inv.SetSlot(item);
                display.UpdateInv(player);
                break;
            case 'h':
                if(map.GetBlock(coords)->GetType() != "Storage")
                    map.GetBlock(coords)->MoveCraftingInvIndex(1);
                break;
            case char(-32): // supr
                player.inv.DelSlot();
                display.UpdateInv(player);
                break;
            case '\t':
                menu = !menu;
                break;
        }
    else
        switch (opt)
        {
            case 'd':
            case 'a':
            case 'w':
            case 's':
                n = ((opt == 's') - (opt == 'w')) * map.GetBlock(coords)->GetInventory().MAX_ROW;
                n += (opt == 'd') - (opt == 'a');
                map.GetBlock(coords)->MoveInvIndex(n);
                display.DrawBlockInfo(map, coords);
                break;
            case 'j':
                item = map.GetBlock(coords)->GetInventory().GetSlot();
                player.inv.PutIn(item);
                map.GetBlock(coords)->SetSlot(item);
                display.UpdateInv(player);
                break;
            case 'h':
                map.GetBlock(coords)->MoveCraftingInvIndex(1);
                break;
            case char(-32): // supr
                map.GetBlock(coords)->GetInventory().DelSlot();
                break;
            case '\t':
                menu = !menu;
                break;
        }
    return opt != 'e';
}

bool InvLoop(Display & display, Player & player, Inventory & craft_inv, bool & crafting)
{
    int n;
    char opt;
    
    if(!crafting)
        display.DrawItemInfo(player.inv.GetSlot());
    else
        display.DrawItemInfo(craft_inv.GetSlot());
    display.UpdateCrafting(craft_inv);

    gotoxy(0, 0);
    while(kbhit()) getch();         // Clear buffer
    opt = getch();
    opt = tolower(opt); 
    if(!crafting)
    { 
        switch(opt)
        {
            case 'd':
            case 'a':
            case 'w':
            case 's':
                n = ((opt == 's') - (opt == 'w')) * player.inv.MAX_ROW;
                n += (opt == 'd') - (opt == 'a');
                player.inv.MoveIndex(n);
                display.UpdateInv(player);
                break;
            case char(-32): // supr
                player.inv.DelSlot();
                display.UpdateInv(player);
                break;
            case '\t':
                crafting = !crafting;
                break;
        }
    }
    else
    {
        Inventory inv_a(craft_inv.GetSize());
        Item item_a;
        switch(opt)
        {
            case 'd':
            case 'a':
            case 'w':
            case 's':
                n = ((opt == 's') - (opt == 'w')) * craft_inv.MAX_ROW;
                n += (opt == 'd') - (opt == 'a');
                craft_inv.MoveIndex(n);
                display.UpdateCrafting(craft_inv);
                break;
            case 'j':
                inv_a = player.inv;
                item_a = craft_inv.GetSlot();
                
                if(!(player.inv.RecipeCheck(craft_inv.GetSlot().GetRecipe()) && player.inv.PutIn(item_a)))
                    player.inv = inv_a;
                
                display.UpdateInv(player);
                sleep(0.8);
                break;
            case '\t':
                crafting = !crafting;
        }
    }
    
    return opt != 'i';
}

bool PauseLoop(Display & display, Map & map, Player & player)
{
    char opt;
    
    display.ShowPauseMenu();

    gotoxy(0, 0);
    while(kbhit()) getch();         // Clear buffer
    opt = getch();
    opt = tolower(opt);
    
    switch(opt)
    {
        case 'h':
            display.ShowHelp();
            break;
        case 's':
            //SaveGame(map, player);
            break;
        case 'l':
            //LoadGame(map, player);
            display.UpdateMap(map, player.GetCoords());
            display.UpdateInv(player);
            break;
    }
    
    return opt != 'p';
}

void SaveGame(Map & map, Player & player)
{
    ofstream f;
    
    f.open(FILE_PATH, ios::binary);
    
    if(f)
    {
        f.write((char*)(&player), sizeof(player));
        
        for(int i = 0; i < player.inv.GetSize(); i++)
           f.write((char*)&player.inv.cont[i], sizeof(Item));
           /*
        for (int i = 0; i < map.GetSize(); i++)
            for (int j = 0; j < map.GetSize(); j++)
            {
                f.write((char*)(map.tiles[i][j]), sizeof(*map.tiles[i][j]));
                
                int a = map.tiles[i][j]->GetInventory().GetSize();
                                
                f.write((char*)&a, a);
                for(int k = 0; k < a; k++)
                    f.write((char*)&map.tiles[i][j]->GetInventory().cont[k], sizeof(Item));
                a = map.tiles[i][j]->GetCraftingInventory().GetSize();
                f.write((char*)&a, sizeof(a));
                for(int k = 0; k < a; k++)
                    f.write((char*)&map.tiles[i][j]->GetCraftingInventory().cont[k], sizeof(Item));
            }*/
    }
    
    f.close();
}

void LoadGame(Map & map, Player & player)
{
    ifstream f;
    DataBase data_base;
    
    f.open(FILE_PATH, ios::binary);
    
    if(f)
    {
        f.read((char*)(&player), sizeof(player));
        
        for(int i = 0; i < player.inv.GetSize(); i++)
           f.read((char*)&player.inv.cont[i], sizeof(Item));
           /*
        for (int i = 0; i < map.GetSize(); i++)
            for (int j = 0; j < map.GetSize(); j++)
            {
                f.read((char*)(map.tiles[i][j]), sizeof(*map.tiles[i][j]));
                
                int s;
                
                f.read((char*)&s, sizeof(s));
                Inventory inv(s);
                
                for(int k = 0; k < s; k++)
                    f.read((char*)&inv.cont[k], sizeof(Item));
                map.tiles[i][j]->SetInventory(inv);
                
                
                f.read((char*)&s, sizeof(s));
                Inventory inv1(s);
                
                for(int k = 0; k < s; k++)
                    f.read((char*)&inv1.cont[k], sizeof(Item));
                map.tiles[i][j]->SetCraftingInventory(inv1);
                
            }*/
    }
    
    f.close();
}
