#include <iostream>
#include <string>
#include "DisplayClass.h"
#include "aux_func/FuncAux.h"
#include <typeinfo>
using namespace std;

const int MAX_MAP = 500;

Display::Display()
{
    is_empty = true;
    
}

void Display::Initialize()
{
    gotoxy(1,1);
    cout << char(218);
    for (int j = 1; j < MAP_W - 1; j++)
        cout << char(196);
    cout << char(191) << endl;
    
    for (int i = 2; i < MAP_H; i++)
    {
        gotoxy(1, i);
        cout << char(179);
        gotoxy(MAP_W, i);
        cout << char(179);
            
        cout << endl << ' ';
    }
    
    cout << char(192);
    for (int j = 1; j < MAP_W - 1; j++)
        cout << char(196);
    cout << char(217);
    
}


/**************************************************************************/ /**
 *
 * Imprime el texto en el cuadro de informacion
 *
 * @param  [in] text Texto a utilizar
 *
 ******************************************************************************/
void Display::WriteText(string text, int color)
{
    string s;
    int y, x;
    unsigned int ini = 0, fin = TEXT_W - 4; 
    
    textcolor(color);
    while (fin < text.length())
    {
        cout << char(179) << ' ';
        while(text[fin] != ' ')
            fin--;
            
        cout << text.substr(ini, fin - ini);
        getCursorPos(x, y);
        gotoxy(MAP_W + TEXT_W + SEP_H - 1, y);
        cout << char(179);
        gotoxy(MAP_W + SEP_H, y + 1);
        
        ini = fin + 1;
        fin = ini + TEXT_W - 4;
    }
    
    cout << char(179) << ' ';
    cout << text.substr(ini, text.length() - ini);
    getCursorPos(x, y);
    gotoxy(MAP_W + TEXT_W + SEP_H - 1, y);
    cout << char(179);
    
}

/**************************************************************************/ /**
 *
 * Limpia el cuadro de informacion
 *
 ******************************************************************************/
void Display::ClearText()
{
    is_empty = true;
    for (int i = 1; i <= TEXT_H; i++)
    {    
        gotoxy(MAP_W + SEP_H, i);
        cout << "                                                  ";
    }
}

void Display::ClearMenu()
{
    for (int i = 1; i <= MENU_H; i++)
    {    
        gotoxy(MAP_W + TEXT_W + 2 * SEP_H, i);
        cout << "                                                  ";
    }
}


void Display::UpdateMap(const Map map, Coords cen)
{
    Coords coords;
    int n = cen.y - MAP_H/2 + 2, 
        m = cen.x - MAP_W/2 + 2;
    for (int i = 0; i < MAP_H - 2; i++)
    {
        gotoxy(2, 2 + i);
        for (int j = 0; j < MAP_W - 2; j++)
        {
            coords.y = n + i;
            coords.x = m + j;
            if(map.InMap(coords))
                map.tiles[coords.x][coords.y]->Draw();
            else
                cout << char(176);
        }
    gotoxy(MAP_W/2, MAP_H/2);
    cout << '&';
    }
    
    gotoxy(1, MAP_H + 1);
    cout << "                  ";
    gotoxy(1, MAP_H + 1);
    cout << cen.x << char(250) << cen.y;
}

void Display::UpdateInv(Player & player)
{
    gotoxy(SEP_H - 1, MAP_H + SEP_V);
    player.inv.Draw();
}

void Display::UpdateCrafting(Inventory & craft_inv)
{
    int x, y, n;
    
    const char hor = char(196), vert = char(179),
              vert_r = char(195), vert_l = char(180),
              //cross = char(197), hor_b = char(194), hor_t = char(193),
              r_b = char(218), l_b = char(191), r_t = char(192), l_t = char(217);
              
    ClearMenu();
    x = MAP_W + TEXT_W + SEP_H * 2;
    
    gotoxy(x, 1);
    cout << r_b;
    for (int j = 1; j < TEXT_W - 1; j++)
        cout << hor;
    cout << l_b;
    
    gotoxy(x, 2);
    cout << vert << " Crafting menu: ";
    gotoxy(x + MENU_W - 1, 2);
    cout << vert;
    
    gotoxy(x, 3);
    cout << vert_r;
    for(int i = 1; i < MENU_W - 1; i++)
        cout << hor;
    cout << vert_l;
    
    gotoxy(x + 5, 4);
    craft_inv.Draw();
    getCursorPos(n, y);
    
    //gotoxy(x + 5, y + 5);
    //craft_inv.GetSlot().GetRecipe().Draw();
    
    for(int i = 4; i < MENU_H; i++)
    {
        gotoxy(x, i);
        cout << vert;
        gotoxy(x + MENU_W - 1, i);
        cout << vert;
    }
    getCursorPos(n, y);
    
    gotoxy(x, y + 1);
    cout << r_t;
    for (int j = 1; j < MENU_W - 1; j++)
        cout << hor;
    cout << l_t;
}

void Display::DrawBlockInfo(Map & map, Coords coords)
{
    int n, y, x = MAP_W + SEP_H;
    const char hor = char(196), vert = char(179),
              vert_r = char(195), vert_l = char(180),
              //cross = char(197), hor_b = char(194), hor_t = char(193),
              r_b = char(218), l_b = char(191), r_t = char(192), l_t = char(217);
              
    ClearText();
    
    gotoxy(x, 1);
    cout << r_b;
    for (int j = 1; j < TEXT_W - 1; j++)
        cout << hor;
    cout << l_b;
    
    gotoxy(x, 2);
        
    cout << vert << ' ';
    map.GetBlock(coords)->DrawName();
    gotoxy(x + TEXT_W - map.GetBlock(coords)->GetType().length() - 2, 2);
    cout << map.GetBlock(coords)->GetType();
    gotoxy(x + TEXT_W - 1, 2);
    cout << vert;
        
    gotoxy(x, 3);
    cout << vert_r;
    for(int i = 1; i < TEXT_W - 1; i++)
        cout << hor;
    cout << vert_l;
     
    
    gotoxy(x, 4);
    WriteText(map.GetBlock(coords)->GetDescription());
    
    getCursorPos(n, y);
        
    if (map.GetBlock(coords)->GetType() != "Storage")
    {
        gotoxy(x + 4, y + 2);
        cout << "Swap between items ('h'): ";
        gotoxy(x + 4, y + 3);
        map.GetBlock(coords)->GetCraftingInventory().Draw();
    }
    getCursorPos(n, n);
    gotoxy(x + 4, n + 2);
    cout << "Machine inventory: ";
    gotoxy(x + 4, n + 3);
    map.GetBlock(coords)->GetInventory().Draw();
    if(map.GetBlock(coords)->GetType() == "Crafting")
    {
        getCursorPos(n, n);
        gotoxy(x + 4, n + 4);
        map.GetBlock(coords)->GetCraftingInventory().GetSlot().GetRecipe().Draw();
    }
    
    while(y < TEXT_H)
    {
        gotoxy(x, y);
        cout << vert;
        gotoxy(x + TEXT_W - 1, y);
        cout << vert;
        y++;
    }
    
    getCursorPos(n, y);
    gotoxy(x + 4, y - 1);
    cout << "Facing: " << map.GetBlock(coords)->GetFacing().x << char(250) << map.GetBlock(coords)->GetFacing().y;
    gotoxy(x, y + 1);
    cout << r_t;
    for (int j = 1; j < TEXT_W - 1; j++)
        cout << hor;
    cout << l_t;
    
}
        
void Display::DrawItemInfo(Item item)
{
    int n, y, x = MAP_W + SEP_H;
    const char hor = char(196), vert = char(179),
              vert_r = char(195), vert_l = char(180),
              //cross = char(197), hor_b = char(194), hor_t = char(193),
              r_b = char(218), l_b = char(191), r_t = char(192), l_t = char(217);
              
    ClearText();
    
    gotoxy(x, 1);
    cout << r_b;
    for (int j = 1; j < TEXT_W - 1; j++)
        cout << hor;
    cout << l_b;
    
    if(item.GetId() != 0)
    {
        gotoxy(x, 2);
        
        cout << vert << ' ';
        item.DrawName();
        
        gotoxy(x + TEXT_W - 1, 2);
        cout << vert;
        
        gotoxy(x, 3);
        cout << vert_r;
        for(int i = 1; i < TEXT_W - 1; i++)
            cout << hor;
        cout << vert_l;
        
        gotoxy(x, 4);
        WriteText(item.GetDescription());
        
        getCursorPos(n, y);
        gotoxy(x + 4, y + 2);
        item.GetRecipe().Draw();
        
        while(y < TEXT_H)
        {
            gotoxy(x, y);
            cout << vert;
            gotoxy(x + TEXT_W - 1, y);
            cout << vert;
            y++;
        }
    }
    else
        for(int i = 2; i < TEXT_H; i++)
        {
            gotoxy(x, i);
            cout << vert;
            gotoxy(x + TEXT_W - 1, i);
            cout << vert;
        }
    
    getCursorPos(n, y);
    gotoxy(x, y + 1);
    cout << r_t;
    for (int j = 1; j < TEXT_W - 1; j++)
        cout << hor;
    cout << l_t;
}

void Display::ShowPauseMenu()
{
    int y, n, x = MAP_W + SEP_H;
    const char hor = char(196), vert = char(179),
              vert_r = char(195), vert_l = char(180),
              //cross = char(197), hor_b = char(194), hor_t = char(193),
              r_b = char(218), l_b = char(191), r_t = char(192), l_t = char(217);
    
    ClearText();
    
    gotoxy(x, 1);
    cout << r_b;
    for (int j = 1; j < TEXT_W - 1; j++)
        cout << hor;
    cout << l_b;
    
    gotoxy(x, 2);
    cout << vert;
    gotoxy(x + TEXT_W - 1, 2);
    cout << vert;
    gotoxy(x + 4, 2);
    cout << "Pause Menu. Select your option: ";

    gotoxy(x, 3);
    cout << vert_r;
    for(int i = 1; i < TEXT_W - 1; i++)
        cout << hor;
    cout << vert_l;
    
    gotoxy(x + 4, 5);
    cout << "h: Help";
    gotoxy(x + 4, 6);
    cout << "s: Save game";
    gotoxy(x + 4, 7);
    cout << "l: Load game";
    gotoxy(x + 4, 8);
    cout << "p: Unpause";

    for(int i = 4; i < TEXT_H; i++)
    {
        gotoxy(x, i);
        cout << vert;
        gotoxy(x + TEXT_W - 1, i);
        cout << vert;
    }

    getCursorPos(n, y);
    gotoxy(x, y + 1);
    cout << r_t;
    for (int j = 1; j < TEXT_W - 1; j++)
        cout << hor;
    cout << l_t;
}


void Display::ShowHelp()
{
    
    int y, n, x = MAP_W + SEP_H;
    const char hor = char(196), vert = char(179),
              vert_r = char(195), vert_l = char(180),
              //cross = char(197), hor_b = char(194), hor_t = char(193),
              r_b = char(218), l_b = char(191), r_t = char(192), l_t = char(217);
    
    ClearText();
    
    gotoxy(x, 1);
    cout << r_b;
    for (int j = 1; j < TEXT_W - 1; j++)
        cout << hor;
    cout << l_b;
    
    gotoxy(x, 2);
    cout << vert;
    gotoxy(x + TEXT_W - 1, 2);
    cout << vert;
    gotoxy(x + 4, 2);
    cout << "How to play: ";

    gotoxy(x, 3);
    cout << vert_r;
    for(int i = 1; i < TEXT_W - 1; i++)
        cout << hor;
    cout << vert_l;
    
    gotoxy(x + 4, 5);
    cout << "j: Break blocks";
    gotoxy(x + 4, 6);
    cout << "k: Place blocks, faces to player";
    gotoxy(x + 6, 7);
    cout << "Shift + k, faces opposite direction";
    gotoxy(x + 4, 8);
    cout << "i: Open inventory";
    gotoxy(x + 4, 9);
    cout << "e: Interact with facing block";
    gotoxy(x + 4, 10);
    cout << "l: Power facing block";
    gotoxy(x + 4, 11);
    cout << "tab: Change active inventory";
    gotoxy(x + 4, 12);
    cout << "j: In open inventory, move or craft items";
    gotoxy(x + 4, 13);
    cout << "p: Pause menu";
    gotoxy(x + 4, 15);
    cout << "Tips: ";
    gotoxy(x + 3, 16);
    cout << "- Upon powering a block, powers facing block";
    gotoxy(x + 3, 17);
    cout << "- Blocks can be powered once per side";
    gotoxy(x + 3, 18);
    cout << "- Machines can auto-output";

    for(int i = 4; i < TEXT_H; i++)
    {
        gotoxy(x, i);
        cout << vert;
        gotoxy(x + TEXT_W - 1, i);
        cout << vert;
    }

    getCursorPos(n, y);
    gotoxy(x, y + 1);
    cout << r_t;
    for (int j = 1; j < TEXT_W - 1; j++)
        cout << hor;
    cout << l_t;
    
    gotoxy(x + 4, TEXT_H - 4);
    system("pause>nul|set/p =Press any key to exit ...");
}