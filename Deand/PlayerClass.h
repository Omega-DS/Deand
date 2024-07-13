#pragma once

#include "MapClass.h"
using namespace std;
        
class Player
{
    public:
        Player();
        Coords GetCoords() const;
        Coords GetFacing() const;
        Coords GetFacingUnary() const;
        char GetChar() const;
        void SetCoords(Coords);
        bool Move(const Map &);
        void SetFacing(Coords);
        const int INV_SIZE = 60;
        Inventory inv{INV_SIZE};
        
    private:
        Coords coords;
        Coords facing;
        string name;
        char ch;
};
