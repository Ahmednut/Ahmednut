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
 * @file    mi250_defines.h
 * @brief   Numerical values for the MI250 FMC card.
 *
 * $Date: 2016/01/25 21:36:31 $
 * $Revision: 1.3 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup MI250
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

#ifndef __MI250_DEFINES__
#define __MI250_DEFINES__
#include "bas_error.h"

 
 
 /** @anchor MI250 defines
 *  @name Core ID defines
 *    
 *  MI250 core ID
 */
#define MI250_COREID  0xc250 	///< MI250 core ID
#define MI250_COREVER  0x0200	///< MI250 core version 

 /** @name Onboard reference clock value
 *    
 *  Onboard reference
 */
#define MI250_CLK_REF_FREQ_100MHz  100000000   ///< Onboard Reference Clock value


 /** @name MI250 PLL defines
 *    
 *  Defines for the MI250 PLL
 */
#define MI250_PFD_MAX_FREQ ((unsigned)10e6)   ///< Maximum PFD frequency
#define MI250_PFD_MIN_FREQ ((unsigned)1e6)    ///< Minimum PFD frequency
#define MI250_MAX_ADC_PLL_CLK_FREQ 250000000  ///< Maximum ADC frequency
#define MI250_MIN_ADC_PLL_CLK_FREQ 50000000  ///< Minimum ADC frequency
#define MI250_MAXVCOFREQ 1310000000				///< Maximum VCO frequency
#define MI250_MINVCOFREQ 1130000000				///< Minimum VCO frequency

// Monitoring devices (ADT7411)
#define MI250_MON_DEVICE1_ADDRESS     (0x96>>1)
#define MI250_MON_DEVICE2_ADDRESS     (0x94>>1)




#define MI250_ADC_CTRL_ADC_1_RUN          0x0001
#define MI250_ADC_CTRL_ADC_2_RUN          0x0002
#define MI250_ADC_CTRL_ADC_1_START        0x0004
#define MI250_ADC_CTRL_ADC_2_START        0x0008
#define MI250_ADC_CTRL_ADC_DATA_TYPE      0x0010

#define MI250_ADC_CTRL_SPI_RESET          0x0001

#define MI250_ADC_CTRL_ADC_1_OVR_RANGE    0x0040
#define MI250_ADC_CTRL_READ_0             0x0080
#define MI250_ADC_CTRL_ADC_2_OVR_RANGE    0x0100
#define MI250_MISC_CTRL_PLL_STATUS        0x0001
#define MI250_MISC_CTRL_PLL_FUNCTION      0x0002
#define MI250_MISC_CTRL_PLL_REF_EN        0x0004
#define MI250_MISC_CTRL_VCO_PWR_EN        0x0008

#define MI250_MISC_CTRL_CLK_MUX_CONFIG    0x0004
#define MI250_MISC_CTRL_CLK_MUX_LOAD      0x0008
#define MI250_MISC_CTRL_CLK_MUX_SIN_0     0x0010
#define MI250_MISC_CTRL_CLK_MUX_SIN_1     0x0020
#define MI250_MISC_CTRL_CLK_MUX_SOUT_0    0x0040
#define MI250_MISC_CTRL_CLK_MUX_SOUT_1    0x0080
#define MI250_MISC_CTRL_UPDATER_BUSY      0x0400
#define MI250_MISC_CTRL_SPI_BUSY          0x0800
#define MI250_MISC_CTRL_REQUEST_2         0x1000
#define MI250_MISC_CTRL_GRANT_2           0x2000
#define MI250_MISC_CTRL_ACK_2             0x4000



#define MISC_CTRL_CLK_MUX_ALL_BITS  (MISC_CTRL_CLK_MUX_CONFIG | \
                                     MISC_CTRL_CLK_MUX_LOAD | \
                                     MISC_CTRL_CLK_MUX_SIN_0 | \
                                     MISC_CTRL_CLK_MUX_SIN_1 | \
                                     MISC_CTRL_CLK_MUX_SOUT_0 | \
                                     MISC_CTRL_CLK_MUX_SOUT_1)


/** @name MI250 error codes
 * 
 *  List all runtime related error codes of a MI250 module.
 */
enum mi250_error {
    MI250OK    = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_MI250, 0),                ///<  Success 
    MI250_NO_VALID_DIVIDER = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI250, 1),      ///<  Cannot calculate Fvco Value 
    MI250_NO_VALID_R = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI250, 2),            ///<  No valid R divider value 
    MI250_REF_CLOCK_TO_HIGH = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI250, 3),     ///<  Reference clock frequency is too high 
    MI250_PFDFREQ_OUT_OF_RANGE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI250, 4),  ///< Phase Frequency Detector frequency is out of range 
    MI250_NO_VALID_PLL_SOLUTION = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI250, 5), ///<  No valid PLL parameter found given current constraints 
    MI250_VCO_OUT_OF_RANGE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI250, 6),      ///<  VCO is out of valid range 
    MI250_PFDFREQ_OUT_OF_SPEC = MAKE_RESULT(LYR_SEVERITY_WARNING, MODULE_MI250, 7), ///<  Running PLL out of specification; PFD is smaller than 1 MHz 
    MI250MUXERROR = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI250, 8),               ///<  invalid mux configuration 
    MI250INVALIDCLOCKSOURCE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI250, 9),    ///<  Invalid clock source 
    MI250ADCSETUPERRORS = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI250, 10),    ///<  ADC write errors 
    MI250COREABSENT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI250, 11), ///<  FPGA core is absent 
    MI250CALIBRATIONFAILURE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI250, 12), ///<  Failed to calibrate ADC data ports for given frequency
    MI250MASTERCALIBPTRINV = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI250, 13), ///<  The pointer to the clock master calibration structure is not set
    MI250MASTERCALIBNOTDONE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI250, 14), ///<  The clock master board need to be calibrated before the clock slave modules are calibrated

};

#endif

