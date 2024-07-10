#pragma once

#include "MapClass.h"
using namespace std;
        
class Player
{
    public:
        Player();
        Coords GetCoords() const;
        Coords GetFacing() const;
        char GetChar() const;
        bool Move(const Map &);
        void SetFacing(Coords);
        const int INV_SIZE = 40;
        Inventory inv{INV_SIZE};
        
    private:
        Coords coords;
        Coords facing;
        string name;
        char ch;
};
