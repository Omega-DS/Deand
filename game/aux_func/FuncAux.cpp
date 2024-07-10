/******* FuncAux.cpp ******************************************************/ /**
 *
 * @file FuncAux.cpp
 *
 * Implementacion de funciones auxiliares
 *
 *  - void gotoxy(int, int)
 *  - void textcolor(int)
 *
 * @version 1.0
 * @author Ricardo Ferris Castell
 * @date 01/03/2023
 *  
 ******************************************************************************/

#include <windows.h>
#include <stdio.h>
//#include <unistd.h>

/******* void gotoxy(int x, int y) ****************************************/ /**
 *
 * Esta funcion situa el cursor en la posicion (x, y) pasada como parametro
 * entendiendo que la posicion (0, 0) esta situada en la esquina superior 
 * izquierda de la consola en la que aparecen los mensajes del programa.
 * Tras la llamada a esta funcion el cout empezara a escribir desde esa posicion
 * de la pantalla.
 *
 * @param  [in]  x  Valor horizontal al que se desplaza el cursor de pantalla
 * @param  [in]  y  Valor vertical al que se desplaza el cursor de pantalla  
 *
 ******************************************************************************/
void gotoxy(int x, int y)
{
    HANDLE hcon;  
    COORD dwPos;  
        
    hcon = GetStdHandle(STD_OUTPUT_HANDLE);  
    dwPos.X = x;  
    dwPos.Y= y;  
    
    SetConsoleCursorPosition(hcon,dwPos);
}

/******* void textcolor(int color) ****************************************/ /**
 *
 * Esta funcion cambia el color de fondo y letra que se escriba
 *
 * @param  [in]  color  Valor del color y fondo a utilizar
 *
 ******************************************************************************/
void textcolor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // pick the colorattribute colot you want
    SetConsoleTextAttribute(hConsole, color);
    
    return;
}

void getCursorPos(int & x, int & y)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    
    GetConsoleScreenBufferInfo(hConsole, &cbsi);
    
    x = cbsi.dwCursorPosition.X;
    y = cbsi.dwCursorPosition.Y;
    
}


bool InRange(int x, int bot, int top)
{
    return bot <= x && x <= top;
}