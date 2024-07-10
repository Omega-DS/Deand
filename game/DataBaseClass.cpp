#include "DataBaseClass.h"
#include <algorithm>
using namespace std;


DataBase::DataBase()
{
    ifstream items_file, blocks_file;
    Item item;
    Block block;
    
    items_file.open(ITEMS_FILE_PATH);
    
    while(item.ReadFromFile(items_file))
        items.push_back(item);

    items_file.close();
    
    blocks_file.open(BLOCKS_FILE_PATH);
    
    while(block.ReadFromFile(blocks_file))
        blocks.push_back(block);
        
    blocks_file.close();
}

Item DataBase::SearchItem(int id) const
{
    return items[id];
}

Block DataBase::SearchBlock(int id) const
{
    return blocks[id];
}

unsigned int DataBase::GetSize() const
{
    return items.size();
}
