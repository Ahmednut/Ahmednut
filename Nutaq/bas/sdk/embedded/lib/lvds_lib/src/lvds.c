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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       lvds.c
 * @brief      Initialize and configure the FMC LVDS card
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup LVDS
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
#include <stdio.h>
#include "lvds.h"
#include "lvds_registers.h"
#include "lvds_defines.h"
#include "bas_debug.h"

Result lvds_open(lvds * board, lvds_core * core, int (*pfUsleep)(unsigned int usec))
{
    board->core = core;
    board->pfUsleep = pfUsleep;
  
    if (board->core->coreversid.coreid == LVDS_GPIO_CORE_ID)
        board->mode = LVDS_GPIO;
    else if (board->core->coreversid.coreid == LVDS_SYNC_CORE_ID)
        board->mode = LVDS_SYNC;
    else
    {
        board->mode = LVDS_NONE;
        return LVDSABSENT;
    }
    
    lvds_reset(board);

    return LVDSOK;
}

Result lvds_presence(lvds * board)
{
    if (board->mode == LVDS_NONE)
        return LVDSABSENT;
    else
        return LVDSOK;
}

Result lvds_getmode(lvds * board)
{
    return board->mode;
}

Result lvds_setoutputenable(lvds * board, unsigned int outputenable)
{
    if (board->mode == LVDS_NONE)
        return LVDSABSENT;
        
    if (board->mode == LVDS_GPIO)
    {
        board->core->outputenable = outputenable;
    }
    else if(board->mode == LVDS_SYNC)
    {
        // Only accept 0 or 1 values for synchronous mode
        if (outputenable >= 2)
            return LVDSIVAL;
            
        board->core->outputenable = outputenable;
    }
    
    return LVDSOK;
}

Result lvds_setvalue(lvds * board, unsigned int value)
{
    if (board->mode == LVDS_NONE)
        return LVDSABSENT;
        
    if (board->mode != LVDS_GPIO)
        return LVDSIMODE;
        
    board->core->value = value;
    
    return LVDSOK;
}

Result lvds_getvalue(lvds * board, unsigned int *value)
{
    if (board->mode == LVDS_NONE)
        return LVDSABSENT;
        
    if (board->mode != LVDS_GPIO)
        return LVDSIMODE;
        
    *value = board->core->value;
    
    return LVDSOK;
}

Result lvds_reset(lvds * board)
{
    if (board->mode == LVDS_NONE)
        return LVDSABSENT;
        
    board->core->outputenable = 0;
    board->core->value = 0;

    board->core->control.CoreReset = 1;
    board->pfUsleep(1);
    board->core->control.CoreReset = 0;
    
    return LVDSOK;
}

Result lvds_reset_fifo(lvds * board)
{
    if (board->mode == LVDS_NONE)
        return LVDSABSENT;
        
    if (board->mode != LVDS_SYNC)
        return LVDSIMODE;

    board->core->control.CoreReset = 1;
    board->pfUsleep(1);
    board->core->control.CoreReset = 0;
    
    return LVDSOK;
}

Result lvds_close(lvds * board)
{
    if (board->mode != LVDS_NONE)
    {
        board->core->outputenable = 0;
        board->core->value = 0;
        board->core->control.CoreReset = 1;
    }

    board->core = NULL;
    board->mode = LVDS_NONE;
    
    return LVDSOK;
}

