#pragma once

#include "PlayerClass.h"
#include "blocks/BlockClass.h"
using namespace std;

class Display
{
    public:
        Display();
        void Initialize();
        void UpdateMap(const Map, Coords);
        void UpdateInv(Player &);
        void WriteText(string, int color = 15);
        void ClearText();
        void UpdateCrafting(Inventory &);
        void ClearMenu();
        
        void DrawItemInfo(Item);
        void DrawBlockInfo(Map & map, Coords coords);
        
        
    private:
        
        static const int MAP_W = 49;
        static const int MAP_H = 25;
        static const int TEXT_W = 50;
        static const int TEXT_H = 32;
        static const int MENU_W = 50;
        static const int MENU_H = 32;
        static const int SEP_H = 6;
        static const int SEP_V = 2;
        
        bool is_empty;
};