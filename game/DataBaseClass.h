#pragma once

#include "blocks/BlockClass.h"
#include "items/ItemClass.h"
using namespace std;

class DataBase
{
    public:
        DataBase();
        Item SearchItem(int) const;
        Block SearchBlock(int) const;
        unsigned int GetSize() const;
        
    private:
        vector<Item> items;
        vector<Block> blocks;
        
        const string ITEMS_FILE_PATH = "assets/items.txt";
        const string BLOCKS_FILE_PATH = "assets/blocks.txt"; 
};