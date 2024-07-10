#include "RecipeClass.h"
#include "globals/database.h"
#include <iostream>
using namespace std;

Recipe::Recipe()
{}

Recipe::Recipe(vector<Ingredient> v)
{
    ingredients = v;
}


Ingredient Recipe::GetIngredient(int i)
{
    return ingredients[i];
}
unsigned int Recipe::GetSize() const
{
    return ingredients.size();
}

void Recipe::Draw()
{
    int x, y;
    DataBase database;
    
    getCursorPos(x, y);
    if(ingredients.size() > 0)
        cout << "Ingredients: ";
    gotoxy(x, y + 1);
    for(unsigned int i = 0; i < ingredients.size(); i++)
    {
        getCursorPos(x, y);
        cout << "  - ";
        database.SearchItem(ingredients[i].id).DrawName();
        cout << ' ';
        database.SearchItem(ingredients[i].id).DrawChar();
        cout << " x" << ingredients[i].quant;
        gotoxy(x, y + 1);
    }
}