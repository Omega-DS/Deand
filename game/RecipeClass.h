#include <string>
#include <vector>
#include "aux_func/FuncAux.h"
using namespace std;

#ifndef _RECIPE_H_
#define _RECIPE_H_

struct Ingredient
{
    int id, quant;  
};

class Recipe
{
    public:
        Recipe();
        Recipe(vector<Ingredient>);
        void Draw();
        Ingredient GetIngredient(int);
        unsigned int GetSize() const;
        
    private:
        vector<Ingredient> ingredients;
};

#endif