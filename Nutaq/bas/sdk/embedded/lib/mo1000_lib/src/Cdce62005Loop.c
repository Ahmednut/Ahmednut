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
 * @file       Cdce62005Loop.c
 * @brief      Enhanced cdce62005 module loop filter definitions
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup MO1000
 *
 * This file is part of Nutaq’s BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq’s BAS Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq’s BAS Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq’s BAS Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq’s BAS Software Suite under the GPL requires that your work based on
 * Nutaq’s BAS Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq’s BAS Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq’s BAS Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/
// WARNING: This file must be included inside Cdce62005.c module to compile correctly
//          It could be tweaked to provide different loop filter configuration according to the needs.

// Maximum number of pre-calculated loop filter setup
#define CDCE62005_MAXLOOPFILT (32)

// These 3 vectors are related to form a complete loopfilter configuration: each element index in each
// table is related to the same element index in other tables

// The filter configurations here are tuned to around 100 Khz loop filter bandwidth, and maximizes the pfd
// attenuation spurs
 
// Synthetized frequency
// !!! M.T. Warning, this code assumes that there are no synth freq entries that are double in the table
// The tables must be sorted according to ascending synthetized frequency
const unsigned Cdce62005_auLoopFilterConfigSyntFrq[CDCE62005_MAXLOOPFILT] = {
393750000,
400000000,
420000000,
432000000,
600000000,
625000000,
630000000,
640000000,
672000000,
675000000,
700000000,
720000000,
735000000,
750000000,
768000000,
784000000,
875000000,
896000000,
900000000,
918750000,
937500000,
960000000,
980000000,
1000000000,
1008000000,
1024000000,
1050000000,
1080000000,
1120000000,
1125000000,
1152000000,
1176000000
};

// Combined (Feedback * bypass) dividers
const unsigned short Cdce62005_ausLoopFilterConfigCFbDiv[CDCE62005_MAXLOOPFILT] = {
252,
16,
84,
432,
24,
20,
252,
128,
672,
108,
56,
144,
588,
24,
768,
784,
56,
896,
36,
588,
60,
192,
392,
32,
1008,
1024,
84,
216,
224,
36,
1152,
1176
};

// Internal setup for the loop filter (CP,C1,R2,C2,R3,C3)
const unsigned char Cdce62005_aucLoopFilterConfigParts[CDCE62005_MAXLOOPFILT][6] = {
{eCdce62005ChargePump750D0UA,eCdce62005C1Val16D5PF,eCdce62005R2Val29D0KO,eCdce62005C2Val148D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump100D0UA,eCdce62005C1Val23D0PF,eCdce62005R2Val13D0KO,eCdce62005C2Val461D0PF,eCdce62005R3Val5D0KO,eCdce62005C3Val27D5PF},
{eCdce62005ChargePump100D0UA,eCdce62005C1Val6D5PF,eCdce62005R2Val51D2KO,eCdce62005C2Val112D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump400D0UA,eCdce62005C1Val6D5PF,eCdce62005R2Val65D4KO,eCdce62005C2Val87D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump50D0UA,eCdce62005C1Val11D5PF,eCdce62005R2Val24D0KO,eCdce62005C2Val251D0PF,eCdce62005R3Val5D0KO,eCdce62005C3Val27D5PF},
{eCdce62005ChargePump50D0UA,eCdce62005C1Val16D5PF,eCdce62005R2Val20D0KO,eCdce62005C2Val263D5PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump200D0UA,eCdce62005C1Val6D5PF,eCdce62005R2Val60D4KO,eCdce62005C2Val87D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump100D0UA,eCdce62005C1Val6D5PF,eCdce62005R2Val60D4KO,eCdce62005C2Val87D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump600D0UA,eCdce62005C1Val6D5PF,eCdce62005R2Val42D2KO,eCdce62005C2Val148D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump100D0UA,eCdce62005C1Val8D0PF,eCdce62005R2Val42D2KO,eCdce62005C2Val124D5PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump50D0UA,eCdce62005C1Val6D5PF,eCdce62005R2Val42D2KO,eCdce62005C2Val148D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump100D0UA,eCdce62005C1Val6D5PF,eCdce62005R2Val56D4KO,eCdce62005C2Val87D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump150D0UA,eCdce62005C1Val21D5PF,eCdce62005R2Val13D0KO,eCdce62005C2Val436D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump50D0UA,eCdce62005C1Val23D0PF,eCdce62005R2Val20D0KO,eCdce62005C2Val210D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump600D0UA,eCdce62005C1Val6D5PF,eCdce62005R2Val47D2KO,eCdce62005C2Val124D5PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump600D0UA,eCdce62005C1Val6D5PF,eCdce62005R2Val47D2KO,eCdce62005C2Val135D5PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump150D0UA,eCdce62005C1Val18D0PF,eCdce62005R2Val13D0KO,eCdce62005C2Val461D0PF,eCdce62005R3Val5D0KO,eCdce62005C3Val27D5PF},
{eCdce62005ChargePump600D0UA,eCdce62005C1Val6D5PF,eCdce62005R2Val47D2KO,eCdce62005C2Val123D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump50D0UA,eCdce62005C1Val11D5PF,eCdce62005R2Val24D0KO,eCdce62005C2Val251D0PF,eCdce62005R3Val5D0KO,eCdce62005C3Val27D5PF},
{eCdce62005ChargePump400D0UA,eCdce62005C1Val16D5PF,eCdce62005R2Val47D2KO,eCdce62005C2Val123D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump150D0UA,eCdce62005C1Val21D5PF,eCdce62005R2Val13D0KO,eCdce62005C2Val436D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump150D0UA,eCdce62005C1Val8D0PF,eCdce62005R2Val42D2KO,eCdce62005C2Val124D5PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump200D0UA,eCdce62005C1Val6D5PF,eCdce62005R2Val60D4KO,eCdce62005C2Val87D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump50D0UA,eCdce62005C1Val16D5PF,eCdce62005R2Val20D0KO,eCdce62005C2Val313D0PF,eCdce62005R3Val5D0KO,eCdce62005C3Val27D5PF},
{eCdce62005ChargePump600D0UA,eCdce62005C1Val6D5PF,eCdce62005R2Val42D2KO,eCdce62005C2Val148D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump600D0UA,eCdce62005C1Val6D5PF,eCdce62005R2Val42D2KO,eCdce62005C2Val148D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump50D0UA,eCdce62005C1Val6D5PF,eCdce62005R2Val42D2KO,eCdce62005C2Val148D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump150D0UA,eCdce62005C1Val8D0PF,eCdce62005R2Val38D2KO,eCdce62005C2Val135D5PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump150D0UA,eCdce62005C1Val8D0PF,eCdce62005R2Val38D2KO,eCdce62005C2Val148D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump50D0UA,eCdce62005C1Val18D0PF,eCdce62005R2Val20D0KO,eCdce62005C2Val247D5PF,eCdce62005R3Val5D0KO,eCdce62005C3Val27D5PF},
{eCdce62005ChargePump600D0UA,eCdce62005C1Val6D5PF,eCdce62005R2Val47D2KO,eCdce62005C2Val124D5PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF},
{eCdce62005ChargePump600D0UA,eCdce62005C1Val6D5PF,eCdce62005R2Val51D2KO,eCdce62005C2Val112D0PF,eCdce62005R3Val10D0KO,eCdce62005C3Val19D5PF}
};
