/****************************************************************************  
 *
 *    ****                              *                                    
 *   ******                            ***                                   
 *   *******                           ****                                  
 *   ********    ****  ****     **** *********    ******* ****    ***********
 *   *********   ****  ****     **** *********  **************  *************
 *   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
 *   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
 *  ****    *********  ****     ****   ****   ****       ****  ****      ****
 *  ****     ********  ****    *****  ****    *****     *****  ****      ****
 *  ****      ******   ***** ******   *****    ****** *******  ****** *******
 *  ****        ****   ************    ******   *************   *************
 *  ****         ***     ****  ****     ****      *****  ****     *****  ****
 *                                                                       ****
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***      
 *
 ************************************************************************//**
 *
 * @file      terminal_utils.c
 * 
 * @brief     Functions that set and get the cursor position in a terminal
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * This file is part of Nutaq's BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq's BAS Software Suite under 
 * either the terms of the GNU General Public License (GPL) or the 
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq's BAS Software Suite may be used under the terms of the GNU General 
 * Public License version 3 as published by the Free Software Foundation 
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq's BAS Software Suite is provided AS IS WITHOUT WARRANTY OF 
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq's BAS Software Suite under the GPL requires that your work based on 
 * Nutaq's BAS Software Suite, if  distributed, must be licensed under the GPL. 
 * If you wish to distribute a work based on Nutaq's BAS Software Suite but desire 
 * to license it under your own terms, e.g. a closed source license, you may purchase 
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq's BAS Software Suite. 
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/
 
#include "terminal_utils.h"
#include <stdio.h>

#ifdef __linux__

#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

int GetCursorLine()
{
    char buf[8];
    char cmd[]="\033[6n";
    int i;
    int res = -1;
    struct termios save,raw;
    tcgetattr(0,&save);
    cfmakeraw(&raw); 
    tcsetattr(0,TCSANOW,&raw);

    if (isatty(fileno(stdin))) 
    {
        write(1,cmd,sizeof(cmd));
        read (0 ,buf ,sizeof(buf));

        for(i=0; i<8 ;i++)
            if(buf[i] == ';') buf[i]='\0';

        res = atoi(buf+2);
    }
    tcsetattr(0,TCSANOW,&save);
    return res;
}

void SetCursorPosition( int column, int line )
{
    printf("\033[%d;%dH", line, column);
}

#else //Windows

#include <Windows.h>

int GetCursorLine()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD                      result;

    if (!GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi))
        return -1;

    result = csbi.dwCursorPosition;
    return result.Y;
}

void SetCursorPosition( int column, int line )
{
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

#endif