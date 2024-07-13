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
#include <windows.h>
#include <mmsystem.h>
using namespace std;

const int WIDTH = 49;
const int HEIGHT = 25;
const string FILE_PATH = "save_file.bin";

const string INV_SOUND = "assets/inv_nav.wav";
const string PLACE_SOUND = "assets/placing.wav";
const string MENU_MUSIC = "assets/menu_music.wav";
const string TYPE = "assets/typing_soft.wav";

void MainLoop(Player player, bool &);
bool InvLoop(Display &, Player &, Inventory &, bool &, bool &);
bool interactedLoop(Display &, Player &, Map &, Coords, bool &);
bool PauseLoop(Display & display, Map & map, Player & player);
void SaveGame(Map & map, Player & player);
void LoadGame(Map & map, Player & player);
void Play(string);
void Print(string s);
void Intro();
void AjustZoom();
void MainMenu(bool &);
bool FirstTime();
void End();
void PrintLine(char, int, bool fill = false);

int main()
{
    Player player;
    bool load;
    
    sleep(1);
    
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE); 
    system("mode 650");
    
    AjustZoom();
    
    
    if(FirstTime())
    {
        Intro();
    }
    
    system("cls");
    
    MainMenu(load);
    system("cls");
    
    MainLoop(player, load);
    
    End();
    
    return 0;
}

void MainLoop(Player player, bool & load)
{
    DataBase data_base;
    Inventory inv(32);
    Display display;
    Map map;
    char opt;
    bool crafting, win = false;
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
    for(int i = 1; i < 40; i++ )
    {
        item = data_base.SearchItem(i);
        item.SetQuant(15);
        player.inv.PutIn(item);
    }*/
    
    if(load)
        LoadGame(map, player);
        
    display.Initialize();
    display.UpdateMap(map, player.GetCoords());
    display.UpdateInv(player);
    if(FirstTime())
    {
        gotoxy(60 , 6);
        cout << "Press p to open pause menu";
    }
    
    
    while(!win)
    {
        gotoxy(WIDTH/2 + player.GetFacingUnary().x, HEIGHT/2 + player.GetFacingUnary().y);
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
                Play(INV_SOUND);
                while(PauseLoop(display, map, player));
                Play(INV_SOUND);
                display.ClearText();
                break;
            case 'i':
            case 'I':
                crafting = false;
                Play(INV_SOUND);
                while(InvLoop(display, player, craft_inv, crafting, win));
                Play(INV_SOUND);
                crafting = false;
                display.ClearText();
                display.ClearMenu();
                break;
            case 'k':
            case 'K':
                item = player.inv.GetSlot();
                if(item.GetPlaced() && map.GetBlock(player.GetFacing())->GetId() == 0 && map.InMap(player.GetFacing()))
                {
                    Play("assets/placing.wav");
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
                    Play(INV_SOUND);
                    while(interactedLoop(display, player, map, player.GetFacing(), crafting));
                    Play(INV_SOUND);
                    crafting = false;
                    map.GetBlock(player.GetFacing())->GetInventory().ResetIndex();
                    display.ClearText();
                }
                break;
            case 'l':
            case 'L':
                coords = player.GetFacing();
                Play("assets/electric.wav");
                if(map.InMap(coords))
                {
                    map.tiles[coords.x][coords.y]->Power(map, display, player.GetCoords(), coords);
                    map.ResetPowered();
                    display.UpdateMap(map, player.GetCoords());
                }
                break;
        }
    }
}

bool interactedLoop(Display & display, Player & player, Map & map, Coords coords, bool & menu)
{
    char opt;
    int n;
    Item item;
    display.DrawBlockInfo(map, coords);
    
    
    gotoxy(WIDTH/2 + player.GetFacingUnary().x, HEIGHT/2 + player.GetFacingUnary().y);
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
                Play(INV_SOUND);
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
                Play(INV_SOUND);
                menu = !menu;
                break;
        }
    return opt != 'e';
}

bool InvLoop(Display & display, Player & player, Inventory & craft_inv, bool & crafting, bool & win)
{
    int n;
    char opt;
    
    if(!crafting)
        display.DrawItemInfo(player.inv.GetSlot());
    else
        display.DrawItemInfo(craft_inv.GetSlot());
    display.UpdateCrafting(craft_inv);
    
    gotoxy(WIDTH/2 + player.GetFacingUnary().x, HEIGHT/2 + player.GetFacingUnary().y);
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
                Play(INV_SOUND);
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
                {
                    player.inv = inv_a;
                    display.UpdateInv(player);
                    sleep(0.8);
                    if (item_a.GetId() == 43)
                        win = true;
                }
                
                break;
            case '\t':
                Play(INV_SOUND);
                crafting = !crafting;
                break;
        }
    }
    
    return opt != 'i';
}

bool PauseLoop(Display & display, Map & map, Player & player)
{
    char opt;
    
    display.ShowPauseMenu();

    gotoxy(WIDTH/2 + player.GetFacingUnary().x, HEIGHT/2 + player.GetFacingUnary().y);
    while(kbhit()) getch();         // Clear buffer
    opt = getch();
    opt = tolower(opt);
    
    switch(opt)
    {
        case 'h':
            display.ShowHelp();
            break;
        case 's':
            SaveGame(map, player);
            gotoxy(60, 24);
            cout << "Saving game";
            sleep(1); cout << " .";
            sleep(1); cout << " .";
            sleep(1); cout << " .";
            gotoxy(60, 24);
            break;
        case 'l':
            LoadGame(map, player);
            gotoxy(60, 24);
            cout << "Loading game";
            sleep(1); cout << " .";
            sleep(1); cout << " .";
            sleep(1); cout << " .";
            gotoxy(60, 24);                                                         
            display.UpdateMap(map, player.GetCoords());
            display.UpdateInv(player);
            break;
    }
    
    return opt != 'p';
}

void SaveGame(Map & map, Player & player)
{
    ofstream f;
    Coords c = player.GetCoords();
    int id, q;
    
    f.open(FILE_PATH, ios::binary);
    
    if(f)
    {
        f.write((char*)(&c.x), sizeof(int));
        f.write((char*)(&c.y), sizeof(int));
        
        for(int i = 0; i < player.inv.GetSize(); i++)
        {
            id = player.inv.cont[i].GetId();
            q = player.inv.cont[i].GetQuant();
            f.write((char*)&id, sizeof(int));
            f.write((char*)&q, sizeof(int));
        }
        
        for(int i = 0; i < map.GetSize(); i++)
            for(int j = 0; j < map.GetSize(); j++)
            {
                id = map.tiles[i][j]->GetId();
                c.x = map.tiles[i][j]->GetFacing().x;
                c.y = map.tiles[i][j]->GetFacing().y;
                f.write((char*)&id, sizeof(int));
                f.write((char*)(&c.x), sizeof(int));
                f.write((char*)(&c.y), sizeof(int));
                
                for(int k = 0; k < map.tiles[i][j]->GetInventory().GetSize(); k++)
                {
                    id = map.tiles[i][j]->GetInventory().GetSlot(k).GetId();
                    q = map.tiles[i][j]->GetInventory().GetSlot(k).GetQuant();
                    f.write((char*)&id, sizeof(int));
                    f.write((char*)&q, sizeof(int));
                }
            }
    }
    
    f.close();
}

void LoadGame(Map & map, Player & player)
{
    ifstream f;
    DataBase data_base;
    Coords c;
    Block block;
    int id, q;
    Item item;
    
    f.open(FILE_PATH, ios::binary);
    
    if(f)
    {
        f.read((char*)(&c.x), sizeof(int));
        f.read((char*)(&c.y), sizeof(int));
        player.SetCoords(c);
        
        
        for(int i = 0; i < player.inv.GetSize(); i++)
        {
            f.read((char*)&id, sizeof(int));
            f.read((char*)&q, sizeof(int));
               
            item = data_base.SearchItem(id);
            item.SetQuant(q);
            player.inv.SetSlot(item, i);
        }
        
        
        for(int i = 0; i < map.GetSize(); i++)
            for(int j = 0; j < map.GetSize(); j++)
            {
                f.read((char*)&id, sizeof(int));
                f.read((char*)(&c.x), sizeof(int));
                f.read((char*)(&c.y), sizeof(int));
                
                block = data_base.SearchBlock(id);
                block.SetFacing(c);
                
                if(block.GetType() == "Production" || block.GetType() == "Crafting")
                {
                    Producer *p;
                    p = new Producer(block);
                    
                    delete map.tiles[i][j];
                    map.tiles[i][j] = p;
                }
                else if(block.GetType() == "Storage")
                {
                    Chest *p;
                    p = new Chest(block);
                    
                    delete map.tiles[i][j];
                    map.tiles[i][j] = p;
                }
                else if(block.GetType() == "Power")
                {
                    Coords c_a;
                    c_a.x = i;
                    c_a.y = j;
                    block.SetFacing(c_a);
                    Bouncer *p;
                    p = new Bouncer(block);
                    
                    delete map.tiles[i][j];
                    map.tiles[i][j] = p;
                }
                else if(block.GetType() == "Logistics")
                {
                    Coords c_a;
                    c_a.x = i;
                    c_a.y = j;
                    Pipe *p;
                    p = new Pipe(block);
                    
                    delete map.tiles[i][j];
                    map.tiles[i][j] = p;
                    
                    map.tiles[i][j]->CalculateFaces(map, c_a, true);
                }
                else
                {
                    Block *p;
                    p = new Block();
                    
                    delete map.tiles[i][j];
                    map.tiles[i][j] = p;
                }
                
                for(int k = 0; k < map.tiles[i][j]->GetInventory().GetSize(); k++)
                {
                    f.read((char*)&id, sizeof(int));
                    f.read((char*)&q, sizeof(int));
                    
                    item = data_base.SearchItem(id);
                    item.SetQuant(q);
                    if(id != 0)
                        map.tiles[i][j]->PutInInv(item);
                }
            }
            
        //Map map_aux;
        //map = map_aux;
    }
    
    f.close();
}

void Intro()
{
    int x = 15, y = 5;
    
    system("cls");
    textcolor(2);
    gotoxy(x - 2, y);
    sleep(2);
    Print("- ");
    Print("Year 2898 -");
    sleep(2);
    gotoxy(x, y + 2);
    Print("Mankind has reached an interplanetary phase, with 17 planets colonized, called Earths.");
    sleep(1);
    gotoxy(x, y + 3);
    Print("Old ways of power generation have been replaced by valmite. A rare mineral found on RT-17, mankind 4th Earth.");
    gotoxy(x, y + 4);
    Print("It is said that a small portion can power several spaceships at once.");
    gotoxy(x, y + 6);
    sleep(2);
    Print("You are a humble engineer in 4th Earth, who discovered by accident a project from the elites to start a war over valmite's possesion.");
    gotoxy(x, y + 7);
    Print("You tried to investigate more. However, all of a sudden, you were transfered to 5th Earth, along with your wife and daughter.");
    gotoxy(x, y + 9);
    sleep(3);
    Print("In the middle of your trip, a message arrives to the spaceship communicaton system: ");
    gotoxy(x + 4, y + 10);
    Print("'You should have kept yourself out of this. This is what you deserve.'");
    sleep(4);
    gotoxy(x, y + 11);
    Print("As the message ends, the ship abruptly auto-reroutes to a nearby planet. Your daughter and wife cry.");
    sleep(5);
    gotoxy(x, y + 13);
    Print("Sun in your nose, sand in your shoes.");
    sleep(2);
    gotoxy(x, y + 15);
    Print("You woke up next to a crashed ship. No survivors other than you.");
    gotoxy(x, y + 16);
    Print("You take valmite's power cube from the ship and install it in your right glove.");
    sleep(1);
    gotoxy(x, y + 17);
    Print("Everywhere you look is just sand. A vast flat desert. Welcome to UL-11, also known as...");
    
    sleep(4);
    gotoxy(x, y + 22);
    system("pause>nul|set/p =Press any key to continue ...");
    
    
}

void AjustZoom()
{
    gotoxy(20, 5);
    cout << "Zoom until you do not see '+' signs, but you see '*' signs (ctrl + wheel)";
    gotoxy(180, 40);
    cout << "+++";
    gotoxy(180, 41);
    cout << "+++";
    gotoxy(180, 42);
    cout << "+++";
    gotoxy(170, 40);
    cout << "***";
    gotoxy(170, 41);
    cout << "***";
    gotoxy(170, 42);
    cout << "***";
    gotoxy(20, 10);
    sleep(2);
    system("pause>nul|set/p =Press any key to continue ...");
    textcolor(15);
}

void MainMenu(bool & load)
{
    int x = 60, y = 5;
    int time = 438;
    char c = char (177), opt = '0';
    bool first_time = FirstTime();
    
    PlaySound(TEXT("assets/menu_music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    textcolor(6);
    gotoxy(x - 4, y - 1);
    cout << "=========================================================";
    Sleep(time);
    gotoxy(x, y);
    cout << "######     ########    ###     ###   ##  ######";
    Sleep(time);
    gotoxy(x, y + 1);
    cout << "##    ##   ##         ## ##    ####  ##  ##    ##";
    Sleep(time);
    gotoxy(x, y + 2);
    cout << "##     ##  #####     ##   ##   ## ## ##  ##     ##";
    Sleep(time);
    gotoxy(x, y + 3);
    cout << "##     ##  ##       #########  ##  ####  ##     ##";
    Sleep(time);
    gotoxy(x, y + 4);
    cout << "##    ##   ##       ##     ##  ##   ###  ##    ##";
    Sleep(time);
    gotoxy(x, y + 5);
    cout << "######     #######  ##     ##  ##    ##  ######";
    gotoxy(x - 4, y + 6);
    Sleep(time);
    cout << "=========================================================";
    textcolor(15);
    
    Sleep(time);
    gotoxy(x + 15, y + 10);
    cout << "1. - New Game -";
    gotoxy(x + 15, y + 11);
    Sleep(time);
    textcolor(15 - (7 * first_time));
    cout << "2. - Load Game -";
    
    textcolor(6);
    gotoxy(0, y + 27);
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 200; j++)
            if(abs(j - x - 24) < 50)
                cout << ' ';
            else
                cout << c;
        if(i % 2 == 0)
            Sleep(time);
        cout << endl;
    }
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 200; j++)
            if(abs(j - x - 24) < 20)
                cout << ' ';
            else
                cout << c;
        if(i % 2 == 0)
            Sleep(time);
        cout << endl;
    }
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 200; j++)
            cout << c;
        cout << endl;
        if(i % 2 == 0)
            Sleep(time);
    }
    textcolor(15);
    gotoxy(x + 22, y + 13);
    while(kbhit()) getch();         // Clear buffer
    while(opt != '1' && (opt != '2' || first_time))
        opt = getch();
    
    load = opt == '2';
    
    PlaySound(0, 0, 0);
}

void End()
{
    int x = 85, y = 20;
    char first = char(176),
         second = char(177),
         third = char(178),
         forth = char(219);
    
    textcolor(2);
    
    system("cls");
    gotoxy(x - 2, y);
    Print("Sorry");
    sleep(4);
    system("cls");
    gotoxy(x - 4, y);
    Print("I lied");
    sleep(3);
    system("cls");
    gotoxy(x - 24, y);
    Print("This is not a spaceship. It is a valmite supression canon");
    sleep(1);
    system("cls");
    gotoxy(x - 40, y);
    Print("It will delete the valmite cube to release every single drop of power it contains");
    sleep(1);
    system("cls");
    gotoxy(x - 19, y);
    Print("I was never meant to leave this planet");
    sleep(3);
    
    system("cls");
    sleep(3);
    Play("assets/lasergun.wav");
    textcolor(11);
    gotoxy(0, 20);
    for(int i = 1; i < 29; i += 2)
    {
        PrintLine(first, i);
        PrintLine(second, i - 2);
        PrintLine(third, i - 4);
        PrintLine(forth, i - 6, true);
        Sleep(2000 / (i*i*i));
    }
    system("cls");
    Play("assets/explosion.wav");
    sleep(5);
    
    x = 60;
    y = 5;
    PlaySound(TEXT("assets/menu_music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    textcolor(6);
    gotoxy(x - 4, y - 1);
    cout << "=========================================================";
    gotoxy(x, y);
    cout << "######     ########    ###     ###   ##  ######";
    gotoxy(x, y + 1);
    cout << "##    ##   ##         ## ##    ####  ##  ##    ##";
    gotoxy(x, y + 2);
    cout << "##     ##  #####     ##   ##   ## ## ##  ##     ##";
    gotoxy(x, y + 3);
    cout << "##     ##  ##       #########  ##  ####  ##     ##";
    gotoxy(x, y + 4);
    cout << "##    ##   ##       ##     ##  ##   ###  ##    ##";
    gotoxy(x, y + 5);
    cout << "######     #######  ##     ##  ##    ##  ######";
    gotoxy(x - 4, y + 6);
    cout << "=========================================================";
    textcolor(15);
    sleep(2);
    gotoxy(x + 15, y + 10);
    cout << "made";
    sleep(1);
    cout << " by";
    sleep(1);
    cout << " Omega";
    gotoxy(0, 0);
    system("pause>nul|set/p =");
    system("pause>nul|set/p =");
}

void PrintLine(char c, int n, bool fill)
{
    if(n > 0)
    {
        if (fill)
            for(int i = 0; i < n/2; i++)
            {
                gotoxy(0, 20 - i);
                for(int i = 0; i < 200; i++)
                    cout << c;
                gotoxy(0, 20 + i);
                for(int i = 0; i < 200; i++)
                    cout << c;
            }
        else
        {
                gotoxy(0, 20 - n/2);
                for(int i = 0; i < 200; i++)
                    cout << c;
                gotoxy(0, 20 + n/2);
                for(int i = 0; i < 200; i++)
                    cout << c;
        }
    }
}

bool FirstTime()
{
    ifstream f;
    bool ok = false;
    
    f.open(FILE_PATH, ios::binary);
    if(!f)
        ok = true;
    f.close();
    
    return ok;
}

void Print(string s)
{
    Play(TYPE);
    for(unsigned int i; i < s.length(); i++)
    {
        cout << s[i];
        Sleep(40);
    }
    PlaySound(0,0,0);
}

void Play(string s)
{
    char a[s.length()];
    for(unsigned int i = 0; i < s.length(); i++)
        a[i] = s[i];
    PlaySound(TEXT(a), NULL, SND_FILENAME | SND_ASYNC);
}