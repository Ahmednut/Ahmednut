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
 * @file       MI125_pca9535.c
 * @brief      IO expander functions
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup MI125
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

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "MI125_pca9535.h"


/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/

// Default outputs value
#define MI125_PCA9535_DEFAULT00 0xF8        // No shutdown (=1111), No spi bridge reset (=1), clk src int 125 MHz = (000)
#define MI125_PCA9535_DEFAULT01 0xD0        // reserved=1, buf clock pwrn=1, trig_out_en = 0, local osc sw = 1, config=0, load=0, soutx = 0

// Default pins direction
#define MI125_PCA9535_DEFAULT00_C 0x00      // All pins outputs
#define MI125_PCA9535_DEFAULT01_C 0x80      // All pins outputs except reserved pin input


/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/


/****************************************************************************
 *                             Local variables                              *
 ****************************************************************************/


/****************************************************************************
 *                       Private functions prototypes                       *
 ****************************************************************************/


/****************************************************************************
 *                             Private Functions                            *
 ****************************************************************************/


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


 /****************************************************************************
 *   NAME : MI125_pca9535_open
 ************************************************************************//**
 *
 *   Open the pca9535 io expander module
 *   This function must be the first one used before using this module.
 *   When the module is not used anymore, it can be closed with MI125_pca9535_close.
 *   The com_wrapper module must be opened before using this module.
 *
 *   @param [out] core
 *      This structure will hold necessary information for the module to be used
 *
 *   @param [in] com
 *      This structure must hold communication module information
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_open(MI125_pca9535_core * core, MI125_com_core * com)
{
    // Save the com module information
    core->com = com;
    
    return MI125_PCA9535OK;
}


 /****************************************************************************
 *   NAME : MI125_pca9535_reset
 ************************************************************************//**
 *
 *   Reset the pca9535 module to default operating condition
 *   This function must be called at least once to reset this module to default
 *   operating condition.
 *   The module must be opened with MI125_pca9535_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_reset(MI125_pca9535_core * core)
{
    Result ret;
    MI125_pca9535_addr caddr;
    MI125_pca9535_Register_Map reg={{{0x0000,0x0000,MI125_PCA9535_DEFAULT00,MI125_PCA9535_DEFAULT01,0x0000,0x0000,MI125_PCA9535_DEFAULT00_C,MI125_PCA9535_DEFAULT01_C}}}; // default powerup pca9535 reg values

    // Does not write PCA9535 reg#00 & #01 registers which are read only.
    for (caddr=MI125_PCA9535_02; caddr<=MI125_PCA9535_07; caddr++)
    {
        // This will reset all register bits to default pca9535 power up values (for all pins)
        ret = MI125_com_write(core->com, MI125_COM_PORTS2, caddr, &reg.RegisterArray[caddr]);
        
        // Check if we got a problem
        if(FAILURE(ret)) break;
    }
    
    return ret;
}


 /****************************************************************************
 *   NAME : MI125_pca9535_close
 ************************************************************************//**
 *
 *   Close the pca9535 io expander module
 *   This function must be the last one used after using this module.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_close(MI125_pca9535_core * core)
{
    return MI125_PCA9535OK;
}


 /****************************************************************************
 *   NAME : MI125_pca9535_setshutdown
 ************************************************************************//**
 *
 *   Configure ADC shutdown mode according to specified active channels
 *   The module must be opened with MI125_pca9535_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [in] actchannel
 *      This is the number of 4 channels group not in shutdown (active).
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_setshutdown(MI125_pca9535_core * core, MI125_pca9535_channel actchannel)
{
    Result ret;
    MI125_pca9535_addr caddr;
    MI125_pca9535_Register_Map regs;

    // Read back existing related registers content
    caddr = MI125_PCA9535_02;
    ret = MI125_com_read(core->com, MI125_COM_PORTS2, caddr, (unsigned char*) &regs.Register_02.raw);
    if(FAILURE(ret)) return ret;

    // Configure which channel are active (the others are in shutdown)
    regs.Register_02.Bit.adc_shdn_o = (actchannel & 0xF);
    ret = MI125_com_write(core->com, MI125_COM_PORTS2, caddr, &regs.Register_02.raw);

    return ret;
}


 /****************************************************************************
 *   NAME : MI125_pca9535_settrgout
 ************************************************************************//**
 *
 *   Configure if the trigger out signal is connected or not from fpga to outside.
 *   The module must be opened with MI125_pca9535_open before using this function.
 *   By default, the trigger out signal is disconnected from outside. 
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [in] mode
 *      Specify if the trigger out signal is connected (MI125_PCA9535_TRIGOUTON)
 *      or not (MI125_PCA9535_TRIGOUTOFF).
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_settrgout(MI125_pca9535_core * core, MI125_pca9535_trigout mode)
{
    Result ret;
    MI125_pca9535_addr caddr;
    MI125_pca9535_Register_Map regs;

    // Read back existing related registers content
    caddr = MI125_PCA9535_03;
    ret = MI125_com_read(core->com, MI125_COM_PORTS2, caddr, (unsigned char*) &regs.Register_03.raw);
    if(FAILURE(ret)) return ret;

    // Set the trigger out mode
    regs.Register_03.Bit.trig_out_en_o =  (mode & 0x1);
    ret = MI125_com_write(core->com, MI125_COM_PORTS2, caddr, &regs.Register_03.raw);

    return ret;
}


 /****************************************************************************
 *   NAME : MI125_pca9535_setclksource
 ************************************************************************//**
 *
 *   Configure the ADC clock source (including clk source for the clock buffer switch)
 *   The module must be opened with MI125_pca9535_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [in] clksrc
 *      Specify which clock source the ADCs will used
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_setclksource(MI125_pca9535_core * core, MI125_pca9535_clksrc clksrc)
{
    Result ret;
    MI125_pca9535_addr caddr;
    MI125_pca9535_Register_Map regs;

    // Read back existing related registers content
    caddr = MI125_PCA9535_02;
    ret = MI125_com_read(core->com, MI125_COM_PORTS2, caddr, (unsigned char*) &regs.Register_02.raw);
    if(FAILURE(ret)) return ret;

    // Configure the adc clock source
    regs.Register_02.Bit.adc_clk_src_o =  (clksrc & 0x7);
    ret = MI125_com_write(core->com, MI125_COM_PORTS2, caddr, &regs.Register_02.raw);

    return ret;
}


 /****************************************************************************
 *   NAME : MI125_pca9535_setbridgereset
 ************************************************************************//**
 *
 *   Control the reset of the spi bridge.
 *   The module must be opened with MI125_pca9535_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [in] resetn
 *      Specify the state of the reset signal (active low)
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_setbridgereset(MI125_pca9535_core * core, MI125_pca9535_bridgereset resetn)
{
    Result ret;
    MI125_pca9535_addr caddr;
    MI125_pca9535_Register_Map regs;

    // Read back existing related registers content
    caddr = MI125_PCA9535_02;
    ret = MI125_com_read(core->com, MI125_COM_PORTS2, caddr, (unsigned char*) &regs.Register_02.raw);
    if(FAILURE(ret)) return ret;

    // Set the bridge reset
    regs.Register_02.Bit.spi_bridge_resetn_o =  (resetn & 0x1);
    ret = MI125_com_write(core->com, MI125_COM_PORTS2, caddr, &regs.Register_02.raw);

    return ret;
}


 /****************************************************************************
 *   NAME : MI125_pca9535_setlocaloscsw
 ************************************************************************//**
 *
 *   Configure if the local 125MHz oscillator is powered up or not.
 *   The module must be opened with MI125_pca9535_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [in] sw
 *      Specify the state of the local oscillator power switch signal (active high)
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_setlocaloscsw(MI125_pca9535_core * core, MI125_pca9535_localoscsw sw)
{
    Result ret;
    MI125_pca9535_addr caddr;
    MI125_pca9535_Register_Map regs;

    // Read back existing related registers content
    caddr = MI125_PCA9535_03;
    ret = MI125_com_read(core->com, MI125_COM_PORTS2, caddr, (unsigned char*) &regs.Register_03.raw);
    if(FAILURE(ret)) return ret;

    // Set the local osc switch
    regs.Register_03.Bit.local_osc_sw_o =  (sw & 0x1);
    ret = MI125_com_write(core->com, MI125_COM_PORTS2, caddr, &regs.Register_03.raw);

    return ret;
}


 /****************************************************************************
 *   NAME : MI125_pca9535_setbufclkpower
 ************************************************************************//**
 *
 *   Configure if the clock buffer is powered up or not.
 *   The module must be opened with MI125_pca9535_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [in] sw
 *      Specify the state of the clock buffer power (active high)
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_setbufclkpower(MI125_pca9535_core * core, MI125_pca9535_bufclkpwr sw)
{
    Result ret;
    MI125_pca9535_addr caddr;
    MI125_pca9535_Register_Map regs;

    // Read back existing related registers content
    caddr = MI125_PCA9535_03;
    ret = MI125_com_read(core->com, MI125_COM_PORTS2, caddr, (unsigned char*) &regs.Register_03.raw);
    if(FAILURE(ret)) return ret;

    // Set the clock buffer power switch
    regs.Register_03.Bit.buf_clock_pwrn_o = (sw & 0x1);
    ret = MI125_com_write(core->com, MI125_COM_PORTS2, caddr, &regs.Register_03.raw);

    return ret;
}


 /****************************************************************************
 *   NAME : MI125_pca9535_setclkbufctrl
 ************************************************************************//**
 *
 *   Configure the clock buffer control signals.
 *   The module must be opened with MI125_pca9535_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [in] config
 *      Specify the state of the clock buffer configuration signal
 *
 *   @param [in] load
 *      Specify the state of the clock buffer load signal
 *
 *   @param [in] sout
 *      Specify the output selection for the clock buffer signals
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_setclkbufctrl(MI125_pca9535_core * core, MI125_pca9535_bufclkconf config,
                                   MI125_pca9535_bufclkload load, MI125_pca9535_bufclkoutput sout)
{
    Result ret;
    MI125_pca9535_addr caddr;
    MI125_pca9535_Register_Map regs;

    // Read back existing related registers content
    caddr = MI125_PCA9535_03;
    ret = MI125_com_read(core->com, MI125_COM_PORTS2, caddr, (unsigned char*) &regs.Register_03.raw);
    if(FAILURE(ret)) return ret;

    // Set the clock buffer switch control signals
    regs.Register_03.Bit.adc_clk_sout_o = (sout & 0x3);
    regs.Register_03.Bit.adc_clk_load_o = (load & 0x1);
    regs.Register_03.Bit.adc_clk_config_o = (config & 0x1);
    ret = MI125_com_write(core->com, MI125_COM_PORTS2, caddr, &regs.Register_03.raw);

    return ret;
}


 /****************************************************************************
 *   NAME : MI125_pca9535_writeportsraw
 ************************************************************************//**
 *
 *   Writes both output data ports in raw form.
 *
 * @warning  
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - The module must be reset with MI125_pca9535_reset() before using this function.
 * - Using this function for diagnostic purpose will destroy all other port settings
 *   programmed with other functions of this module. This function is normally used
 *   for production purpose.   
 *   
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [in] port0
 *      Port0 output raw digital value
 *
 *   @param [in] port1
 *      Port1 output raw digital value
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_writeportsraw(MI125_pca9535_core * core, unsigned char port0, unsigned char port1)
{
    Result ret;
    MI125_pca9535_addr caddr;
    MI125_pca9535_Register_Map regs;

    // Set port0 output raw
    caddr = MI125_PCA9535_02;
    regs.Register_02.raw = port0;
    ret = MI125_com_write(core->com, MI125_COM_PORTS2, caddr, &regs.Register_02.raw);
    if(FAILURE(ret)) return ret;

    // Set port1 output raw
    caddr = MI125_PCA9535_03;
    regs.Register_03.raw = port1;
    ret = MI125_com_write(core->com, MI125_COM_PORTS2, caddr, &regs.Register_03.raw);

    return ret;
}


 /****************************************************************************
 *   NAME : MI125_pca9535_readportsraw
 ************************************************************************//**
 *
 *   Read both output data ports in raw form.
 *
 * @warning  
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - The module must be reset with MI125_pca9535_reset() before using this function.
 * - Using this function for diagnostic purpose will destroy all other port settings
 *   programmed with other functions of this module. This function is normally used
 *   for production purpose.
 * - This function does not read the ports pins, but the actual output ports registers
 *   content written with MI125_pca9535_writeportsraw().     
 *   
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [out] port0
 *      Port0 output raw digital value
 *
 *   @param [out] port1
 *      Port1 output raw digital value
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_readportsraw(MI125_pca9535_core * core, unsigned char *port0, unsigned char *port1)
{
    Result ret;
    MI125_pca9535_addr caddr;
    MI125_pca9535_Register_Map regs;

    // Reads port0 output raw
    caddr = MI125_PCA9535_02;
    ret = MI125_com_read(core->com, MI125_COM_PORTS2, caddr, &regs.Register_02.raw);
    if(FAILURE(ret)) return ret;
    *port0 = regs.Register_02.raw;

    // Reads port1 output raw
    caddr = MI125_PCA9535_03;
    ret = MI125_com_read(core->com, MI125_COM_PORTS2, caddr, &regs.Register_03.raw);
    if(FAILURE(ret)) return ret;
    *port1 = regs.Register_03.raw;

    return ret;
}


 /****************************************************************************
 *   NAME : MI125_pca9535_readportspinraw
 ************************************************************************//**
 *
 *   Reads both data ports input pins in raw form.
 *
 * @warning  
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - The module must be reset with MI125_pca9535_reset() before using this function.
 * - This function is normally used for production purpose.   
 *   
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [out] port0
 *      Port0 pins input raw digital value
 *
 *   @param [out] port1
 *      Port1 pins input raw digital value
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_readportspinraw(MI125_pca9535_core * core, unsigned char *port0, unsigned char *port1)
{
    Result ret;
    MI125_pca9535_addr caddr;
    MI125_pca9535_Register_Map regs;

    // Read port0 input pins raw
    caddr = MI125_PCA9535_00;
    ret = MI125_com_read(core->com, MI125_COM_PORTS2, caddr, &regs.Register_00.raw);
    if(FAILURE(ret)) return ret;
    *port0 = regs.Register_00.raw;

    // Read port1 input pins raw
    caddr = MI125_PCA9535_01;
    ret = MI125_com_read(core->com, MI125_COM_PORTS2, caddr, &regs.Register_01.raw);
    if(FAILURE(ret)) return ret;
    *port1 = regs.Register_01.raw;

    return ret;
}


