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
 *  @file       : adac250_mux.c
 *  @brief      : ADAC250 configuration functions API for the clock multiplexer
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup ADAC250
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
#include "adac250.h"
#include "adac250_user.h"
#include "adac250_types.h"
#include "bas_error.h"

// Point the the pfADAC250_usleep function pointer define in the adac250.c
extern int (*pfADAC250_usleep)(unsigned int usec);

Result  ADAC250_MUX_ConfigClockOutput(ADAC250Board_t * ADAC250Board, ADAC250_CLOCK_OUTPUT_ID ClockToConfig, ADAC250_CLOCK_INPUT_ID ClockInputSelection)
{
    int InVal, OutVal;

    switch (ClockToConfig)
    {
        case ADAC250_CLOCKOUT_REFOUT:
            OutVal = 0;
            break;

        case ADAC250_CLOCKOUT_PLLCLK1:
            OutVal = 1;
            break;

        case ADAC250_CLOCKOUT_PLLREF:
            OutVal = 2;
            break;

        case ADAC250_CLOCKOUT_FMCCLK1:
            OutVal = 3;
            break;

        default:
            return -1;
    }

    switch (ClockInputSelection)
    {
        case ADAC250_CLOCKIN_10MHZ:
            InVal = 0;
            break;

        case ADAC250_CLOCKIN_REFIN:
            InVal = 1;
            break;

        case ADAC250_CLOCKIN_PLLCLKOUT2:
            InVal = 2;
            break;

        case ADAC250_CLOCKIN_FMCCLK3:
            InVal = 3;
            break;

        default:
            return -1;
    }


    ADAC250Board->Adaptor->Misc_Control.Register &= (~(MISC_CTRL_CLK_MUX_ALL_BITS));

    ADAC250Board->Adaptor->Misc_Control.Register |= ((InVal<<4)&0x30) | ((OutVal<<6)&0xc0);

    ADAC250Board->Adaptor->Misc_Control.Register |= MISC_CTRL_CLK_MUX_LOAD;
    ADAC250Board->Adaptor->Misc_Control.Register &= ~MISC_CTRL_CLK_MUX_LOAD;

    ADAC250Board->Adaptor->Misc_Control.Register |= MISC_CTRL_CLK_MUX_CONFIG;
    ADAC250Board->Adaptor->Misc_Control.Register &= ~MISC_CTRL_CLK_MUX_CONFIG;

    //let clock settle
    pfADAC250_usleep(1);

    return 0;
}

Result ADAC250_ConfigAllClockMuxOutput(ADAC250Board_t * ADAC250Board, ADAC250_CLOCK_INPUT_ID ClockOutputConfig [])
{
    int idx;

    for(idx = 0; idx < 4; idx ++)
    {
        if(ADAC250_MUX_ConfigClockOutput(ADAC250Board,
                                    (ADAC250_CLOCK_OUTPUT_ID) idx,
                                    ClockOutputConfig[idx]))
            return -1;
    }

    return 0;
}

