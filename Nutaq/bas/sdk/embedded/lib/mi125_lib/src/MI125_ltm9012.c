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
 * @file       MI125_ltm9012.c
 * @brief      ADC related functions
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
#include "MI125_ltm9012.h"


/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/


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
 *   NAME : MI125_ltm9012_open(MI125_ltm9012_core * core, MI125_com_core * com)
 ************************************************************************//**
 *
 *   Open the ltm9012 module
 *   This function must be the first one used before using an ADC module.
 *   When the module is not used anymore, it can be closed with MI125_ltm9012_close.
 *   The MI125_com_wrapper module must be opened before using this module. 
 *
 *   @param [out] core
 *      This structure will hold necessary information for the module to be used
 *
 *   @param [in] com
 *      This structure must hold communication module information
 *
 *   @return   Error code: MI125_LTM9012OK if no problem is found, !MI125_LTM9012OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LTM9012
 *
 ***************************************************************************/ 
Result MI125_ltm9012_open(MI125_ltm9012_core * core, MI125_com_core * com)
{
    core->com = com;
    return MI125_LTM9012OK;
}


 /****************************************************************************
 *   NAME : MI125_ltm9012_reset(MI125_ltm9012_core * core, MI125_COM_DEVICES adcdev)
 ************************************************************************//**
 *
 *   Reset the specified ltm9012 ADC module to default operating condition
 *   Default chip operation is configured with complement 2 binary output format. 
 *   This function must be called at least once to reset this device to default
 *   operating condition.
 *   The module must be opened with MI125_ltm9012_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_ltm9012_open
 *
 *   @param [in] adcdev
 *      Which adc devices to do this function (MI125_COM_ADC0, MI125_COM_ADC1, MI125_COM_ADC2, MI125_COM_ADC3)
 *
 *   @return   Error code: MI125_LTM9012OK if no problem is found, !MI125_LTM9012OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LTM9012
 *
 ***************************************************************************/ 
Result MI125_ltm9012_reset(MI125_ltm9012_core * core, MI125_COM_DEVICES adcdev)
{
    Result ret;
    MI125_ltm9012_Register_Map regs;
    MI125_ltm9012_addr caddr;

    // Init internal structure
    regs.RegisterArray[0]=0;
    regs.RegisterArray[1]=0;
    regs.RegisterArray[2]=0;
    regs.RegisterArray[3]=0;
    regs.RegisterArray[4]=0;

    caddr = MI125_LTM9012_A0;
    regs.Register_a0.Bit.reset = 1;

    // This will reset all registers bits to 0
    ret = MI125_com_write(core->com, adcdev, caddr, &regs.Register_a0.raw);
    if(FAILURE(ret)) return ret;

    regs.Register_a0.Bit.reset = 0;

    // Configure data output format mode by default 2compl
    caddr = MI125_LTM9012_A1;
    regs.Register_a1.Bit.twoscomp = MI125_LTM9012_TWOCOMPLEMENTFORMAT;
    ret = MI125_com_write(core->com, adcdev, caddr, &regs.Register_a1.raw);
    if(FAILURE(ret)) return ret;
    
    // Configure default lvds 1.75ma term on
    caddr = MI125_LTM9012_A2;
    regs.Register_a2.Bit.ilvds = 7;
    //regs.Register_a2.Bit.ilvds = 5;
    regs.Register_a2.Bit.termon = 1;
    ret = MI125_com_write(core->com, adcdev, caddr, &regs.Register_a2.raw);
    
    return ret;
}


 /****************************************************************************
 *   NAME : MI125_ltm9012_close(MI125_ltm9012_core * core)
 ************************************************************************//**
 *
 *   Close the ltm9012 adc module
 *   This function must be the last one used after using this module.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_ltm9012_open
 *
 *   @return   Error code: MI125_LTM9012OK if no problem is found, !MI125_LTM9012OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LTM9012
 *
 ***************************************************************************/ 
Result MI125_ltm9012_close(MI125_ltm9012_core * core)
{
    return MI125_LTM9012OK;
}


 /****************************************************************************
 *   NAME : MI125_ltm9012_setdigitalcalib(MI125_ltm9012_core * core, MI125_ltm9012_dcalib control, unsigned short pattern, MI125_COM_DEVICES adcdev)
 ************************************************************************//**
 *
 *   Configure the digital output pattern used for calibration or test purpose
 *   and enable or not this mode of operation.
 *   The module must be opened with MI125_ltm9012_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_ltm9012_open
 *
 *   @param [in] control
 *      This configures if the digital pattern is active (MI125_LTM9012_DCALIBON)
 *      or not (MI125_LTM9012_DCALIBOFF) on adc outputs
 *
 *   @param [in] pattern
 *      adc 14 bits pattern used for the outputs (only 14 lsb bits are used, 2 msb bits are force to 0)
 *
 *   @param [in] adcdev
 *      Which adc devices to do this function (MI125_COM_ADC0, MI125_COM_ADC1, MI125_COM_ADC2, MI125_COM_ADC3)
 *
 *   @return   Error code: MI125_LTM9012OK if no problem is found, !MI125_LTM9012OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LTM9012
 *
 ***************************************************************************/ 
Result MI125_ltm9012_setdigitalcalib(MI125_ltm9012_core * core, MI125_ltm9012_dcalib control, unsigned short pattern, MI125_COM_DEVICES adcdev)
{
    Result ret;
    MI125_ltm9012_Register_Map regs;
    MI125_ltm9012_addr caddr;

    // Configure the pattern 8 LSB.
    regs.Register_a4.Bit.tp_lsb = (pattern & 0xFF);

    // Configure the pattern 6 MSB
    regs.Register_a3.Bit.tp_msb = ((pattern>>8) & 0x3F);
    
    // Control to enable or not the pattern
    regs.Register_a3.Bit.outtest = control;

    // Write the pattern and control to the device
    caddr = MI125_LTM9012_A4;
    ret = MI125_com_write(core->com, adcdev, caddr, &regs.Register_a4.raw);
    if(FAILURE(ret)) return ret;

    caddr = MI125_LTM9012_A3;
    ret = MI125_com_write(core->com, adcdev, caddr, &regs.Register_a3.raw);

    return ret;
}


 /****************************************************************************
 *   NAME : MI125_ltm9012_setchannelmode(MI125_ltm9012_core * core, MI125_ltm9012_channel mode, MI125_COM_DEVICES adcdev)
 ************************************************************************//**
 *
 *   Configure which adc channel is active or not on the device. Channels
 *   are configure by group of 4 (0 channel means device in sleep mode) 
 *   The module must be opened with MI125_ltm9012_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_ltm9012_open
 *
 *   @param [in] mode
 *      This configures which channels are active, all channels (MI125_LTM9012_4CHANNEL)
 *      or no (MI125_LTM9012_0CHANNEL) channel
 *
 *   @param [in] adcdev
 *      Which adc devices to do this function (MI125_COM_ADC0, MI125_COM_ADC1, MI125_COM_ADC2, MI125_COM_ADC3)
 *
 *   @return   Error code: MI125_LTM9012OK if no problem is found, !MI125_LTM9012OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LTM9012
 *
 ***************************************************************************/ 
Result MI125_ltm9012_setchannelmode(MI125_ltm9012_core * core, MI125_ltm9012_channel mode, MI125_COM_DEVICES adcdev)
{
    Result ret;
    MI125_ltm9012_Register_Map regs;
    MI125_ltm9012_addr caddr;

    // Read back adc existing related registers content
    caddr = MI125_LTM9012_A1;
    ret = MI125_com_read(core->com, adcdev, caddr, (unsigned char*) &regs.Register_a1.raw);
    if(FAILURE(ret)) return ret;
    caddr = MI125_LTM9012_A2;
    ret = MI125_com_read(core->com, adcdev, caddr, (unsigned char*) &regs.Register_a2.raw);
    if(FAILURE(ret)) return ret;

    // According the specified mode
    switch(mode)
    {
    case MI125_LTM9012_0CHANNEL:
        // No channel, device in sleep outputs off
        regs.Register_a1.Bit.sleep = 1;
        regs.Register_a2.Bit.outoff = 1;
        break;
    case MI125_LTM9012_4CHANNEL:
        // All channels, device full active with outputs on
        regs.Register_a1.Bit.sleep = 0;
        regs.Register_a1.Bit.nap_1 = 0;
        regs.Register_a1.Bit.nap_2 = 0;
        regs.Register_a1.Bit.nap_3 = 0;
        regs.Register_a1.Bit.nap_4 = 0;
        regs.Register_a2.Bit.outoff = 0;
        break;
    default:
        return MI125_LTM9012_IVAL;
    }
    
    // Configure the related modified registers in the device (register A2 1st)
    ret = MI125_com_write(core->com, adcdev, caddr, &regs.Register_a2.raw);
    if(FAILURE(ret)) return ret;
    
    caddr = MI125_LTM9012_A1;
    ret = MI125_com_write(core->com, adcdev, caddr, &regs.Register_a1.raw);
    
    return ret;
}


 /****************************************************************************
 *   NAME : MI125_ltm9012_setconfig(MI125_ltm9012_core * core, MI125_ltm9012_randmode rand, MI125_ltm9012_binmode bin, MI125_ltm9012_lvds lvds, MI125_COM_DEVICES adcdev)
 ************************************************************************//**
 *
 *   Configure adc operating options
 *   The module must be opened with MI125_ltm9012_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_ltm9012_open
 *
 *   @param [in] rand
 *      This configures if output randomize mode is used (MI125_LTM9012_RANDOMIZEON)
 *      or not (MI125_LTM9012_RANDOMIZEOFF).
 *      This mode can be used to minimize potential adc output crosstalk
 *      FPGA core must also be configured accordingly 
 *      This format is ignored when MI125_ltm9012_setdigitalcalib is active.
 *
 *   @param [in] bin
 *      This configures adc output data format mode. It could be binary offset (MI125_LTM9012_OFFSETBINARYFORMAT)
 *      or 2 compl (default) (MI125_LTM9012_TWOCOMPLEMENTFORMAT).
 *      This format is ignored when MI125_ltm9012_setdigitalcalib is active.
 *
 *   @param [in] lvds
 *      This configures adc output lvds configuration. This configuration must be choosen to
 *      according to FPGA inputs. An additionnal ADC internal termination can be added and the
 *      drive current can be configured.
 *
 *   @param [in] adcdev
 *      Which adc devices to do this function (MI125_COM_ADC0, MI125_COM_ADC1, MI125_COM_ADC2, MI125_COM_ADC3)
 *
 *   @return   Error code: MI125_LTM9012OK if no problem is found, !MI125_LTM9012OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LTM9012
 *
 ***************************************************************************/ 
Result MI125_ltm9012_setconfig(MI125_ltm9012_core * core, MI125_ltm9012_randmode rand, MI125_ltm9012_binmode bin, MI125_ltm9012_lvds lvds, MI125_COM_DEVICES adcdev)
{
    Result ret;
    MI125_ltm9012_Register_Map regs;
    MI125_ltm9012_addr caddr;

    // Read back related device registers
    caddr = MI125_LTM9012_A1;
    ret = MI125_com_read(core->com, adcdev, caddr, (unsigned char*) &regs.Register_a1.raw);
    if(FAILURE(ret)) return ret;

    caddr = MI125_LTM9012_A2;
    ret = MI125_com_read(core->com, adcdev, caddr, (unsigned char*) &regs.Register_a2.raw);
    if(FAILURE(ret)) return ret;

    // Configure digital output randomize mode
    regs.Register_a1.Bit.rand = (rand & 1);

    // Configure data output format mode
    regs.Register_a1.Bit.twoscomp = (bin & 1);

    // Configure lvds output mode
    // When termination is active, only a small subset of low drive currents are possible
    switch(lvds)
    {
        // No internal termination, 1.75mA drive current
        case MI125_LTM9012_TERMOFF1750UA:
            regs.Register_a2.Bit.ilvds = 7;
            regs.Register_a2.Bit.termon = 0;
            break;
        // No internal termination, 2.1mA drive current
        case MI125_LTM9012_TERMOFF2100UA:
            regs.Register_a2.Bit.ilvds = 6;
            regs.Register_a2.Bit.termon = 0;
            break;
        // No internal termination, 2.5mA drive current
        case MI125_LTM9012_TERMOFF2500UA:
            regs.Register_a2.Bit.ilvds = 5;
            regs.Register_a2.Bit.termon = 0;
            break;
        // No internal termination, 3.0mA drive current
        case MI125_LTM9012_TERMOFF3000UA:
            regs.Register_a2.Bit.ilvds = 4;
            regs.Register_a2.Bit.termon = 0;
            break;
        // No internal termination, 3.5mA drive current
        case MI125_LTM9012_TERMOFF3500UA:
            regs.Register_a2.Bit.ilvds = 0;
            regs.Register_a2.Bit.termon = 0;
            break;
        // No internal termination, 4.0mA drive current
        case MI125_LTM9012_TERMOFF4000UA:
            regs.Register_a2.Bit.ilvds = 1;
            regs.Register_a2.Bit.termon = 0;
            break;
        // No internal termination, 4.5mA drive current
        case MI125_LTM9012_TERMOFF4500UA:
            regs.Register_a2.Bit.ilvds = 2;
            regs.Register_a2.Bit.termon = 0;
            break;
        // With internal termination, 1.75mA drive current
        case MI125_LTM9012_TERMON1750UA:
            regs.Register_a2.Bit.ilvds = 7;
            regs.Register_a2.Bit.termon = 1;
            break;
        // With internal termination, 2.1mA drive current
        case MI125_LTM9012_TERMON2100UA:
            regs.Register_a2.Bit.ilvds = 6;
            regs.Register_a2.Bit.termon = 1;
            break;
        // With internal termination, 2.5mA drive current
        case MI125_LTM9012_TERMON2500UA:
            regs.Register_a2.Bit.ilvds = 5;
            regs.Register_a2.Bit.termon = 1;
            break;
        default:
            // Invalid provided lvds config value
            return MI125_LTM9012_ILVDSVAL;
            break;
    }

    // Support only 16 bits 2 lanes serialization output mode
    regs.Register_a2.Bit.outmode = 0;

    // Setup the registers according to the config
    caddr = MI125_LTM9012_A1;
    ret = MI125_com_write(core->com, adcdev, caddr, &regs.Register_a1.raw);
    if(FAILURE(ret)) return ret;

    caddr = MI125_LTM9012_A2;
    ret = MI125_com_write(core->com, adcdev, caddr, &regs.Register_a2.raw);

    return ret;
}


 /****************************************************************************
 *   NAME : MI125_ltm9012_checkadc(MI125_ltm9012_core * core, unsigned char *written,
 *                                 unsigned char *verify, unsigned char *detected,
 *                                 MI125_COM_DEVICES adcdev)
 ************************************************************************//**
 *
 *   Diagnostic function to detect if adc communication interface is working
 *   correctly. 
 *   Warning:  this function must not be used when adc data calibration is
 *   currently running (as set by MI125_ltm9012_setdigitalcalib), because they
 *   are not compatible and will interfere each other. Usually, this function
 *   would only be used for manufacturing purpose.  
 *   The module must be opened with MI125_ltm9012_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_ltm9012_open
 *
 *   @param [out] written
 *      Data pattern that was written for the test
 *
 *   @param [out] verify
 *      Data pattern that was readback (should match written value if everything ok)
 *
 *   @param [out] detected
 *      return a value =0 if the tested adc was not detected correctly, !=0 if it was OK
 *
 *   @param [in] adcdev
 *      Which adc devices to do this function (MI125_COM_ADC0, MI125_COM_ADC1, MI125_COM_ADC2, MI125_COM_ADC3)
 *
 *   @return   Error code: MI125_LTM9012OK if no problem is found, !MI125_LTM9012OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LTM9012
 *
 ***************************************************************************/ 
Result MI125_ltm9012_checkadc(MI125_ltm9012_core * core, unsigned char *written, unsigned char *verify, unsigned char *detected, MI125_COM_DEVICES adcdev)
{
    Result ret = MI125_LTM9012OK;
    MI125_ltm9012_Register_Map regs;
    MI125_ltm9012_addr caddr;

    // By default means a problem
    *detected = 0;
        
    // This register is used as scrachpad register. When MI125_ltm9012_setdigitalcalib is not
    // configured active, this register would not interfere with adc operation
    caddr = MI125_LTM9012_A4;

    // Default read back value different than written pattern
    *verify = 0xFF;     

    // Write start written pattern (value of 0)
    *written = 0;
    regs.Register_a4.raw = *written;
    ret = MI125_com_write(core->com, adcdev, caddr, &regs.Register_a4.raw);
    if(FAILURE(ret)) return ret;
    
    // Read back the 1st pattern
    ret = MI125_com_read(core->com, adcdev, caddr, verify);
    if(FAILURE(ret)) return ret;

    // Check if we read back the expected pattern
    if (*verify == *written)
    {
        // At this point verify value is 0
        
        // Write 2nd pattern
        *written = 0x5C;
        regs.Register_a4.raw = *written;
        ret = MI125_com_write(core->com, adcdev, caddr, &regs.Register_a4.raw);
        if(FAILURE(ret)) return ret;
        
        // Read back second pattern
        ret = MI125_com_read(core->com, adcdev, caddr, verify);
        if(FAILURE(ret)) return ret;
    
        // Check if we read back the expected pattern
        if (*verify == *written)
            // Ok test completed correctly
            *detected = 1;
    }
        
    return ret;
}

