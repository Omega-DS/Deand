#include "aux_func/FuncAux.h"
#include "DisplayClass.h"
#include "blocks/Producer.h"
#include "blocks/ChestClass.h"
#include "blocks/BouncerClass.h"
#include "blocks/PipeClass.h"
#include "globals/database.h"
#include <synchapi.h>
#include <iostream>
#include <conio.h>
using namespace std;

const int WIDTH = 49;
const int HEIGHT = 25;

bool MainLoop(Player player);
bool InvLoop(Display &, Player &, Inventory &, bool &);
bool interactedLoop(Display &, Player &, Map &, Coords, bool &);

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
        gotoxy(WIDTH + 5, HEIGHT + 1);
        while(kbhit()) getch();         // Clear buffer
        opt = getch();
        
        switch (opt)
        {
            case 'd':
            case 'a':
            case 'w':
            case 's':
                coords.x = (opt == 'd') - (opt == 'a');
                coords.y = (opt == 's') - (opt == 'w');
                player.SetFacing(coords);
                player.Move(map);
                    
                display.UpdateMap(map, player.GetCoords());
                break;
            case 'i':
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
                if(map.GetBlock(player.GetFacing())->GetInteracted())
                {
                    while(interactedLoop(display, player, map, player.GetFacing(), crafting));
                    crafting = false;
                    map.GetBlock(player.GetFacing())->GetInventory().ResetIndex();
                    display.ClearText();
                }
                break;
            case 'l':
                coords = player.GetFacing();
                map.tiles[coords.x][coords.y]->Power(map, display, player.GetCoords());
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
    
    gotoxy(WIDTH + 5, HEIGHT + 1);
    while(kbhit()) getch();         // Clear buffer
    opt = getch();
    
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
    
    gotoxy(WIDTH + 5, HEIGHT + 1);
    while(kbhit()) getch();         // Clear buffer
    opt = getch();
       
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