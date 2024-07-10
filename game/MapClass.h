#pragma once


#include "TileFwdClass.h"
#include "blocks/BlockClass.h"
using namespace std;

class Map
{
    public:
        Map();
        int GetSize() const;
        bool InMap(Coords) const;
        Tile* GetBlock(Coords) const;
        void SetBlock(Coords, Block);
        void ResetPowered();
        static const int MAX_MAP = 200;
        
        Tile* tiles[MAX_MAP][MAX_MAP];
    private:
};