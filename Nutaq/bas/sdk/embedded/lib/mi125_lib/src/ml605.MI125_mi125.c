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
 * @file       ml605.MI125_mi125.c
 * @brief      mi125 module related functions
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup MI125
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

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "MI125_mi125.h"
#include "MI125_com_wrapper.h"


/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/

 // Version of MI125 driver
#define MI125_DVERSION 0x001c


// Comment this line for the real MI125 target
//#define MI125_SIMUL 1

#ifdef DEBUG
// Uncomment this line to add some trace debug output strings
#define MI125_MODDEBUG 1
#endif

// Debug related definitions
#ifdef MI125_MODDEBUG
    // These macros are used to get printout info while debugging
    #define FUNC_ENTER()      printf("MI125: enter %s\n", __FUNCTION__)
    #define FUNC_EXIT()       printf("MI125: exit  %s\n", __FUNCTION__)
    #define PDEBUG(f, ...) printf(f, __VA_ARGS__)

#else

    #define FUNC_ENTER()
    #define FUNC_EXIT()
    #define PDEBUG(f, ...)

#endif

// This identifies the MI125 fpga core
#define MI125_COREID_MI125 0xC125

#define MI125_ADCCLKPLL_DELAY (25 * 2)             ///< Delay to stabilize the adc clock input pll (min 25 usec, keep some margin)
#define MI125_ADCSMALL_DELAY (1)                   ///< Smallest possible delay
#define MI125_ADCIDELAYRDY_DELAY (3 * 5)           ///< from FPGA IDELAYCTRLRst 1->0 to IDELAYRDY = 0->1, Virtex-6-1 at 100MHz
#define MI125_ADCMCMCLOCKRDY_DELAY (100 * 2)       ///< from FPGA ADCClockMMCMRst 1->0 to AdcClockMmcmLock = 0->1, Virtex-6-1 (max 100usec)
// !!! M.T. to be tuned later
#define MI125_ADCDIGITALCALIB (50000)              ///< Max time allowed for a single calibration trial process
#define MI125_ADCSLEEPOUT_DELAY (2000 * 2)         ///< Delay to stabilize the adc recovering from sleep mode (when new additionnal channels are configured)

// Local osc stabilization time specified by manufacturer, need to measure approx.
// Startup is approx. 2msec (measured)
#define MI125_LOCALOSC_DELAY (10000)               ///< Delay to stabilize the 125 MHz local oscillator after powerup

// Specified by manuf is max 195msec
#define MI125_BRIDGESTART_DELAY (200000)           ///< Worst case delay before the spi bridge is out of reset and becoming ready for used

// Local osc stabilization time specified by manufacturer, need to measure approx.
// Startup is approx 4msec (measured)
#define MI125_BUF_CLOCK_PWRN (10000)               ///< Delay to stabilize the 125 MHz local oscillator after powerup

// ADC Digital calibration pattern used
#define MI125_ADCCALIB 0x033C   // This will calibrate to 0x2C value for each output lane

// Pattern to test single calib support mode
#define MI125_TEST_CALIBSNG 0x55AA33CC

// Defines the needed delay for the mi125 trig signal (to the record core) so
// that the adc captured data match the position of the trigger. The prog delay
// is programmed value + 1
#define MI125_TRIG_ADC_SYNC_DELAY 6

// Fix part of io expander i2c addresses
#define PCF8574_I2CID 0x38
#define PCA9535_I2CID 0x20


/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/


/****************************************************************************
 *                             Local variables                              *
 ****************************************************************************/


/****************************************************************************
 *                       Private functions prototypes                       *
 ****************************************************************************/
static Result _MI125_mi125_setclkbuf14fanout(MI125_mi125 * board);
static Result _MI125_mi125_detectbetaboard(MI125_mi125 * board);


/****************************************************************************
 *                             Private Functions                            *
 ****************************************************************************/


 /****************************************************************************
 * NAME : _MI125_mi125_setclkbuf14fanout
 ************************************************************************//**
 *
 * Configure the clock buffer switch in 1:4 fanout buffer mode according
 * to the current selected clock source.
 *
 * @warning
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - The clock source input selection must be set with MI125_mi125_set_clksrc() before
 *   using this function to select which clock source to program in the clock buffer outputs.
 *
 * @param [in] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open(), MI125_mi125_reset(), MI125_mi125_set_clksrc().
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_reset, MI125_mi125_close, MI125_mi125_set_clksrc.
 *
 ***************************************************************************/
static Result _MI125_mi125_setclkbuf14fanout(MI125_mi125 * board)
{
    Result ret = OK;

#ifndef SIMUL
    int outval;

    // For each clock buffer output to configure to the selected clock source
    for (outval = MI125_PCA9535_CKBUFOUT0; outval <= MI125_PCA9535_CKBUFOUT3; outval++)
    {
        // Wait small delay to meet clock input selection setup time (from MI125_mi125_set_clksrc)
        // or config hold time after each output is programmed
        (*board->waitusec)(MI125_ADCSMALL_DELAY);
        ret = MI125_pca9535_setclkbufctrl(&board->expander2, MI125_PCA9535_CKBUFCONFOFF,
                                                             MI125_PCA9535_CKBUFLOADOFF,
                                                             outval);
        if(FAILURE(ret)) {return ret;}

        // Wait small delay to meet clock output selection setup time
        (*board->waitusec)(MI125_ADCSMALL_DELAY);
        ret = MI125_pca9535_setclkbufctrl(&board->expander2, MI125_PCA9535_CKBUFCONFOFF,
                                                             MI125_PCA9535_CKBUFLOADON,
                                                             outval);
        if(FAILURE(ret)) {return ret;}

        // Wait small delay to meet load pulse width time
        (*board->waitusec)(MI125_ADCSMALL_DELAY);
        ret = MI125_pca9535_setclkbufctrl(&board->expander2, MI125_PCA9535_CKBUFCONFOFF,
                                                             MI125_PCA9535_CKBUFLOADOFF,
                                                             outval);
        if(FAILURE(ret)) {return ret;}

        // Wait small delay to meet load to config setup time
        (*board->waitusec)(MI125_ADCSMALL_DELAY);
        ret = MI125_pca9535_setclkbufctrl(&board->expander2, MI125_PCA9535_CKBUFCONFON,
                                                             MI125_PCA9535_CKBUFLOADOFF,
                                                             outval);
        if(FAILURE(ret)) {return ret;}

        // Wait small delay to meet config pulse width time
        (*board->waitusec)(MI125_ADCSMALL_DELAY);
        ret = MI125_pca9535_setclkbufctrl(&board->expander2, MI125_PCA9535_CKBUFCONFOFF,
                                                             MI125_PCA9535_CKBUFLOADOFF,
                                                             outval);
        if(FAILURE(ret)) {return ret;}
    }

    // Wait small delay to meet config hold time
    (*board->waitusec)(MI125_ADCSMALL_DELAY);

#endif

    return ret;
}


 /****************************************************************************
 * NAME : _MI125_mi125_detectbetaboard
 ************************************************************************//**
 *
 * Attempt to detect if we have a beta board (or later) or not (alpha).
 * This function will set in global structure information the detected board
 * type (beta member).
 *
 * @warning
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - This function may wrongly detect a beta board as an alpha board if the beta
 *   board is defective.
 *
 * @param [in,out] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *    The beta member is set correctly according to board type.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open(), MI125_mi125_reset().
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_reset, MI125_mi125_close.
 *
 ***************************************************************************/
static Result _MI125_mi125_detectbetaboard(MI125_mi125 * board)
{
    Result ret;

#ifndef SIMUL
    MI125_pca9535_addr caddr;
    volatile unsigned char inval;

    // Read back existing related registers content
    caddr = MI125_PCA9535_00;
    ret = MI125_com_read(board->expander2.com, MI125_COM_PORTS2, caddr, (unsigned char*)&inval);
    if(FAILURE(ret))
        // Set operation to alpha board
        board->beta = 0;
    else
        // Set operation to beta board and later
        board->beta = 1;
#else
    // Set operation to beta board
    board->beta = 1;
#endif

    return OK;
}


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


/****************************************************************************
 * Function: MI125_mi125_open
 ************************************************************************//**
 *
 * Open a mi125 module instance (one must be opened for each fmc module instance
 * available).
 * When the module is not used anymore, it can be closed with MI125_mi125_close().
 * @warning
 * This function must be the first one used before using an mi125 module.
 *
 * @param [in,out] board
 *    This structure will hold necessary information for the module to be used.
 *    An empty structure is provided and it is filled before returning.
 *
 * @param [in] core
 *    This is the fpga core mi125 instance address.
 *
 * @param [in] i2c_base_addr
 *    This is the fpga i2c instance base address. In the perseus module, the fpga is
 *    connected to the i2c devices chain of the fmc module.
 *
 * @param [in] release
 *    This is the fpga i2c bus control base address. In the perseus module, this
 *    control allows to grant the i2c bus control to a master or not before using.
 *
 * @param [in] i2c_dev_list
 *    This is a table of all devices i2c addresses of the module. These address
 *    will vary, according to GA used (normally 00 for perseus) and also will
 *    be different if the module is in the bottom or top position of the fmc slot.
 *    For perseus, bottom of fmc slot is normally #MI125_I2CADR_MAIN_GA00.
 *    For perseus, top of fmc slot is normally #MI125_I2CADR_SEC_GA00.
 *
 * @param [in] timer_waitusec
 *    Function to wait for some uSec delay (similar as linux usleep)..
 *
 * @param [in] fmcposition
 *    This specifies for this module instance what is the module position in the fmc slot
 *    (#MI125_BOTTOM or #MI125_TOP).
 *
 * @return
 *    Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured.
 *
 * Example:
 * @code
 * // ...
 *
 * // Perseus base address
 * #define PERSEUS_BASE_ADDR_REG (XPAR_AXI_PERSEUS6010_REGS_BASEADDR)
 *
 * // Perseus I2C bus release control register
 * #define PERSEUS_I2C_RELEASE_REG  ((MI125_i2c_release *)(PERSEUS_BASE_ADDR_REG + 0xA0))
 *
 * // MI125 core addresses
 * #define MI125_BOTTOM_ADDRESS ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_BOTTOM_BASEADDR))
 * #define MI125_TOP_ADDRESS    ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_TOP_BASEADDR))
 *
 * // I2C module instance in perseus
 * #define PERSEUS_I2C_MODULE (XPAR_FMC_I2C_BASEADDR)
 * // ...
 * {
 *  Result ret;
 *  MI125_mi125 board[MI125_NBMAX];
 *  MI125_mi125_i2caddr i2c_dev_list[MI125_NBMAX]={MI125_I2CADR_MAIN_GA00,MI125_I2CADR_SEC_GA00}; // GA=00 for perseus
 *  MI125_mi125_Register_Map *mi125core[MI125_NBMAX]={MI125_BOTTOM_ADDRESS,MI125_TOP_ADDRESS};
 *  int lastboard = MI125_NBMAX;
 *  int boardtype[MI125_NBMAX]={0,1},inst;
 *  MI125_mi125_fmc_position boardposition[MI125_NBMAX]={MI125_BOTTOM,MI125_TOP};
 *
 *  // For each board instance that we have detected
 *  for (inst=0; inst<lastboard; inst++)
 *  {
 *      // Open the mi125 module instance and provides our own timer functions (no OS available to provide them)
 *  	ret = MI125_mi125_open(&board[inst], mi125core[inst], PERSEUS_I2C_MODULE, PERSEUS_I2C_RELEASE_REG,
 *                             &i2c_dev_list[boardtype[inst]],
 *                             timer_waitusec,
 *                             boardposition[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("MI125_mi125_open FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *       //Reset and setup MI125 with basic default configuration (after module powerup, all 16 channels)
 *      ret = MI125_mi125_reset(&board[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Reset FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *  }
 *  // When closing the module
 *  for (inst=0; inst<lastboard; inst++)
 *  {
 *      // When module not needed anymore
 *      ret = MI125_mi125_close(&board[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("MI125_mi125_close FAILED: 0x%08X\n",ret);
 *      }
 *  }
 * }
 * @endcode
 *
 * @see
 *    MI125_mi125_close, @ref MI125_board_i2c_addresses
 *
 ***************************************************************************/
Result MI125_mi125_open(MI125_mi125 * board, MI125_mi125_Register_Map * core, unsigned i2c_base_addr, MI125_i2c_release * release,
                        MI125_mi125_i2caddr *i2c_dev_list,
                        int (*timer_waitusec)(unsigned int delayus),
                        MI125_mi125_fmc_position fmcposition)
{
    Result ret;
    // Default lane offset delays for a dual fmc mi125 stack
    // !!! M.T. These values would need to be changed if a mi125 is on top of a different fmc board.
    
    /* Perseus
    MI125_mi125_laneoffset lanedelay[MI125_NBMAX] = {{0x00000000,0x00000000,0x00000000,0x00000000},
                                                     {0x00000000,0x00000000,0x00000000,0xBB00CBDB}};
    */
    
    /* ML605 */
    MI125_mi125_laneoffset lanedelay[MI125_NBMAX] = {{0x00000000,0x00000000,0x00000000,0x00000000},
                                                     {0x11111111,0x11111111,0x11555555,0xDD00510D}};

    FUNC_ENTER();

    // Make sure fmcposition is possible
    fmcposition = fmcposition % MI125_NBMAX;

    // Save the lanes offset delays
    board->lanedelays.laneoffset0 = lanedelay[fmcposition].laneoffset0;
    board->lanedelays.laneoffset1 = lanedelay[fmcposition].laneoffset1;
    board->lanedelays.laneoffset2 = lanedelay[fmcposition].laneoffset2;
    board->lanedelays.laneoffset3 = lanedelay[fmcposition].laneoffset3;

    // Keep trace if global module reset was done or not
    board->resetdone = 0;

    // Default operation is alpha board
    board->beta = 0;

    // This shows that we are not currently in the reset procedure
    board->resetinprog = 0;

    // Keep trace if module calib was done correctly or not
    board->calibdone = 0;

    // Keep trace of the number of calibration trials
    board->calibndx = 0;

    // Keep trace of the channels lane that were calibrated correctly the last time it was done
    board->channelslanecalib[board->calibndx] = 0;

    // Clear mask used to check calibration (default all 16 channels lanes)
    board->channellanemask = 0xffffffff;

    // Set default randomize mode
    board->randommode = 0;

    // Default does not support single calib mode
    board->singlecalib = 0;

    // Keep trace if test mode is active
    board->testmode = 0;

    // By default, all 16 channels are used
    board->groupch = MI125_16CHANNELS;

    // Assign the board memory map registers
    board->core = core;

    // Save the board position (bottom or top)
    board->fmcposition = fmcposition;

    // Check if delay function is correct
    if (!timer_waitusec) {FUNC_EXIT(); return MI125_MI125BADUSECFCT;}

    // Save the timer functions
    board->waitusec = timer_waitusec;

    // Open the communication channel for other devices that are i2c based
    // For the moment, the expander & expander2 addresses are hard coded
    // generated from the known fix part of device address
    ret = MI125_com_open(&board->com, i2c_base_addr, release, i2c_dev_list->spibridge,
                   i2c_dev_list->sensor,
                   (i2c_dev_list->expander & 0x07) | PCF8574_I2CID,
                   (i2c_dev_list->expander & 0x07) | PCA9535_I2CID);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Open the io expander module (alpha boards)
    ret = MI125_pcf8574_open(&board->expander, &board->com);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Open the io expander2 module (beta boards and later)
    ret = MI125_pca9535_open(&board->expander2, &board->com);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Open the adc module
    ret = MI125_ltm9012_open(&board->adc, &board->com);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Open the temperature monitor module
    ret = MI125_lm75_open(&board->monitor, &board->com);

    FUNC_EXIT();

    return ret;
}


 /****************************************************************************
 * NAME : MI125_mi125_reset
 ************************************************************************//**
 *
 * Reset the mi125 module instance to default operating condition:
 * all 16 channels active, internal 125 MHz clock source, trig out disable,
 * 2compl output mode, adc randomize output off, lvds with termination and 1.75mA drive current,
 * testmode disable. After a reset, the module is already configure correctly
 * for adc capture.
 *
 * @warning
 * - This function must be called at least once to reset this device to default
 *   operating condition before using other module functions to change the default
 *   operating mode.
 * - The module must be opened with MI125_mi125_open() before using this function.
 *
 * @attention
 * The module which is the clock master in a multi modules configuration
 * must be reset and configured first before the non master module. Resetting, reconfiguring
 * the clock source or changing any external clock source frequency for the clock master will
 * requiere that the non master module is calibrated again (see MI125_mi125_get_clkmaster()).
 * At reset, the default clock source is internal 125MHz (#MI125_CLKSRC125MHZ) for bottom
 * adapter, and fmc bottom clock source for top adapter (#MI125_CLKSRCBOTTOMFMC).
 *
 * @param [in] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *
 * @return
 *    Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open()
 *
 * Example:
 * @code
 * // ...
 *
 * // Perseus base address
 * #define PERSEUS_BASE_ADDR_REG (XPAR_AXI_PERSEUS6010_REGS_BASEADDR)
 *
 * // Perseus I2C bus release control register
 * #define PERSEUS_I2C_RELEASE_REG  ((MI125_i2c_release *)(PERSEUS_BASE_ADDR_REG + 0xA0))
 *
 * // MI125 core addresses
 * #define MI125_BOTTOM_ADDRESS ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_BOTTOM_BASEADDR))
 * #define MI125_TOP_ADDRESS    ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_TOP_BASEADDR))
 *
 * // I2C module instance in perseus
 * #define PERSEUS_I2C_MODULE (XPAR_FMC_I2C_BASEADDR)
 * // ...
 * {
 *  Result ret;
 *  MI125_mi125 board[MI125_NBMAX];
 *  MI125_mi125_i2caddr i2c_dev_list[MI125_NBMAX]={MI125_I2CADR_MAIN_GA00,MI125_I2CADR_SEC_GA00}; // GA=00 for perseus
 *  MI125_mi125_Register_Map *mi125core[MI125_NBMAX]={MI125_BOTTOM_ADDRESS,MI125_TOP_ADDRESS};
 *  int lastboard = MI125_NBMAX;
 *  int boardtype[MI125_NBMAX]={0,1},inst;
 *  MI125_mi125_fmc_position boardposition[MI125_NBMAX]={MI125_BOTTOM,MI125_TOP};
 *
 *  // For each board instance that we have detected
 *  for (inst=0; inst<lastboard; inst++)
 *  {
 *      // Open the mi125 module instance and provides our own timer functions (no OS available to provide them)
 *  	ret = MI125_mi125_open(&board[inst], mi125core[inst], PERSEUS_I2C_MODULE, PERSEUS_I2C_RELEASE_REG,
 *                             &i2c_dev_list[boardtype[inst]],
 *                             timer_waitusec,
 *                             boardposition[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("MI125_mi125_open FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *       //Reset and setup MI125 with basic default configuration (after module powerup, all 16 channels)
 *      ret = MI125_mi125_reset(&board[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Reset FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *  }
 *  // ... Close the module when not needed
 * }
 * @endcode
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_close.
 *
 ***************************************************************************/
Result MI125_mi125_reset(MI125_mi125 * board)
{
    Result ret = OK;
    int jj;
    MI125_mi125_clock_source clksrc;

    FUNC_ENTER();

    // Keep trace if global module reset was done or not
    board->resetdone = 0;

    // Default operation is alpha board
    board->beta = 0;

    // Keep trace of the number of calibration trials
    board->calibndx = 0;

    // Keep trace of the channels lane that were calibrated correctly the last time it was done
    board->channelslanecalib[board->calibndx] = 0;

    // Clear mask used to check calibration (default all 16 channels lanes)
    board->channellanemask = 0xffffffff;

    // Keep trace if calib was made correctly or not
    board->calibdone = 0;

    // Default does not support single calib mode
    board->singlecalib = 0;

    // By default, all 16 channels are used
    board->groupch = MI125_16CHANNELS;

#ifndef SIMUL
    // Check if the core is correct MI125 type
    if (board->core->Register_mi125info.Bit.coreid != MI125_COREID_MI125)
    {
        FUNC_EXIT();
        return(MI125_MI125NOCORE);
    }
#endif

#ifndef SIMUL
    // Reset the mi125 core
    board->core->Register_mi125ctrl.Bit.IPSoftRst = 1;

    // Reset the mi125 trigger logic
    board->core->Register_mi125ctrl.Bit.TrigSyncRst = 1;

    // Reset the mi125 core clock MCM
    board->core->Register_mi125ctrl.Bit.ADCClockMMCMRst = 1;

    // Wait small delay
    (*board->waitusec)(MI125_ADCSMALL_DELAY);

    // Check for single calib support
    board->core->Register_mi125calibenctrl.raw = MI125_TEST_CALIBSNG;
    if (board->core->Register_mi125calibenctrl.raw == MI125_TEST_CALIBSNG)
    {
        board->core->Register_mi125calibenctrl.raw = 0;

        if (board->core->Register_mi125calibenctrl.raw == 0)
        {
            // Support single lane calib mode
            board->singlecalib = 1;
        }
    }

    // Remove the core reset
    board->core->Register_mi125ctrl.Bit.IPSoftRst = 0;

    // Remove the mi125 trigger logic reset
    board->core->Register_mi125ctrl.Bit.TrigSyncRst = 0;

    // Remove core MCM clock reset
    board->core->Register_mi125ctrl.Bit.ADCClockMMCMRst = 0;

    // Configure the lane offset delays
    board->core->Register_mi125delaylaneoffset0.raw = board->lanedelays.laneoffset0;
    board->core->Register_mi125delaylaneoffset1.raw = board->lanedelays.laneoffset1;
    board->core->Register_mi125delaylaneoffset2.raw = board->lanedelays.laneoffset2;
    board->core->Register_mi125delaylaneoffset3.raw = board->lanedelays.laneoffset3;

    // Configure the trigger delay to compensate with adc lag and internal fpga delays
    board->core->Register_mi125ctrl.Bit.TriggerDelay = MI125_TRIG_ADC_SYNC_DELAY;

    // Configure the hardware with the number of active channels group
    board->core->Register_mi125ctrl.Bit.ChannelConfig = board->groupch;
#endif

    // Attempt to detect the board type (will set the beta member according to detection)
    ret = _MI125_mi125_detectbetaboard(board);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Reset the io expander according to board type
    if (board->beta)
    {
        // Reset the io expander
        ret = MI125_pca9535_reset(&board->expander2);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

        // Because it is possible a beta board could have the bridge in reset before
        // resetting the io expander, an additionnal delay before the bridge is ready
        // could be needed to make sure that the bridge is ready for operation.
        // Attempt to initialize the com channel and the bridge (is it ready for used?);
        ret = MI125_com_init(&board->com);
        if(FAILURE(ret))
        {
            // Wait the worst time bridge reset delay
            (*board->waitusec)(MI125_BRIDGESTART_DELAY);

            // Retry to configure the bridge which now must be ready
            ret = MI125_com_init(&board->com);
        }
    }
    else
    {
        // Initialize the com channel and the bridge
        ret = MI125_com_init(&board->com);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

        // Reset the io expander
        ret = MI125_pcf8574_reset(&board->expander);
    }
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // If our board is at top position, the clk source could not be anything else that bottom fmc clk
    // because of an fpga clock distribution limitation
    if (board->fmcposition != MI125_BOTTOM)
    {
        // Default for top adapter
        clksrc = MI125_CLKSRCBOTTOMFMC;
    }
    else
    {
        // Default for bottom adapter
        clksrc = MI125_CLKSRC125MHZ;
    }

    // Change the clksrc according to board type
    if (board->beta)
    {
        MI125_pca9535_localoscsw sw;

        // Check if we don't use the internal oscillator and turn it off to lower noise
        if (clksrc == MI125_CLKSRC125MHZ)
        {
            sw = MI125_PCA9535_LOSWON;
            ret = MI125_pca9535_setlocaloscsw(&board->expander2, sw);
            if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

            // Make sure the local oscillator has started and stabilized
            // Wait for the bigger of MI125_LOCALOSC_DELAY or MI125_BUF_CLOCK_PWRN
            (*board->waitusec)((MI125_LOCALOSC_DELAY > MI125_BUF_CLOCK_PWRN ? MI125_LOCALOSC_DELAY : MI125_BUF_CLOCK_PWRN));
        }
        else
        {
            sw = MI125_PCA9535_LOSWOFF;
            ret = MI125_pca9535_setlocaloscsw(&board->expander2, sw);
            if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

            // Make sure the clock switch is power correctly after the io expander reset
            // before using _MI125_mi125_setclkbuf14fanout()...
            (*board->waitusec)(MI125_BUF_CLOCK_PWRN);
        }

        ret = MI125_pca9535_setclksource(&board->expander2, clksrc);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}
        ret = _MI125_mi125_setclkbuf14fanout(board);
    }
    else
        ret = MI125_pcf8574_setclksource(&board->expander, clksrc);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Reset each adc
    for (jj = 0; jj <= board->groupch; jj++)
    {
        // Reset the adc
        ret = MI125_ltm9012_reset(&board->adc, MI125_COM_ADC0 + jj);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}
    }

    // Set fpga to match adc data format to record correctly in memory (two complement)
    board->core->Register_mi125ctrl.Bit.DataFormat = 1;

    // Set default randomize mode
    board->randommode = 0;

    // Set fpga to match adc randomize mode
    board->core->Register_mi125ctrl.Bit.DigOutRandEn = board->randommode;

    // Reset the temp monitor device to default configuration
    ret = MI125_lm75_reset(&board->monitor);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Reset the clock mcm before the calibration
    ret = MI125_mi125_clockreset(board);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // This shows that we are currently in the reset procedure (to force a calibration anyway)
    board->resetinprog = 1;

    // Digital output calibration
    ret = MI125_mi125_digital_adccalibration(board);

    // Remove what we added before
    board->resetinprog = 0;

    if(SUCCESS(ret))
        // Keep trace if global module reset was done or not
        board->resetdone = 1;

    FUNC_EXIT();

    return ret;
}


 /****************************************************************************
 * NAME : MI125_mi125_close
 ************************************************************************//**
 *
 * Close the mi125 module instance.
 *
 * @warning
 * This function must be the last one used after using this module.
 *
 * @param [in] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open()
 *
 * Example:
 * @code
 * // ...
 *
 * // Perseus base address
 * #define PERSEUS_BASE_ADDR_REG (XPAR_AXI_PERSEUS6010_REGS_BASEADDR)
 *
 * // Perseus I2C bus release control register
 * #define PERSEUS_I2C_RELEASE_REG  ((MI125_i2c_release *)(PERSEUS_BASE_ADDR_REG + 0xA0))
 *
 * // MI125 core addresses
 * #define MI125_BOTTOM_ADDRESS ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_BOTTOM_BASEADDR))
 * #define MI125_TOP_ADDRESS    ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_TOP_BASEADDR))
 *
 * // I2C module instance in perseus
 * #define PERSEUS_I2C_MODULE (XPAR_FMC_I2C_BASEADDR)
 * // ...
 * {
 *  Result ret;
 *  MI125_mi125 board[MI125_NBMAX];
 *  MI125_mi125_i2caddr i2c_dev_list[MI125_NBMAX]={MI125_I2CADR_MAIN_GA00,MI125_I2CADR_SEC_GA00}; // GA=00 for perseus
 *  MI125_mi125_Register_Map *mi125core[MI125_NBMAX]={MI125_BOTTOM_ADDRESS,MI125_TOP_ADDRESS};
 *  int lastboard = MI125_NBMAX;
 *  int boardtype[MI125_NBMAX]={0,1},inst;
 *  MI125_mi125_fmc_position boardposition[MI125_NBMAX]={MI125_BOTTOM,MI125_TOP};
 *
 *  // For each board instance that we have detected
 *  for (inst=0; inst<lastboard; inst++)
 *  {
 *      // Open the mi125 module instance and provides our own timer functions (no OS available to provide them)
 *  	ret = MI125_mi125_open(&board[inst], mi125core[inst], PERSEUS_I2C_MODULE, PERSEUS_I2C_RELEASE_REG,
 *                             &i2c_dev_list[boardtype[inst]],
 *                             timer_waitusec,
 *                             boardposition[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("MI125_mi125_open FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *       //Reset and setup MI125 with basic default configuration (after module powerup, all 16 channels)
 *      ret = MI125_mi125_reset(&board[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Reset FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *  }
 *  // When closing the module
 *  for (inst=0; inst<lastboard; inst++)
 *  {
 *      // When module not needed anymore
 *      ret = MI125_mi125_close(&board[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("MI125_mi125_close FAILED: 0x%08X\n",ret);
 *      }
 *  }
 * }
 * @endcode
 *
 * @see
 *    MI125_mi125_open
 *
 ***************************************************************************/
Result MI125_mi125_close(MI125_mi125 * board)
{
    Result ret = OK;

    FUNC_ENTER();

    // Keep trace if global module reset was done or not
    board->resetdone = 0;

    // This shows that we are not currently in the reset procedure
    board->resetinprog = 0;

    // Keep trace of the number of calibration trials
    board->calibndx = 0;

    // Keep trace of the channels lane that were calibrated correctly the last time it was done
    board->channelslanecalib[board->calibndx] = 0;

    // Clear mask used to check calibration (default all 16 channels lanes)
    board->channellanemask = 0xffffffff;

    // Keep trace if calib was made corerectly or not
    board->calibdone = 0;

    // Default does not support single calib mode
    board->singlecalib = 0;

    // Close all board devices
    ret =  MI125_lm75_close(&board->monitor);
    ret =  MI125_ltm9012_close(&board->adc);
    ret =  MI125_pcf8574_close(&board->expander);
    ret =  MI125_pca9535_close(&board->expander2);
    ret =  MI125_com_close(&board->com);

    FUNC_EXIT();

    return ret;
}


 /****************************************************************************
 * NAME : MI125_mi125_digital_adccalibration
 ************************************************************************//**
 *
 * Force a digital calibration of all adc with active channels.
 * This function must be used each time adc clock input configuration / frequency is changed.
 * It must be used also each time the board active channels configuration is changed (when new
 * channels are added: this is handled internally).
 *
 * @warning
 * - Failure to do a needed calibration could give unpredictable adc data captured.
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - The MI125_mi125_reset() function must be call at least once before using this function.
 *
 * @attention
 * A change of clock for the clock master implies that a calibration must be done
 * also for the module which is not the clock master (see MI125_mi125_set_clksrc() and
 * MI125_mi125_get_clkmaster()). If the clock frequency is changed for an external source,
 * the function MI125_mi125_clockreset() on the clock master must be called before doing a
 * calibration with this function.
 *
 * @param [in] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open(), MI125_mi125_reset().
 *
 * Example:
 * @code
 * // ...
 *
 * // Perseus base address
 * #define PERSEUS_BASE_ADDR_REG (XPAR_AXI_PERSEUS6010_REGS_BASEADDR)
 *
 * // Perseus I2C bus release control register
 * #define PERSEUS_I2C_RELEASE_REG  ((MI125_i2c_release *)(PERSEUS_BASE_ADDR_REG + 0xA0))
 *
 * // MI125 core addresses
 * #define MI125_BOTTOM_ADDRESS ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_BOTTOM_BASEADDR))
 * #define MI125_TOP_ADDRESS    ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_TOP_BASEADDR))
 *
 * // I2C module instance in perseus
 * #define PERSEUS_I2C_MODULE (XPAR_FMC_I2C_BASEADDR)
 * // ...
 * {
 *  Result ret;
 *  MI125_mi125 board[MI125_NBMAX];
 *  MI125_mi125_i2caddr i2c_dev_list[MI125_NBMAX]={MI125_I2CADR_MAIN_GA00,MI125_I2CADR_SEC_GA00}; // GA=00 for perseus
 *  MI125_mi125_Register_Map *mi125core[MI125_NBMAX]={MI125_BOTTOM_ADDRESS,MI125_TOP_ADDRESS};
 *  int lastboard = MI125_NBMAX;
 *  int boardtype[MI125_NBMAX]={0,1},inst;
 *  MI125_mi125_fmc_position boardposition[MI125_NBMAX]={MI125_BOTTOM,MI125_TOP};
 *
 *  // For each board instance that we have detected
 *  for (inst=0; inst<lastboard; inst++)
 *  {
 *      // Open the mi125 module instance and provides our own timer functions (no OS available to provide them)
 *  	ret = MI125_mi125_open(&board[inst], mi125core[inst], PERSEUS_I2C_MODULE, PERSEUS_I2C_RELEASE_REG,
 *                             &i2c_dev_list[boardtype[inst]],
 *                             timer_waitusec,
 *                             boardposition[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("MI125_mi125_open FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *       //Reset and setup MI125 with basic default configuration (after module powerup, all 16 channels)
 *      ret = MI125_mi125_reset(&board[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Reset FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *   	// Calibration can be done as needed, but the clock must be reset first if the clock frequency has changed:
 *   	// this is done automatically in all cases in api functions, but not when the clock source is external which is impossible to
 *   	// control what the user is doing with the external clock.
 *      ret = MI125_mi125_digital_adccalibration(&board[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Calib FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *  }
 *  // ... Close the module when not needed
 * }
 * @endcode
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_reset, MI125_mi125_clockreset, MI125_mi125_close,
 *    MI125_mi125_set_clksrc.
 *
 ***************************************************************************/
Result MI125_mi125_digital_adccalibration(MI125_mi125 * board)
{
    Result ret = OK;
    int jj, val;
    unsigned int caliben=0xffffffff, calibrated=0;

    FUNC_ENTER();

    // Keep trace of the number of calibration trials
    board->calibndx = 0;

    // Keep trace of the channels lane that were calibrated correctly the last time it was done
    board->channelslanecalib[board->calibndx] = 0;

    // Keep trace if calib was made correctly or not
    board->calibdone = 0;

    // Check if the board has been reset correctly at least once in not currently in reset!
    if (!board->resetdone && board->resetinprog == 0)
    {
        FUNC_EXIT();
        return(MI125_MI125NORESET);
    }

    // 0. Make sure the adc pll is stabilized if any clock changed was done (min 25 usec)
    (*board->waitusec)(MI125_ADCCLKPLL_DELAY);

    // Calibration Steps
    // 1. Reset FPGA complete clock logic
#ifndef SIMUL
    // Reset the mi125 core clock MCM
    //board->core->Register_mi125ctrl.Bit.ADCClockMMCMRst = 1;

    // Wait small delay
    //(*board->waitusec)(MI125_ADCSMALL_DELAY);

    // Remove core MCM clock reset
    //board->core->Register_mi125ctrl.Bit.ADCClockMMCMRst = 0;
#endif

    // Testmode would be disable by this routine
    board->testmode = 0;

    // Set fpga to match adc randomize mode OFF
    board->core->Register_mi125ctrl.Bit.DigOutRandEn = 0;

    // 2. Setup used ADC correctly with test pattern and calib test mode
    // Setup each adc
    for (jj = 0; jj <= board->groupch; jj++)
    {
        // Set adc in calib mode
        ret = MI125_ltm9012_setdigitalcalib(&board->adc, MI125_LTM9012_DCALIBON, MI125_ADCCALIB, MI125_COM_ADC0 + jj);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}
    }

    // 3. Reset IdelayCtrlRst and IdelayRst.
#ifndef SIMUL
    // These resets are positive edge trigger
    board->core->Register_mi125ctrl.Bit.IdelayCtrlRst = 0;
    board->core->Register_mi125ctrl.Bit.IdelayRst = 0;

    // Wait small delay
    (*board->waitusec)(MI125_ADCSMALL_DELAY);

    board->core->Register_mi125ctrl.Bit.IdelayCtrlRst = 1;
    board->core->Register_mi125ctrl.Bit.IdelayRst = 1;

    // Wait small delay
    (*board->waitusec)(MI125_ADCSMALL_DELAY);

    board->core->Register_mi125ctrl.Bit.IdelayCtrlRst = 0;
    board->core->Register_mi125ctrl.Bit.IdelayRst = 0;
#endif

    // 4. Check IdelayRdy register is getting to 1
    // Wait at least 3 usec timeout which should be max time after delay reset
#ifndef SIMUL
    (*board->waitusec)(MI125_ADCIDELAYRDY_DELAY);

    // Check if we got a timeout
    if ((val = board->core->Register_mi125status.Bit.IdelayRdy) == 0)
    {
        FUNC_EXIT();

        // Problem with signal not rising after reset
        return(MI125_MI125IDELAYRDY);
    }

    // 4.5 Check AdcClockMmcmLock register is getting to 1
    // !!! M.T. Wait at least 3??? usec timeout which should be max time after delay reset
    //(*board->waitusec)(MI125_ADCIDELAYRDY_DELAY);

    // Check if mcm is locked as a security measure
    if ((val = board->core->Register_mi125status.Bit.AdcClockMmcmLock) == 0)
    {
        FUNC_EXIT();

        // Problem with signal not rising after reset
        return(MI125_MI125MMCMLOCK);
    }

    // Makes sure calibration controls are disable
    board->core->Register_mi125ctrl.Bit.DigitalCalibEn = 0;
    board->core->Register_mi125calibenctrl.raw = 0;

    // Try to calibrate to a max number of trials if we are getting problems
    do
    {
        // First try, activates all channels the old way for compatibility purpose
        if (!board->singlecalib)
            // 5. Set DigitalCalibEn (to 1) (old compatibility)
            board->core->Register_mi125ctrl.Bit.DigitalCalibEn = 1;
        else
            // 5. Set individual calibration controls (to 1)
            board->core->Register_mi125calibenctrl.raw = caliben;
#endif

        // 6. Wait for calibration to happens (look for the individual calib lanes instead)
#ifndef SIMUL
        (*board->waitusec)(MI125_ADCDIGITALCALIB);

        if (!board->singlecalib)
            // 7. Reset DigitalCalibEn (to 0)
            board->core->Register_mi125ctrl.Bit.DigitalCalibEn = 0;
        else
            // 7. Reset individual calibration controls (to 0)
            board->core->Register_mi125calibenctrl.raw = 0;

        // Keep trace of the channels lanes that were calibrated
        board->channelslanecalib[board->calibndx] = board->core->Register_mi125calibstatus.Bit.AllChCalibStatus;

        // Check if we are fully calibrated on all channels lanes
        calibrated = ((board->channelslanecalib[board->calibndx] & board->channellanemask) == board->channellanemask);

        // Compute which lanes to recalibrate
        if (!calibrated)
        {
            // If supports single calib, compute which lines to recalibrate, if not recalibrates every lines
            // with global calib bit...
            if (board->singlecalib)
                caliben = ((board->channelslanecalib[board->calibndx] ^ board->channellanemask)) & board->channellanemask;
            PDEBUG("Trial #%02d, to recalibrate: 0x%08X.\n",board->calibndx, caliben);
        }
#endif
#ifndef SIMUL
    // Check if the digital calibration went correctly
    }while(!calibrated && ( (++board->calibndx) < MI125_MAX_CALIB_ATTEMPT));

    // Check if the digital calibration went correctly
    if (!calibrated)
    {
        // Set the index inside allowed buffer
        board->calibndx--;

        FUNC_EXIT();

        // Problem with digital calibration of outputs
        return(MI125_MI125TRAININGDONE);
    }
#endif

    // Set fpga to match adc saved randomize mode
    board->core->Register_mi125ctrl.Bit.DigOutRandEn = board->randommode;

    // 8. Disable ADC digital calib testmode.
    // Setup each adc
    for (jj = 0; jj <= board->groupch; jj++)
    {
        // Set adc in normal mode
        ret = MI125_ltm9012_setdigitalcalib(&board->adc, MI125_LTM9012_DCALIBOFF, MI125_ADCCALIB, MI125_COM_ADC0 + jj);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}
    }

    // Display the number of calibration retries
    PDEBUG("Calibration trials: %d.\n", board->calibndx);

    // Keep trace if calib was made correctly or not
    board->calibdone = 1;

    FUNC_EXIT();

    return ret;
}


 /****************************************************************************
 * NAME : MI125_mi125_set_config
 ************************************************************************//**
 *
 * Configure the board with user specified custom options.
 * This function is optionnal and can be used if the default from MI125_mi125_reset()
 * for board operation is not what is desired.
 * The options that can be configured are: the number of active channels
 * by group of 4 (to lower board consumption and temperature increase when there are
 * unused channels), the adc lvds configuration (termination, drive current),
 * adc randomize output mode (for possible performance improvement if outputs crosstalk),
 * adc data format used (binary offset or 2 complement).
 *
 * @warning
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - The module MI125_mi125_reset() function must be call at least once before using this function.
 *
 * @param [in] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *
 * @param [in] config
 *    This is the configuration structure of the desired options.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open(), MI125_mi125_reset().
 *
 * Example:
 * @code
 * // ...
 *
 * // Perseus base address
 * #define PERSEUS_BASE_ADDR_REG (XPAR_AXI_PERSEUS6010_REGS_BASEADDR)
 *
 * // Perseus I2C bus release control register
 * #define PERSEUS_I2C_RELEASE_REG  ((MI125_i2c_release *)(PERSEUS_BASE_ADDR_REG + 0xA0))
 *
 * // MI125 core addresses
 * #define MI125_BOTTOM_ADDRESS ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_BOTTOM_BASEADDR))
 * #define MI125_TOP_ADDRESS    ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_TOP_BASEADDR))
 *
 * // I2C module instance in perseus
 * #define PERSEUS_I2C_MODULE (XPAR_FMC_I2C_BASEADDR)
 * // ...
 * {
 *  Result ret;
 *  MI125_mi125 board[MI125_NBMAX];
 *  MI125_mi125_i2caddr i2c_dev_list[MI125_NBMAX]={MI125_I2CADR_MAIN_GA00,MI125_I2CADR_SEC_GA00}; // GA=00 for perseus
 *  MI125_mi125_Register_Map *mi125core[MI125_NBMAX]={MI125_BOTTOM_ADDRESS,MI125_TOP_ADDRESS};
 *  int lastboard = MI125_NBMAX;
 *  int boardtype[MI125_NBMAX]={0,1},inst;
 *  MI125_mi125_fmc_position boardposition[MI125_NBMAX]={MI125_BOTTOM,MI125_TOP};
 *  // User config similar as provided by reset function (16 channels, ADC internal lvds termination, lvds drv curr 1.75mA with int termination, 2compl data format, adc output not randomized)
 *  MI125_mi125_config config[MI125_NBMAX] = {{MI125_16CHANNELS,MI125_TERMON1750UA,MI125_RANDOMIZEOFF,MI125_TWOCOMPLEMENTFORMAT},
 *                                            {MI125_16CHANNELS,MI125_TERMON1750UA,MI125_RANDOMIZEOFF,MI125_TWOCOMPLEMENTFORMAT}};
 *
 *  // For each board instance that we have detected
 *  for (inst=0; inst<lastboard; inst++)
 *  {
 *      // Open the mi125 module instance and provides our own timer functions (no OS available to provide them)
 *  	ret = MI125_mi125_open(&board[inst], mi125core[inst], PERSEUS_I2C_MODULE, PERSEUS_I2C_RELEASE_REG,
 *                             &i2c_dev_list[boardtype[inst]],
 *                             timer_waitusec,
 *                             boardposition[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("MI125_mi125_open FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *       //Reset and setup MI125 with basic default configuration (after module powerup, all 16 channels)
 *      ret = MI125_mi125_reset(&board[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Reset FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *      //Config the MI125 with specified user setup (if different than default provided at reset)
 *      ret = MI125_mi125_set_config(&board[inst], &config[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Config FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *  }
 *  // ... Close the module when not needed
 * }
 * @endcode
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_reset, MI125_mi125_close.
 *
 ***************************************************************************/
Result MI125_mi125_set_config(MI125_mi125 * board, MI125_mi125_config *config)
{
    Result ret = OK;
    MI125_ltm9012_randmode irand;
    MI125_ltm9012_binmode ibin;
    MI125_ltm9012_lvds ilvds;
    MI125_ltm9012_channel chact[4];
    MI125_pcf8574_channel actchannel;
    int jj, maxadc;

    FUNC_ENTER();

    // Check if the board has been reset correctly at least once
    if (!board->resetdone)
    {
        FUNC_EXIT();
        return(MI125_MI125NORESET);
    }

    // Configure the number of active channels
    chact[0] = MI125_LTM9012_4CHANNEL;

    // Validates how many channels are active by group of 4
    switch(config->groupch)
    {
        case MI125_04CHANNELS:
            chact[1] = MI125_LTM9012_0CHANNEL;
            chact[2] = MI125_LTM9012_0CHANNEL;
            chact[3] = MI125_LTM9012_0CHANNEL;
            actchannel = MI125_PCF8574_04CHANNELS;
            board->channellanemask = 0x000000ff;
            break;

        case MI125_08CHANNELS:
            chact[1] = MI125_LTM9012_4CHANNEL;
            chact[2] = MI125_LTM9012_0CHANNEL;
            chact[3] = MI125_LTM9012_0CHANNEL;
            actchannel = MI125_PCF8574_08CHANNELS;
            board->channellanemask = 0x0000ffff;
            break;

        case MI125_12CHANNELS:
            chact[1] = MI125_LTM9012_4CHANNEL;
            chact[2] = MI125_LTM9012_4CHANNEL;
            chact[3] = MI125_LTM9012_0CHANNEL;
            actchannel = MI125_PCF8574_12CHANNELS;
            board->channellanemask = 0x00ffffff;
            break;

        case MI125_16CHANNELS:
            chact[1] = MI125_LTM9012_4CHANNEL;
            chact[2] = MI125_LTM9012_4CHANNEL;
            chact[3] = MI125_LTM9012_4CHANNEL;
            actchannel = MI125_PCF8574_16CHANNELS;
            board->channellanemask = 0xffffffff;
            break;

        default:
            // Invalid channel config
            FUNC_EXIT();
            return(MI125_MI125INVALIDCH);
    }

    // Check if we added more channels to the group than what we currently have?
    if (config->groupch > board->groupch)
        // This means we need more channels to be actived than before (add more)
        maxadc = config->groupch;
    else
        maxadc = board->groupch;

    // Configure each adc for the channels used
    for (jj = 0; jj <= maxadc; jj++)
    {
        // Configure how many channels are used for each adc
        ret = MI125_ltm9012_setchannelmode(&board->adc, chact[jj], MI125_COM_ADC0 + jj);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}
    }

    // Apply external adc shutdown for the unused adc devices
    if (board->beta)
        ret = MI125_pca9535_setshutdown(&board->expander2, actchannel);
    else
        ret = MI125_pcf8574_setshutdown(&board->expander, actchannel);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Configure the hardware with the number of active group
    board->core->Register_mi125ctrl.Bit.ChannelConfig = config->groupch;

    // Valid lvds settings
    switch(config->lvds)
    {
        case MI125_TERMOFF1750UA:
        case MI125_TERMOFF2100UA:
        case MI125_TERMOFF2500UA:
        case MI125_TERMOFF3000UA:
        case MI125_TERMOFF3500UA:
        case MI125_TERMOFF4000UA:
        case MI125_TERMOFF4500UA:
        case MI125_TERMON1750UA:
        case MI125_TERMON2100UA:
        case MI125_TERMON2500UA:
            ilvds = config->lvds;
            break;
        default:
            // Invalid lvds configuration
            FUNC_EXIT();
            return(MI125_MI125INVALIDLVDS);
    }

    // Valid randomize feature
    switch(config->randmode)
    {
        case MI125_RANDOMIZEOFF:
            irand = config->randmode;

            // Set fpga to match adc randomize mode OFF
            board->core->Register_mi125ctrl.Bit.DigOutRandEn = 0;

            // Save fpga randomize mode
            board->randommode = 0;
            break;
        case MI125_RANDOMIZEON:
            irand = config->randmode;

            // Set fpga to match adc randomize mode ON
            board->core->Register_mi125ctrl.Bit.DigOutRandEn = 1;

            // Save fpga randomize mode
            board->randommode = 1;
            break;
        default:
            // Invalid output randomize mode
            FUNC_EXIT();
            return(MI125_MI125INVALIDRAN);
    }

    // Valid output binary mode feature
    switch(config->binmode)
    {
        case MI125_OFFSETBINARYFORMAT:
            ibin = config->binmode;

            // Set fpga to match adc data format to record correctly in memory
            board->core->Register_mi125ctrl.Bit.DataFormat = 0;
            break;
        case MI125_TWOCOMPLEMENTFORMAT:
            ibin = config->binmode;

            // Set fpga to match adc data format to record correctly in memory
            board->core->Register_mi125ctrl.Bit.DataFormat = 1;
            break;
        default:
            // Invalid output binary format mode
            FUNC_EXIT();
            return(MI125_MI125INVALIDBIN);
    }

    // Configure each adc for the channels used
    for (jj = 0; jj <= config->groupch; jj++)
    {
        // Configure how many channels are used for each adc
        ret = MI125_ltm9012_setconfig(&board->adc, irand, ibin, ilvds,  MI125_COM_ADC0 + jj);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}
    }

    // Check if we added more channels to the group, and so need to redo the digital calibration
    if (config->groupch > board->groupch)
    {
        // Save the number of channels in the group
        board->groupch = config->groupch;

        // If new channels are added, their adc must stabilize 1st by going out of sleep mode
        (*board->waitusec)(MI125_ADCSLEEPOUT_DELAY);

        // Digital output calibration
        ret = MI125_mi125_digital_adccalibration(board);
    }
    else
        // Save the number of channels in the group
        board->groupch = config->groupch;

    FUNC_EXIT();

    return(ret);
}


 /****************************************************************************
 * NAME : MI125_mi125_set_clksrc
 ************************************************************************//**
 *
 * Configure the clock source that the boards ADC will used for all channels.
 * This function is optionnal and can be used if the default from MI125_mi125_reset()
 * for board operation is not what is desired.
 * The options that can be configured are: internal 125MHz, external clock,
 * for the TOP position adapter, it could also be clocked from the bottom adapter
 * to allow synchronize clocking for an adapter pair.
 *
 * @warning
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - The module MI125_mi125_reset() function must be call at least once before using this function.
 *
 * @attention
 * If a module instance is not the clock master (see MI125_mi125_get_clkmaster()),
 * (example the top adapter), its clock selection could only be #MI125_CLKSRCBOTTOMFMC; in this
 * situation, the bottom module must be configured to select the correct clock source input.
 * A change of clock for the clock master implies that a calibration must be done also for the
 * module which is not the clock master.
 *
 * @param [in] board
 *    This structure is the one returned at module opening with MI125_mi125_open()
 *
 * @param [in] clksrc
 *    This is the desired clock source.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open(), MI125_mi125_reset().
 *
 * Example:
 * @code
 * // ...
 *
 * // Perseus base address
 * #define PERSEUS_BASE_ADDR_REG (XPAR_AXI_PERSEUS6010_REGS_BASEADDR)
 *
 * // Perseus I2C bus release control register
 * #define PERSEUS_I2C_RELEASE_REG  ((MI125_i2c_release *)(PERSEUS_BASE_ADDR_REG + 0xA0))
 *
 * // MI125 core addresses
 * #define MI125_BOTTOM_ADDRESS ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_BOTTOM_BASEADDR))
 * #define MI125_TOP_ADDRESS    ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_TOP_BASEADDR))
 *
 * // I2C module instance in perseus
 * #define PERSEUS_I2C_MODULE (XPAR_FMC_I2C_BASEADDR)
 * // ...
 * {
 *  Result ret;
 *  MI125_mi125 board[MI125_NBMAX];
 *  MI125_mi125_i2caddr i2c_dev_list[MI125_NBMAX]={MI125_I2CADR_MAIN_GA00,MI125_I2CADR_SEC_GA00}; // GA=00 for perseus
 *  MI125_mi125_Register_Map *mi125core[MI125_NBMAX]={MI125_BOTTOM_ADDRESS,MI125_TOP_ADDRESS};
 *  int lastboard = MI125_NBMAX;
 *  int boardtype[MI125_NBMAX]={0,1},inst;
 *  MI125_mi125_fmc_position boardposition[MI125_NBMAX]={MI125_BOTTOM,MI125_TOP};
 *  MI125_mi125_clock_source clksrc[MI125_NBMAX] = {MI125_CLKSRC125MHZ,MI125_CLKSRCBOTTOMFMC};
 *
 *  // For each board instance that we have detected
 *  for (inst=0; inst<lastboard; inst++)
 *  {
 *      // Open the mi125 module instance and provides our own timer functions (no OS available to provide them)
 *  	ret = MI125_mi125_open(&board[inst], mi125core[inst], PERSEUS_I2C_MODULE, PERSEUS_I2C_RELEASE_REG,
 *                             &i2c_dev_list[boardtype[inst]],
 *                             timer_waitusec,
 *                             boardposition[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("MI125_mi125_open FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *       //Reset and setup MI125 with basic default configuration (after module powerup, all 16 channels)
 *      ret = MI125_mi125_reset(&board[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Reset FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *      // Setup ADC clock source (needed if clock source is different than default as provided with reset)
 *      ret = MI125_mi125_set_clksrc(&board[inst],clksrc[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Clk source FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *  }
 *  // ... Close the module when not needed
 * }
 * @endcode
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_reset, MI125_mi125_close, MI125_mi125_get_clkmaster.
 *
 ***************************************************************************/
Result MI125_mi125_set_clksrc(MI125_mi125 * board,MI125_mi125_clock_source clksrc)
{
    MI125_pcf8574_clksrc inclksrc;
    Result ret = OK;

    FUNC_ENTER();

    // Check if the board has been reset correctly at least once
    if (!board->resetdone)
    {
        FUNC_EXIT();
        return(MI125_MI125NORESET);
    }

    // For any board type we could have at bottom position, the clk source could not be bottom fmc
    // This is only possible for top adapter
    if ((board->fmcposition == MI125_BOTTOM && clksrc == MI125_CLKSRCBOTTOMFMC))
    {
        FUNC_EXIT();
        return(MI125_MI125INVALIDCLKBOT);
    }

    // For top board, the clk source could not be different than MI125_CLKSRCBOTTOMFMC
    // if the top board is not the clock master.
    if ((board->fmcposition != MI125_BOTTOM && clksrc != MI125_CLKSRCBOTTOMFMC) &&
        board->core->Register_mi125status.Bit.MI125ClkMaster == 0)
    {
        FUNC_EXIT();
        return(MI125_MI125INVALIDCLKTOP);
    }

    // Validates the clock src
    switch(clksrc)
    {
        case MI125_CLKSRC125MHZ:
        case MI125_CLKSRCEXT:
        case MI125_CLKSRCBOTTOMFMC:
        case MI125_CLKSRCFMCCARRIER:
            inclksrc = clksrc;
            break;

        default:
            // Invalid clock source
            FUNC_EXIT();
            return(MI125_MI125INVALIDCLK);
    }

    // Change the clksrc according to board type
    if (board->beta)
    {
        MI125_pca9535_localoscsw sw;

        // Check if we don't use the internal oscillator and turn it off to lower noise
        if (clksrc == MI125_CLKSRC125MHZ)
        {
            sw = MI125_PCA9535_LOSWON;
            ret = MI125_pca9535_setlocaloscsw(&board->expander2, sw);
            if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

            // Make sure the local oscillator has started and stabilized
            (*board->waitusec)(MI125_LOCALOSC_DELAY);
        }
        else
        {
            sw = MI125_PCA9535_LOSWOFF;
            ret = MI125_pca9535_setlocaloscsw(&board->expander2, sw);
            if(FAILURE(ret)) {FUNC_EXIT(); return ret;}
        }

        ret = MI125_pca9535_setclksource(&board->expander2, inclksrc);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}
        ret = _MI125_mi125_setclkbuf14fanout(board);
    }
    else
        ret = MI125_pcf8574_setclksource(&board->expander, inclksrc);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Reset the clock mcm before the calibration
    ret = MI125_mi125_clockreset(board);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Digital output calibration must be done again because of clock change
    ret = MI125_mi125_digital_adccalibration(board);

    FUNC_EXIT();

    return(ret);
}


 /****************************************************************************
 * NAME : MI125_mi125_set_trigout
 ************************************************************************//**
 *
 * Configure the trigger out IO to be disconnected (default) or connected
 * from FPGA to outside.
 * The options that can be configured are: trigger out enable or disable.
 *
 * @warning
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - The module MI125_mi125_reset() function must be call at least once before using this function.
 * - Because the trigger out function is shared with the trigger in pin,
 *   the trig out functionnality must be disable if the external trig pin is used for
 *   trigger input.
 *
 * @param [in] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *
 * @param [in] trigout
 *    This is the desired trigger out configuration (see #MI125_mi125_trigout).
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open(), MI125_mi125_reset().
 *
 * Example:
 * @code
 * // ...
 *
 * // Perseus base address
 * #define PERSEUS_BASE_ADDR_REG (XPAR_AXI_PERSEUS6010_REGS_BASEADDR)
 *
 * // Perseus I2C bus release control register
 * #define PERSEUS_I2C_RELEASE_REG  ((MI125_i2c_release *)(PERSEUS_BASE_ADDR_REG + 0xA0))
 *
 * // MI125 core addresses
 * #define MI125_BOTTOM_ADDRESS ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_BOTTOM_BASEADDR))
 * #define MI125_TOP_ADDRESS    ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_TOP_BASEADDR))
 *
 * // I2C module instance in perseus
 * #define PERSEUS_I2C_MODULE (XPAR_FMC_I2C_BASEADDR)
 * // ...
 * {
 *  Result ret;
 *  MI125_mi125 board[MI125_NBMAX];
 *  MI125_mi125_i2caddr i2c_dev_list[MI125_NBMAX]={MI125_I2CADR_MAIN_GA00,MI125_I2CADR_SEC_GA00}; // GA=00 for perseus
 *  MI125_mi125_Register_Map *mi125core[MI125_NBMAX]={MI125_BOTTOM_ADDRESS,MI125_TOP_ADDRESS};
 *  int lastboard = MI125_NBMAX;
 *  int boardtype[MI125_NBMAX]={0,1},inst;
 *  MI125_mi125_fmc_position boardposition[MI125_NBMAX]={MI125_BOTTOM,MI125_TOP};
 *  MI125_mi125_trigout trigout[MI125_NBMAX] = {MI125_TRIGOUTON,MI125_TRIGOUTOFF};
 *
 *  // For each board instance that we have detected
 *  for (inst=0; inst<lastboard; inst++)
 *  {
 *      // Open the mi125 module instance and provides our own timer functions (no OS available to provide them)
 *  	ret = MI125_mi125_open(&board[inst], mi125core[inst], PERSEUS_I2C_MODULE, PERSEUS_I2C_RELEASE_REG,
 *                             &i2c_dev_list[boardtype[inst]],
 *                             timer_waitusec,
 *                             boardposition[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("MI125_mi125_open FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *       //Reset and setup MI125 with basic default configuration (after module powerup, all 16 channels)
 *      ret = MI125_mi125_reset(&board[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Reset FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *      // Setup board trigout function (do not connect an externel trig source on the external trig pin at the same time)
 *      // To use the trigout function, it must be connected somewhere in the FPGA user logic (not provided by default).
 *      ret = MI125_mi125_set_trigout(&board[inst],trigout[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Trigout FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *  }
 *  // ... Close the module when not needed
 * }
 * @endcode
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_reset, MI125_mi125_close.
 *
 ***************************************************************************/
Result MI125_mi125_set_trigout(MI125_mi125 * board, MI125_mi125_trigout trigout)
{
    Result ret = OK;
    MI125_pcf8574_trigout iout;

    FUNC_ENTER();

    // Check if the board has been reset correctly at least once
    if (!board->resetdone)
    {
        FUNC_EXIT();
        return(MI125_MI125NORESET);
    }

    // Validates trigger out
    switch(trigout)
    {
        case MI125_TRIGOUTON:
        case MI125_TRIGOUTOFF:
            iout = trigout;
            break;

        default:
            // Invalid trigger out configuration
            FUNC_EXIT();
            return(MI125_MI125INVALIDTRIG);
    }

    // Change the trig out configuration
    if (board->beta)
        // Trigout configuration is inverted as compared to original alpha board
        ret = MI125_pca9535_settrgout(&board->expander2, (iout ^ 1));
    else
        ret = MI125_pcf8574_settrgout(&board->expander, iout);

    FUNC_EXIT();

    return(ret);
}


 /****************************************************************************
 * NAME : MI125_mi125_set_testmode
 ************************************************************************//**
 *
 * This function could be used to configure special test mode (mostly use at production time
 * for test purpose).
 * Here are some details about possible test modes:
 *
 * #MI125_TESTMODEOFF:
 * All test modes are disabled.
 *
 * #MI125_TESTMODE2:
 * When test mode is active, each adc channel would be captured with user specified data pattern.
 * This is usefull to verify the reliability of the complete capture chain.
 * Only the 14 LSB bits of specified pattern are used for the test (2 MSB bits are force to 0)
 * The module MI125_mi125_digital_adccalibration() must have been done successfully last time
 * before using this function (this requierement is met after a successfull MI125_mi125_reset()).
 *
 * #MI125_TESTMODE1:
 * This mode is similar as #MI125_TESTMODE2 explained above, but is mainly reserved for
 * production purpose.
 * @attention
 * The adc data captured in this mode is not necessary the specified pattern
 * because it could be done uncalibrated (so this mode could not be used to verify the
 * reliability of the complete captured chain).
 *
 * For normal board ADC operation, test mode must be disable after use.
 *
 * @warning
 * - Doing MI125_mi125_digital_adccalibration(), MI125_mi125_set_clksrc(), or MI125_mi125_set_config()
 *   when adding more channels, will disable the test mode automatically and revert to normal mode.
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - The module MI125_mi125_reset() function must be call at least once before using this function.
 *
 * @param [in] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *
 * @param [in] test
 *    This is to configure what test mode to use or no test mode (see #MI125_mi125_testmode).
 *
 * @param [in] pattern
 *    14 bits test pattern used (2 MSB bits are ignored and force to 0).
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open(), MI125_mi125_reset().
 *
 * Example:
 * @code
 * // ...
 *
 * // Perseus base address
 * #define PERSEUS_BASE_ADDR_REG (XPAR_AXI_PERSEUS6010_REGS_BASEADDR)
 *
 * // Perseus I2C bus release control register
 * #define PERSEUS_I2C_RELEASE_REG  ((MI125_i2c_release *)(PERSEUS_BASE_ADDR_REG + 0xA0))
 *
 * // MI125 core addresses
 * #define MI125_BOTTOM_ADDRESS ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_BOTTOM_BASEADDR))
 * #define MI125_TOP_ADDRESS    ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_TOP_BASEADDR))
 *
 * // I2C module instance in perseus
 * #define PERSEUS_I2C_MODULE (XPAR_FMC_I2C_BASEADDR)
 * // ...
 * {
 *  Result ret;
 *  MI125_mi125 board[MI125_NBMAX];
 *  MI125_mi125_i2caddr i2c_dev_list[MI125_NBMAX]={MI125_I2CADR_MAIN_GA00,MI125_I2CADR_SEC_GA00}; // GA=00 for perseus
 *  MI125_mi125_Register_Map *mi125core[MI125_NBMAX]={MI125_BOTTOM_ADDRESS,MI125_TOP_ADDRESS};
 *  int lastboard = MI125_NBMAX;
 *  int boardtype[MI125_NBMAX]={0,1},inst;
 *  MI125_mi125_fmc_position boardposition[MI125_NBMAX]={MI125_BOTTOM,MI125_TOP};
 *  unsigned short mpattern[MI125_NBMAX] = {0x33C,0x33C};
 *
 *  // For each board instance that we have detected
 *  for (inst=0; inst<lastboard; inst++)
 *  {
 *      // Open the mi125 module instance and provides our own timer functions (no OS available to provide them)
 *  	ret = MI125_mi125_open(&board[inst], mi125core[inst], PERSEUS_I2C_MODULE, PERSEUS_I2C_RELEASE_REG,
 *                             &i2c_dev_list[boardtype[inst]],
 *                             timer_waitusec,
 *                             boardposition[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("MI125_mi125_open FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *       //Reset and setup MI125 with basic default configuration (after module powerup, all 16 channels)
 *      ret = MI125_mi125_reset(&board[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Reset FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *      // If we want to enable adc test mode by using this test pattern: 0x33C
 *      ret = MI125_mi125_set_testmode(&board[inst], MI125_TESTMODE2, mpattern[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Test mode FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *      // ...
 *
 *      // Disable test mode when not needed anymore (pattern is dummy here)
 *      ret = MI125_mi125_set_testmode(&board[inst], MI125_TESTMODEOFF, mpattern[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Test mode disable FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *  }
 *  // ... Close the module when not needed
 * }
 * @endcode
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_reset, MI125_mi125_close, MI125_mi125_digital_adccalibration,
 *    MI125_mi125_set_clksrc, MI125_mi125_set_config.
 *
 ***************************************************************************/
Result MI125_mi125_set_testmode(MI125_mi125 * board, MI125_mi125_testmode test, unsigned short pattern)
{
    Result ret = OK;
    int jj;
    MI125_ltm9012_dcalib calib;

    FUNC_ENTER();

    // Check if the board has been reset correctly at least once
    if (!board->resetdone)
    {
        FUNC_EXIT();
        return(MI125_MI125NORESET);
    }

    // Check if the last board calibration was successfull: this is critical for the test mode
    // MI125_TESTMODE2 to work correctly
    if (!board->calibdone && test == MI125_TESTMODE2)
    {
        FUNC_EXIT();
        return(MI125_MI125NOCALIB);
    }

    // Check for test mode parameter
    switch(test)
    {
        case MI125_TESTMODE1:
        case MI125_TESTMODE2:
            calib = MI125_LTM9012_DCALIBON;
            board->testmode = test;

            // Set fpga to match adc randomize mode OFF
            board->core->Register_mi125ctrl.Bit.DigOutRandEn = 0;
            break;
        case MI125_TESTMODEOFF:
            calib = MI125_LTM9012_DCALIBOFF;
            board->testmode = test;

            // Set fpga to match adc saved randomize mode
            board->core->Register_mi125ctrl.Bit.DigOutRandEn = board->randommode;
            break;

        default:
            // Invalid test mode setting
            FUNC_EXIT();
            return(MI125_MI125INVALIDTEST);
    }

    // Setup ADC correctly with test pattern and calib test mode
    // Setup each adc
    for (jj = 0; jj <= board->groupch; jj++)
    {
        // Set adc in calib mode
        ret = MI125_ltm9012_setdigitalcalib(&board->adc, calib, pattern, MI125_COM_ADC0 + jj);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}
    }

    FUNC_EXIT();

    return ret;
}


 /****************************************************************************
 * NAME : MI125_mi125_get_temperature
 ************************************************************************//**
 *
 * Read board pcb temperature in specified mode (resolution of 1C or 0.1C).
 *
 * @attention
 * Board temperature is for information only. Different locations
 * on the board and environment conditions could affect the difference of temperature
 * seen on the board (like ADCs temperature). User is responsible to keep the
 * board temperature inside the allowed operating condition.
 *
 * @warning
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - The module MI125_mi125_reset() function must be call at least once before using this function.
 *
 * @param [in] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *
 * @param [in] mode
 *    This is to select the temperature reading mode (resolution of 1C or 0.1C)
 *    (see #MI125_mi125_tempmode).
 *
 * @param [out] temp
 *    Returns the read temperature in 2 compl format. If the choosen mode is 1C resolution,
 *    the value is directly the temp in C. If the mode is #MI125_TEMP0DOT1C, the temperature
 *    returned is X 10 to allow returning an extra 0.x digit in an integer value.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open(), MI125_mi125_reset().
 *
 * Example:
 * @code
 * // ...
 *
 * // Perseus base address
 * #define PERSEUS_BASE_ADDR_REG (XPAR_AXI_PERSEUS6010_REGS_BASEADDR)
 *
 * // Perseus I2C bus release control register
 * #define PERSEUS_I2C_RELEASE_REG  ((MI125_i2c_release *)(PERSEUS_BASE_ADDR_REG + 0xA0))
 *
 * // MI125 core addresses
 * #define MI125_BOTTOM_ADDRESS ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_BOTTOM_BASEADDR))
 * #define MI125_TOP_ADDRESS    ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_TOP_BASEADDR))
 *
 * // I2C module instance in perseus
 * #define PERSEUS_I2C_MODULE (XPAR_FMC_I2C_BASEADDR)
 * // ...
 * {
 *  Result ret;
 *  MI125_mi125 board[MI125_NBMAX];
 *  MI125_mi125_i2caddr i2c_dev_list[MI125_NBMAX]={MI125_I2CADR_MAIN_GA00,MI125_I2CADR_SEC_GA00}; // GA=00 for perseus
 *  MI125_mi125_Register_Map *mi125core[MI125_NBMAX]={MI125_BOTTOM_ADDRESS,MI125_TOP_ADDRESS};
 *  int lastboard = MI125_NBMAX;
 *  int boardtype[MI125_NBMAX]={0,1},inst;
 *  MI125_mi125_fmc_position boardposition[MI125_NBMAX]={MI125_BOTTOM,MI125_TOP};
 *  short temp[MI125_NBMAX];
 *
 *  // For each board instance that we have detected
 *  for (inst=0; inst<lastboard; inst++)
 *  {
 *      // Open the mi125 module instance and provides our own timer functions (no OS available to provide them)
 *  	ret = MI125_mi125_open(&board[inst], mi125core[inst], PERSEUS_I2C_MODULE, PERSEUS_I2C_RELEASE_REG,
 *                             &i2c_dev_list[boardtype[inst]],
 *                             timer_waitusec,
 *                             boardposition[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("MI125_mi125_open FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *       //Reset and setup MI125 with basic default configuration (after module powerup, all 16 channels)
 *      ret = MI125_mi125_reset(&board[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Reset FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *      // Display the current MI125 pcb temperature in 0.1C resolution
 *      // WARNING, this temperature is for information only. This does not
 *      // give ADC core temperature information. User must make sure that
 *      // temperature of MI125 board (incl ADC) meet the requierements in his
 *      // system. Some calibration could be needed between this value
 *      // and different places/parts on the MI125 board.
 *      ret = MI125_mi125_get_temperature(&board[inst], MI125_TEMP0DOT1C, &temp[inst]);
 *      if(SUCCESS(ret))
 *      {
 *          printf("MI125 pcb board #%d temp: %d.%01dC\n",inst, temp[inst]/10,abs(temp[inst]%10));
 *      }else{
 *          printf("Get Temperature FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *  }
 *  // ... Close the module when not needed
 * }
 * @endcode
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_reset, MI125_mi125_close.
 *
 ***************************************************************************/
Result MI125_mi125_get_temperature(MI125_mi125 * board, MI125_mi125_tempmode mode, short* temp)
{
    Result ret = OK;

    FUNC_ENTER();

    // Check if the board has been reset correctly at least once
    if (!board->resetdone)
    {
        FUNC_EXIT();
        return(MI125_MI125NORESET);
    }

    // Read pcb temperature
    ret = MI125_lm75_readtemp10x(&board->monitor, temp);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // If temperature mode is rounding to Celsius
    if (mode == MI125_TEMP1C)
        *temp = *temp / 10;

    FUNC_EXIT();

    return(ret);
}


 /****************************************************************************
 * NAME : MI125_mi125_get_channelcalibstatus
 ************************************************************************//**
 *
 * Read ADC channels calibration information. This info was the last time the
 * board ADC were calibrated (with MI125_mi125_digital_adccalibration()), manually
 * by user, or internally by other module functions.
 *
 * @warning
 * The module must be opened with MI125_mi125_open() before using this function.
 *
 * @param [in] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *
 * @param [out] channellanecalib
 *    When channelcalibstatus=0, there was an
 *    error and channellanecalib shows all digital lanes calibration status, starting
 *    LSB bits for 1st adc 1st channel (2 lvds lanes each channel). All bits at 1 shows
 *    channels lanes that calibrated correctly, others are in problem. ADC channels
 *    which are not configured (unused) will show there lanes set to 0 (uncalibrated).
 *
 * @param [out] channelcalibstatus
 *    When !=0, this means the channels calibration was successfull. If 0, there was an
 *    error and channellanecalib shows all digital lanes calibration status, starting
 *    LSB bits for 1st adc 1st channel (2 lvds lanes each channel). All bits at 1 shows
 *    channels lanes that calibrated correctly, others are in problem.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open().
 *
 * Example:
 * @code
 * // ...
 *
 * // Perseus base address
 * #define PERSEUS_BASE_ADDR_REG (XPAR_AXI_PERSEUS6010_REGS_BASEADDR)
 *
 * // Perseus I2C bus release control register
 * #define PERSEUS_I2C_RELEASE_REG  ((MI125_i2c_release *)(PERSEUS_BASE_ADDR_REG + 0xA0))
 *
 * // MI125 core addresses
 * #define MI125_BOTTOM_ADDRESS ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_BOTTOM_BASEADDR))
 * #define MI125_TOP_ADDRESS    ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_TOP_BASEADDR))
 *
 * // I2C module instance in perseus
 * #define PERSEUS_I2C_MODULE (XPAR_FMC_I2C_BASEADDR)
 * // ...
 * {
 *  Result ret;
 *  MI125_mi125 board[MI125_NBMAX];
 *  MI125_mi125_i2caddr i2c_dev_list[MI125_NBMAX]={MI125_I2CADR_MAIN_GA00,MI125_I2CADR_SEC_GA00}; // GA=00 for perseus
 *  MI125_mi125_Register_Map *mi125core[MI125_NBMAX]={MI125_BOTTOM_ADDRESS,MI125_TOP_ADDRESS};
 *  int lastboard = MI125_NBMAX;
 *  int boardtype[MI125_NBMAX]={0,1},inst;
 *  MI125_mi125_fmc_position boardposition[MI125_NBMAX]={MI125_BOTTOM,MI125_TOP};
 *  unsigned int channellanecalib[MI125_NBMAX], channelcalibstatus[MI125_NBMAX];
 *
 *  // For each board instance that we have detected
 *  for (inst=0; inst<lastboard; inst++)
 *  {
 *      // Open the mi125 module instance and provides our own timer functions (no OS available to provide them)
 *  	ret = MI125_mi125_open(&board[inst], mi125core[inst], PERSEUS_I2C_MODULE, PERSEUS_I2C_RELEASE_REG,
 *                             &i2c_dev_list[boardtype[inst]],
 *                             timer_waitusec,
 *                             boardposition[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("MI125_mi125_open FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *       //Reset and setup MI125 with basic default configuration (after module powerup, all 16 channels)
 *      ret = MI125_mi125_reset(&board[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Reset FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *      // Display ADC channels lanes calibration information
 *      ret = MI125_mi125_get_channelcalibstatus(&board[inst], &channellanecalib[inst], &channelcalibstatus[inst]);
 *      if(SUCCESS(ret))
 *      {
 *          // Check if the last channels calibration was ok
 *          if (channelcalibstatus[inst])
 *          {
 *              printf("Board #%d, all channels calibration successfull!\n",inst);
 *          }
 *          else
 *          {
 *              printf("Board #%d, calibration problem, lane status: %08X\n", inst, channellanecalib[inst]);
 *          }
 *      }else{
 *          printf("Get Calibration FAILED: 0x%08X\n",ret);
 *           return (ret);
 *      }
 *  }
 *  // ... Close the module when not needed
 * }
 * @endcode
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_reset, MI125_mi125_close, MI125_mi125_digital_adccalibration.
 *
 ***************************************************************************/
Result MI125_mi125_get_channelcalibstatus(MI125_mi125 * board, unsigned int* channellanecalib, unsigned int* channelcalibstatus)
{
    int indx;

    FUNC_ENTER();

    // Get the current lane calibration status for the active channels
    *channellanecalib = (board->channelslanecalib[board->calibndx] & board->channellanemask);

#if MI125_MODDEBUG
    for (indx = 0; indx <= board->calibndx; indx++)
            PDEBUG("Trial #%02d, calib status: 0x%08X.\n",indx, (board->channelslanecalib[indx] & board->channellanemask));
#endif

    // Keep trace if calib was made correctly or not
    *channelcalibstatus = board->calibdone;

    FUNC_EXIT();

    return(MI125_MI125OK);
}


 /****************************************************************************
 * NAME : MI125_mi125_get_versions
 ************************************************************************//**
 *
 * Read mi125 fpga core module version and driver version information.
 * This info could be used for diagnostic purpose.
 *
 * @warning
 * - The module must be opened with MI125_mi125_open() before using this function.
 *
 * @param [in] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *
 * @param [out] core_version
 *    Returns MI125 fpga core version.
 *
 * @param [out] drv_version
 *    Returns MI125 software driver version.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open().
 *
 * Example:
 * @code
 * // ...
 *
 * // Perseus base address
 * #define PERSEUS_BASE_ADDR_REG (XPAR_AXI_PERSEUS6010_REGS_BASEADDR)
 *
 * // Perseus I2C bus release control register
 * #define PERSEUS_I2C_RELEASE_REG  ((MI125_i2c_release *)(PERSEUS_BASE_ADDR_REG + 0xA0))
 *
 * // MI125 core addresses
 * #define MI125_BOTTOM_ADDRESS ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_BOTTOM_BASEADDR))
 * #define MI125_TOP_ADDRESS    ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_TOP_BASEADDR))
 *
 * // I2C module instance in perseus
 * #define PERSEUS_I2C_MODULE (XPAR_FMC_I2C_BASEADDR)
 * // ...
 * {
 *  Result ret;
 *  MI125_mi125 board[MI125_NBMAX];
 *  MI125_mi125_i2caddr i2c_dev_list[MI125_NBMAX]={MI125_I2CADR_MAIN_GA00,MI125_I2CADR_SEC_GA00}; // GA=00 for perseus
 *  MI125_mi125_Register_Map *mi125core[MI125_NBMAX]={MI125_BOTTOM_ADDRESS,MI125_TOP_ADDRESS};
 *  int lastboard = MI125_NBMAX;
 *  int boardtype[MI125_NBMAX]={0,1},inst;
 *  MI125_mi125_fmc_position boardposition[MI125_NBMAX]={MI125_BOTTOM,MI125_TOP};
 *  unsigned int core_version[MI125_NBMAX], drv_version[MI125_NBMAX];
 *
 *  // For each board instance that we have detected
 *  for (inst=0; inst<lastboard; inst++)
 *  {
 *      // Open the mi125 module instance and provides our own timer functions (no OS available to provide them)
 *  	ret = MI125_mi125_open(&board[inst], mi125core[inst], PERSEUS_I2C_MODULE, PERSEUS_I2C_RELEASE_REG,
 *                             &i2c_dev_list[boardtype[inst]],
 *                             timer_waitusec,
 *                             boardposition[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("MI125_mi125_open FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *      // Display MI125 hardware core and driver versions (for info only)
 *      ret = MI125_mi125_get_versions(&board[inst], &core_version[inst], &drv_version[inst]);
 *      if(SUCCESS(ret))
 *      {
 *          printf("MI125 Core Version: 0x%04X, Driver Version: 0x%04X\n" ,core_version[inst], drv_version[inst]);
 *      }else{
 *          printf("Versions FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *  }
 *  // ... Close the module when not needed
 * }
 * @endcode
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_close.
 *
 ***************************************************************************/
Result MI125_mi125_get_versions(MI125_mi125 * board, unsigned int* core_version, unsigned int *drv_version)
{

    FUNC_ENTER();

    // Get the MI125 hardware core version
    *core_version = board->core->Register_mi125info.Bit.version;

    // Get the MI125 software driver version
    *drv_version = MI125_DVERSION;

    FUNC_EXIT();

    return(MI125_MI125OK);
}


 /****************************************************************************
 * NAME : MI125_mi125_checkadc
 ************************************************************************//**
 *
 * Check ADC communication for diagnostic purpose.
 * For each board ADC that is verified, this function reports an error if there is a problem.
 * It will also returns the written test pattern that caused problem and what verified
 * value was read.
 *
 * @warning
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - MI125_mi125_reset() function must be called before using this function to reset the board correctly.
 * - Even if the MI125_mi125_reset() function fails, an attempt to call MI125_mi125_checkadc() can be made later.
 * - Test mode must not be active before using this function (see MI125_mi125_set_testmode()).
 *
 * @param [in] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *
 * @param [out] written
 *    Returns written test pattern
 *
 * @param [out] verify
 *    Returns verified test pattern (should be the same as written when no error).
 *
 * @param [out] detected
 *    return a value =0 if the tested adc was not detected correctly, !=0 if it was OK.
 *
 * @param [in] adcid
 *    Which ADC we want to verify (#MI125_ADC0, #MI125_ADC1, #MI125_ADC2 or #MI125_ADC3).
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open(), MI125_mi125_reset().
 *
 * Example:
 * @code
 * // ...
 *
 * // Perseus base address
 * #define PERSEUS_BASE_ADDR_REG (XPAR_AXI_PERSEUS6010_REGS_BASEADDR)
 *
 * // Perseus I2C bus release control register
 * #define PERSEUS_I2C_RELEASE_REG  ((MI125_i2c_release *)(PERSEUS_BASE_ADDR_REG + 0xA0))
 *
 * // MI125 core addresses
 * #define MI125_BOTTOM_ADDRESS ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_BOTTOM_BASEADDR))
 * #define MI125_TOP_ADDRESS    ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_TOP_BASEADDR))
 *
 * // I2C module instance in perseus
 * #define PERSEUS_I2C_MODULE (XPAR_FMC_I2C_BASEADDR)
 * // ...
 * {
 *  Result ret;
 *  MI125_mi125 board[MI125_NBMAX];
 *  MI125_mi125_i2caddr i2c_dev_list[MI125_NBMAX]={MI125_I2CADR_MAIN_GA00,MI125_I2CADR_SEC_GA00}; // GA=00 for perseus
 *  MI125_mi125_Register_Map *mi125core[MI125_NBMAX]={MI125_BOTTOM_ADDRESS,MI125_TOP_ADDRESS};
 *  int lastboard = MI125_NBMAX;
 *  int boardtype[MI125_NBMAX]={0,1},inst;
 *  MI125_mi125_fmc_position boardposition[MI125_NBMAX]={MI125_BOTTOM,MI125_TOP};
 *
 *  // For each board instance that we have detected
 *  for (inst=0; inst<lastboard; inst++)
 *  {
 *      // Open the mi125 module instance and provides our own timer functions (no OS available to provide them)
 *  	ret = MI125_mi125_open(&board[inst], mi125core[inst], PERSEUS_I2C_MODULE, PERSEUS_I2C_RELEASE_REG,
 *                             &i2c_dev_list[boardtype[inst]],
 *                             timer_waitusec,
 *                             boardposition[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("MI125_mi125_open FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *       //Reset and setup MI125 with basic default configuration (after module powerup, all 16 channels)
 *      ret = MI125_mi125_reset(&board[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Reset FAILED: 0x%08X\n",ret);
 *          //return (ret);
 *      }
 *
 *       // This is an optionnal communication test with each ADC
 *       {
 *           unsigned char written, verify, detected, count;
 *           MI125_mi125_adcid adcn[]={MI125_ADC0,MI125_ADC1,MI125_ADC2,MI125_ADC3};
 *
 *       	// Test each adc
 *           for(count=0;count<4;count++)
 *       	{
 *              ret = MI125_mi125_checkadc(&board[inst], &written, &verify, &detected, adcn[count]);
 *              if(SUCCESS(ret))
 *              {
 *                   printf("MI125 pcb board #%d ADC #%d detected: ",inst, adcn[count]);
 *                   if (detected)
 *                       printf("yes.\n");
 *                   else
 *                       printf("no.\n");
 *              }else{
 *                   printf("MI125_mi125_checkadc FAILED: 0x%08X\n",ret);
 *                   return (ret);
 *              }
 *          }
 *       }
 *  }
 *  // ... Close the module when not needed
 * }
 * @endcode
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_reset, MI125_mi125_close, MI125_mi125_set_testmode.
 *
 ***************************************************************************/
Result MI125_mi125_checkadc(MI125_mi125 * board, unsigned char *written, unsigned char *verify, unsigned char *detected, MI125_mi125_adcid adcid)
{
    Result ret = OK;
    MI125_COM_DEVICES adcdev;

    FUNC_ENTER();

    *detected = 0;

    // Check if test mode is active
    if (board->testmode)
    {
        FUNC_EXIT();
        return(MI125_MI125TESTMODEACT);
    }

    // Initialize the com channel and the bridge if not already done
    if (board->beta)
    {
        // Because it is possible a beta board could have the bridge in reset before
        // resetting the io expander, an additionnal delay before the bridge is ready
        // could be needed to make sure that the bridge is ready for operation.
        // Attempt to initialize the com channel and the bridge (is it ready for used?);
        ret = MI125_com_init(&board->com);
        if(FAILURE(ret))
        {
            // Wait the worst time bridge reset delay
            (*board->waitusec)(MI125_BRIDGESTART_DELAY);

            // Retry to configure the bridge which now must be ready
            ret = MI125_com_init(&board->com);
        }
    }
    else
    {
        // Initialize the com channel and the bridge
        ret = MI125_com_init(&board->com);
    }
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Check for adc selected
    switch(adcid)
    {
        case MI125_ADC0:
        case MI125_ADC1:
        case MI125_ADC2:
        case MI125_ADC3:
            adcdev = adcid;
            break;

        default:
            // Invalid ADC id
            FUNC_EXIT();
            return(MI125_MI125INVALIDADC);
    }

    // Do the adc check test (to verify adc presence)
    ret = MI125_ltm9012_checkadc(&board->adc, written, verify, detected, adcdev);

    FUNC_EXIT();

    return(ret);
}


 /****************************************************************************
 * NAME : MI125_mi125_checkcore
 ************************************************************************//**
 *
 * Attempt to detect the MI125 fpga core, for diagnostic purpose or evaluation
 * if the correct fpga core was loaded.
 *
 * @warning
 * - The module must be opened with MI125_mi125_open() before using this function.
 *
 * @param [in] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *
 * @param [out] detected
 *    Returns if the mi125 core was detected or not.
 *    Detected is true (!=0) when the MI125 fpga core is detected correctly.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open().
 *
 * Example:
 * @code
 * // ...
 *
 * // Perseus base address
 * #define PERSEUS_BASE_ADDR_REG (XPAR_AXI_PERSEUS6010_REGS_BASEADDR)
 *
 * // Perseus I2C bus release control register
 * #define PERSEUS_I2C_RELEASE_REG  ((MI125_i2c_release *)(PERSEUS_BASE_ADDR_REG + 0xA0))
 *
 * // MI125 core addresses
 * #define MI125_BOTTOM_ADDRESS ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_BOTTOM_BASEADDR))
 * #define MI125_TOP_ADDRESS    ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_TOP_BASEADDR))
 *
 * // I2C module instance in perseus
 * #define PERSEUS_I2C_MODULE (XPAR_FMC_I2C_BASEADDR)
 * // ...
 * {
 *  Result ret;
 *  MI125_mi125 board[MI125_NBMAX];
 *  MI125_mi125_i2caddr i2c_dev_list[MI125_NBMAX]={MI125_I2CADR_MAIN_GA00,MI125_I2CADR_SEC_GA00}; // GA=00 for perseus
 *  MI125_mi125_Register_Map *mi125core[MI125_NBMAX]={MI125_BOTTOM_ADDRESS,MI125_TOP_ADDRESS};
 *  int lastboard = MI125_NBMAX;
 *  int boardtype[MI125_NBMAX]={0,1},inst;
 *  MI125_mi125_fmc_position boardposition[MI125_NBMAX]={MI125_BOTTOM,MI125_TOP};
 *  unsigned char detected[MI125_NBMAX];
 *
 *  // For each board instance that we have detected
 *  for (inst=0; inst<lastboard; inst++)
 *  {
 *      // Open the mi125 module instance and provides our own timer functions (no OS available to provide them)
 *  	ret = MI125_mi125_open(&board[inst], mi125core[inst], PERSEUS_I2C_MODULE, PERSEUS_I2C_RELEASE_REG,
 *                             &i2c_dev_list[boardtype[inst]],
 *                             timer_waitusec,
 *                             boardposition[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("MI125_mi125_open FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *      // Check if the correct MI125 fpga core is there for this board instance?
 *      ret = MI125_mi125_checkcore(&board[inst], &detected[inst]);
 *      if(SUCCESS(ret))
 *      {
 *          printf("Board #%d, MI125 Core " ,inst);
 *          if (detected[inst])
 *          {
 *              printf("detected.\n");
 *          }
 *          else
 *          {
 *              printf("not detected.\n");
 *              continue;
 *          }
 *      }else{
 *          printf("Checkcore FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *  }
 *  // ... Close the module when not needed
 * }
 * @endcode
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_close.
 *
 ***************************************************************************/
Result MI125_mi125_checkcore(MI125_mi125 * board, unsigned char *detected)
{
    Result ret = OK;

    FUNC_ENTER();

#ifndef SIMUL
    // Check if the core is correct MI125 type
    *detected = (board->core->Register_mi125info.Bit.coreid == MI125_COREID_MI125);
#else
    *detected = 1;
#endif

    FUNC_EXIT();

    return(ret);
}


 /****************************************************************************
 * NAME : MI125_mi125_get_clkmaster
 ************************************************************************//**
 *
 * Detect if this current module instance is a clock master for the FPGA (uses its own MMCM).
 * In a multiple modules configuration, the FPGA clock master is the one that
 * provides its own DCO clock for the ADC data captured process. Normally, only
 * the module instance which is defined as clock master can use the function related
 * to clock source selection (MI125_mi125_set_clksrc()) to select internal (#MI125_CLKSRC125MHZ),
 * external (#MI125_CLKSRCEXT) clock source, etc. If external clock source is selected for the
 * clock master module, it must be connected on the EC signal of this module connector.
 * The module which is not the FPGA clock master can only selects as an ADC clock source,
 * the other module (#MI125_CLKSRCBOTTOMFMC).
 *
 * @warning
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - In a multiple modules configuration, if there is only a single module configured as the
 *   clock master, it must be configured first.
 *
 * @param [in] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *
 * @param [out] master
 *    Returns if the mi125 core instance is the clock master or not.
 *    Master is true (!=0) when the MI125 fpga core instance is the clock master.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open().
 *
 * Example:
 * @code
 * // ...
 *
 * // Perseus base address
 * #define PERSEUS_BASE_ADDR_REG (XPAR_AXI_PERSEUS6010_REGS_BASEADDR)
 *
 * // Perseus I2C bus release control register
 * #define PERSEUS_I2C_RELEASE_REG  ((MI125_i2c_release *)(PERSEUS_BASE_ADDR_REG + 0xA0))
 *
 * // MI125 core addresses
 * #define MI125_BOTTOM_ADDRESS ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_BOTTOM_BASEADDR))
 * #define MI125_TOP_ADDRESS    ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_TOP_BASEADDR))
 *
 * // I2C module instance in perseus
 * #define PERSEUS_I2C_MODULE (XPAR_FMC_I2C_BASEADDR)
 * // ...
 * {
 *  Result ret;
 *  MI125_mi125 board[MI125_NBMAX];
 *  MI125_mi125_i2caddr i2c_dev_list[MI125_NBMAX]={MI125_I2CADR_MAIN_GA00,MI125_I2CADR_SEC_GA00}; // GA=00 for perseus
 *  MI125_mi125_Register_Map *mi125core[MI125_NBMAX]={MI125_BOTTOM_ADDRESS,MI125_TOP_ADDRESS};
 *  int lastboard = MI125_NBMAX;
 *  int boardtype[MI125_NBMAX]={0,1},inst;
 *  MI125_mi125_fmc_position boardposition[MI125_NBMAX]={MI125_BOTTOM,MI125_TOP};
 *  unsigned int master;
 *
 *  // For each board instance that we have detected
 *  for (inst=0; inst<lastboard; inst++)
 *  {
 *      // Open the mi125 module instance and provides our own timer functions (no OS available to provide them)
 *  	ret = MI125_mi125_open(&board[inst], mi125core[inst], PERSEUS_I2C_MODULE, PERSEUS_I2C_RELEASE_REG,
 *                             &i2c_dev_list[boardtype[inst]],
 *                             timer_waitusec,
 *                             boardposition[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("MI125_mi125_open FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *      // Display if this module is a clock master or not.
 *      // In a dual stack board, a module which is not a clok master for the FPGA
 *      // is using the another module as clock source
 *      ret = MI125_mi125_get_clkmaster(&board[inst], &master);
 *      if(SUCCESS(ret))
 *      {
 *          printf("MI125 pcb board #%d is a clk master: ",inst);
 *          if (master)
 *          	printf("yes.\n");
 *          else
 *          	printf("no.\n");
 *      }else{
 *          printf("MI125_mi125_get_clkmaster FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *  }
 *  // ... Close the module when not needed
 * }
 * @endcode
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_close, MI125_mi125_set_clksrc.
 *
 ***************************************************************************/
Result MI125_mi125_get_clkmaster(MI125_mi125 * board, unsigned int *master)
{
    Result ret = OK;

    FUNC_ENTER();

#ifndef SIMUL
    // Check if this fpga core instance is the clock master or not in a multiple boards configuration
    *master = board->core->Register_mi125status.Bit.MI125ClkMaster;
#else
    *master = 1;
#endif

    FUNC_EXIT();

    return(ret);
}

 /****************************************************************************
 * NAME : MI125_mi125_clockreset
 ************************************************************************//**
 *
 * Force a MCM clock reset. This function is related to the adc calibration and clock operation.
 * Each time the adc clock source is changed, this function must be used before doing and adc
 * calibration with MI125_mi125_digital_adccalibration(). While this is handled internally
 * automatically when the other module functions are used, there is a case when the clock
 * source input is configured to external clock source (or other fmc module clock source for a
 * module which is not a clock master), and it is not possible to detect when the user
 * is changing externally the clock frequency for example. For this situation, a clock reset
 * must be used before running a new adc calibration with MI125_mi125_digital_adccalibration().
 *
 * @warning
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - In a multiple modules configuration, if there is only a single module configured as the
 *   clock master, if this function is called on the clock master, the other module instance must
 *   be also recalibrated the same way as the master (see MI125_mi125_get_clkmaster()).
 *
 * @param [in] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open().
 *
 * Example:
 * @code
 * // ...
 *
 * // Perseus base address
 * #define PERSEUS_BASE_ADDR_REG (XPAR_AXI_PERSEUS6010_REGS_BASEADDR)
 *
 * // Perseus I2C bus release control register
 * #define PERSEUS_I2C_RELEASE_REG  ((MI125_i2c_release *)(PERSEUS_BASE_ADDR_REG + 0xA0))
 *
 * // MI125 core addresses
 * #define MI125_BOTTOM_ADDRESS ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_BOTTOM_BASEADDR))
 * #define MI125_TOP_ADDRESS    ((MI125_mi125_Register_Map *)(XPAR_AXI_MI125_TOP_BASEADDR))
 *
 * // I2C module instance in perseus
 * #define PERSEUS_I2C_MODULE (XPAR_FMC_I2C_BASEADDR)
 * // ...
 * {
 *  Result ret;
 *  MI125_mi125 board[MI125_NBMAX];
 *  MI125_mi125_i2caddr i2c_dev_list[MI125_NBMAX]={MI125_I2CADR_MAIN_GA00,MI125_I2CADR_SEC_GA00}; // GA=00 for perseus
 *  MI125_mi125_Register_Map *mi125core[MI125_NBMAX]={MI125_BOTTOM_ADDRESS,MI125_TOP_ADDRESS};
 *  int lastboard = MI125_NBMAX;
 *  int boardtype[MI125_NBMAX]={0,1},inst;
 *  MI125_mi125_fmc_position boardposition[MI125_NBMAX]={MI125_BOTTOM,MI125_TOP};
 *
 *  // For each board instance that we have detected
 *  for (inst=0; inst<lastboard; inst++)
 *  {
 *      // Open the mi125 module instance and provides our own timer functions (no OS available to provide them)
 *  	ret = MI125_mi125_open(&board[inst], mi125core[inst], PERSEUS_I2C_MODULE, PERSEUS_I2C_RELEASE_REG,
 *                             &i2c_dev_list[boardtype[inst]],
 *                             timer_waitusec,
 *                             boardposition[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("MI125_mi125_open FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *       //Reset and setup MI125 with basic default configuration (after module powerup, all 16 channels)
 *      ret = MI125_mi125_reset(&board[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Reset FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *
 *      // If user changed external clk frequency value (after module is already running) for this clock source mode MI125_CLKSRCEXT for example,
 *      // it will need to do a clock reset first, then a calibration.
 *      // If one module (in a multiple modules setup) is a clock master and the other is not,
 *      // if the clock is reset on the master module, the other module must be recalibrated as well...
 *      ret = MI125_mi125_clockreset(&board[inst]);
 *      if( SUCCESS(ret) )
 *      {
 *      }else{
 *          printf("MI125_mi125_clockreset error: 0x%08X\r\n",ret);
 *          return (ret);
 *      }
 *
 *  	// Calibration can be done as needed, but the clock must be reset first if the clock frequency has changed:
 *  	// this is done automatically in all cases in api functions, but not when the clock source is external which is impossible to
 *  	// control what the user is doing with the external clock.
 *      ret = MI125_mi125_digital_adccalibration(&board[inst]);
 *      if(!SUCCESS(ret))
 *      {
 *          printf("Calib FAILED: 0x%08X\n",ret);
 *          return (ret);
 *      }
 *  }
 *  // ... Close the module when not needed
 * }
 * @endcode
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_reset, MI125_mi125_close, MI125_mi125_get_clkmaster,
 *    MI125_mi125_digital_adccalibration.
 *
 ***************************************************************************/
Result MI125_mi125_clockreset(MI125_mi125 * board)
{
    Result ret = OK;
    int val;

    FUNC_ENTER();

    // Keep trace of the number of calibration trials
    board->calibndx = 0;

    // Keep trace of the channels lane that were calibrated correctly the last time it was done
    board->channelslanecalib[board->calibndx] = 0;

    // Keep trace if calib was made correctly or not
    board->calibdone = 0;

#ifndef SIMUL
    // Reset the mi125 core clock MCM
    board->core->Register_mi125ctrl.Bit.ADCClockMMCMRst = 1;

    // Wait small delay
    (*board->waitusec)(MI125_ADCSMALL_DELAY);

    // Remove core MCM clock reset
    board->core->Register_mi125ctrl.Bit.ADCClockMMCMRst = 0;

    // Check AdcClockMmcmLock register is getting to 1
    (*board->waitusec)(MI125_ADCMCMCLOCKRDY_DELAY);

    // Check if we got a timeout
    if ((val = board->core->Register_mi125status.Bit.AdcClockMmcmLock) == 0)
    {
        FUNC_EXIT();

        // Problem with signal not rising after reset
        return(MI125_MI125MMCMLOCK);
    }
#endif

    FUNC_EXIT();

    return ret;
}
