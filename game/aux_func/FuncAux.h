/******* FuncAux.h ********************************************************/ /**  
 *
 * @file FuncAux.h
 *
 * Fichero de cabecera de funciones auxiliares
 *
 * @version 1.0
 * @author Ricardo Ferris Castell
 * @date 1/03/2023
 *
 ******************************************************************************/  

#ifndef _FUNC_AUX_H_
#define _FUNC_AUX_H_

void gotoxy(int x, int y);

void textcolor(int color);

void getCursorPos(int & x, int & y);

bool InRange(int x, int bot, int top);

#endif
