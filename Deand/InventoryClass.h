#ifndef _INVENTORYCLASS_H_
#define _INVENTORYCLASS_H_

#include "items/ItemClass.h"
using namespace std;

class Inventory
{
    
    public:
        Inventory();
        Inventory(int);
        Inventory(Recipe);
        int GetSize() const;
        Item GetSlot(int i = -1);
        void SetSlot(Item, int i = -1);
        void DelSlot();
        bool IsEmpty();
        void MoveIndex(int);
        void ResetIndex();
        void Draw();
        bool SearchItem(int id, int & pos, int & quant, int starting_pos = 0) const;
        bool PutIn(Item &);
        bool RecipeCheck(Recipe);
        void ClearEmpty();
        static const int MAX_ROW = 20;
        vector<Item> cont;
        
    private:
        int size, ind;
        
};

#endif