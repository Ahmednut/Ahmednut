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
 * @file       adc5000.c
 * @brief      Adc5000 module function definitions
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup ADC5000
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
#include "adc5000.h"
#include "com_wrapper.h"
#include <string.h>
#include "bas_debug.h"

#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_fmc_service.h"
#include "carrier_fmc_service_def.h"

#include "FMCEEprom_eeprom_id.h"

Result adc5000_open(adc5000 * board, adc5000_core * core, Carrier_handle_t hCarrier)
{
    Result ret;
    unsigned int I2cZone;

    board->core = core;
    
    board->hCarrier = hCarrier;
    // Only support first FMC connector at the moment
    board->FmcConnector = (Carrier_eFmcConnectorNum_t)0;
    
    // Get the I2C zone based on the FMC position
    ret = Carrier_FmcGetI2cZone(board->hCarrier, board->FmcConnector, &I2cZone);
    if(FAILURE(ret)) return ret;

    board->calibration_done = 0;
    ret = com_open(&board->com, hCarrier, I2cZone);
    if(FAILURE(ret))
	{
		return ret;
	}
    ret = ad9517_open(&board->pll, &board->com);
    if(FAILURE(ret))
	{
		return ret;
	}
    ret = ev10aq190_open(&board->adc, &board->com);
    if(FAILURE(ret))
	{
		return ret;
	}
    ret = adt7411_open(&board->monitoring, &board->com);
    if(FAILURE(ret))
	{
		return ret;
	}

  return ADC5000OK;
}

Result adc5000_reset(adc5000 * board)
{
    Result ret = ADC5000OK;
    unsigned char data8;
    unsigned short data16;
    adc5000_cpld_Register_Map regs;
    ad9517_param param;

    board->core->control.core_reset_pulse = 1;

    memset(&regs, 0, sizeof(regs));

    com_init(&board->com, 0, 2);

    regs.Register_00.Bit.clk_src = ADC5000_CLOCK_INTERNAL_REF;
    regs.Register_00.Bit.sync_src = ADC5000_SYNC_CARRIER;
    regs.Register_00.Bit.adc_reset = 1;
    regs.Register_00.Bit.pll_reset = 1;

    ret = com_write(&board->com, COM_CPLD, 0, &regs.Register_00.raw);
    if(FAILURE(ret)) return ret;

    Carrier_usleep(board->hCarrier, 1000);

    regs.Register_00.Bit.pll_reset = 0;

    ret = com_write(&board->com, COM_CPLD, 0, &regs.Register_00.raw);
    if(FAILURE(ret)) return ret;

    Carrier_usleep(board->hCarrier, 1000);

    ret = ad9517_reset(&board->pll);
    if(FAILURE(ret)) return ret;

    ret = com_read(&board->com, COM_PLL, 3, &data8);
    if(FAILURE(ret)) return ret;

    if (data8 != 0x51)
		return ADC5000_ERROR_COMM_PLL;

    //FIXME The correct configuration should be written using the ad9517_reset command
    ret = adc5000_pll_calculate(&param, ((unsigned)100e6), ((unsigned)2500e6), ((unsigned)2500e6), ((unsigned)250e6), ((unsigned)250e6), ADC5000_CLOCK_INTERNAL_REF);
    if(FAILURE(ret)) return ret;

    ret = adc5000_pll_setparam(board, &param);
    if(FAILURE(ret)) return ret;

    regs.Register_00.Bit.adc_reset = 0;

    ret = com_write(&board->com, COM_CPLD, 0, &regs.Register_00.raw);
    if(FAILURE(ret)) return ret;

    Carrier_usleep(board->hCarrier, 1000);

    ret = com_read(&board->com, COM_ADC, 0, &data16);
    if(FAILURE(ret)) return ret;

    if ((data16 != 0x0418) && (data16 != 0x041C))
    	return ADC5000_ERROR_COMM_ADC;

    ret = ev10aq190_reset(&board->adc);
    if(FAILURE(ret)) return ret;
    
    ret = adt7411_reset(&board->monitoring);
    if(FAILURE(ret)) return ret;

    return ADC5000OK;
}

Result adc5000_close(adc5000 * board)
{
    Result ret = ADC5000OK;

    ret = ev10aq190_close(&board->adc);
    if(FAILURE(ret)) return ret;

    ret = ad9517_close(&board->pll);
    if(FAILURE(ret)) return ret;

    ret = com_close(&board->com);
    if(FAILURE(ret)) return ret;
    
    ret = adt7411_close(&board->monitoring);
    if(FAILURE(ret)) return ret;

    return ADC5000OK;
}

Result adc5000_calibrateiodelay(adc5000 * board)
{
    Result ret = ADC5000OK;
    unsigned int i = 0;
    unsigned int validDelayA[32] = {0};
    unsigned int validDelayB[32] = {0};
    unsigned int validDelayC[32] = {0};
    unsigned int validDelayD[32] = {0};
    unsigned char delayGroupCnt = 0;
    unsigned int minTap = 0;
    unsigned int maxTap = 31;
    unsigned char delayGroupMiddleA = 0;
    unsigned char delayGroupMiddleB = 0;
    unsigned char delayGroupMiddleC = 0;
    unsigned char delayGroupMiddleD = 0;
    unsigned char delayGroupMax = 0;
    unsigned char findNextError = 0;
    adc5000_cpld_Register_Map ori_regs;
    adc5000_cpld_Register_Map tmp_regs;
    ev10aq190_Register_Map ori_regs_ev10aq190;
    
    board->core->control.a_aligned = 0;
    board->core->control.b_aligned = 0;    
    board->core->control.c_aligned = 0;
    board->core->control.d_aligned = 0;   

    // backup current sync source and set it to carrier during the calibration
    ret = com_read(&board->com, COM_CPLD, 0, &ori_regs.Register_00.raw);
    if(FAILURE(ret)) return ret;
    tmp_regs.Register_00.raw = ori_regs.Register_00.raw;
    tmp_regs.Register_00.Bit.sync_src = ADC5000_SYNC_CARRIER;
    ret = com_write(&board->com, COM_CPLD, 0, &tmp_regs.Register_00.raw);
    if(FAILURE(ret)) return ret;

    // backup adc5000 standby mode
    ret = com_read((&board->adc)->com, COM_ADC, 0x1, (unsigned*) &ori_regs_ev10aq190.Register_01.raw);
      
    // enable all adc5000 channels
    ret = adc5000_setstandby(board, EV10AQ190_FULL_ACTIVE);
    if(FAILURE(ret)) return ret;

    // Set adc5000 in test mode (ramp)
    ret = adc5000_settestmode(board, EV10AQ190_RAMP);
    if(FAILURE(ret)) return ret;

    //Align the FPGA SERDES
    for(i=0; i<32; i++)
    {
        board->core->calibA.idelay = i;
        board->core->calibB.idelay = i;
        board->core->calibC.idelay = i;
        board->core->calibD.idelay = i;
        board->core->calibA.idelay_we = 0x3FF;
        board->core->calibB.idelay_we = 0x3FF;
        board->core->calibC.idelay_we = 0x3FF;
        board->core->calibD.idelay_we = 0x3FF;
        Carrier_usleep(board->hCarrier, 100);
        validDelayA[i] = board->core->calibErrorChA;
        validDelayB[i] = board->core->calibErrorChB;
        validDelayC[i] = board->core->calibErrorChC;
        validDelayD[i] = board->core->calibErrorChD;
    }

    // Find the longest valid delay group and its middle value for channel A
    delayGroupCnt = 0;
    delayGroupMax = 0;
    for(i=minTap; i<=maxTap; i++)
    {
      if(validDelayA[i]==0)
      {
        delayGroupCnt++;
        if(delayGroupCnt>delayGroupMax)
        {
          delayGroupMax = delayGroupCnt;
          delayGroupMiddleA = i-delayGroupCnt/2;
        }
      }
      else
      {
        delayGroupCnt = 0;
      }
    }
    
    findNextError = 0;
    for(i=delayGroupMiddleA; i<=maxTap; i++)
    {
      if(findNextError == 0)
      {
        if(validDelayA[i]==1)
        {
          findNextError = 1;
        }
      }
      else
      {
        if(validDelayA[i]==0)
        {
          maxTap = i-1;
          break;
        }
      }
    }
    
    findNextError = 0;
    for(i=delayGroupMiddleA; i<32; i--)
    {
      if(findNextError == 0)
      {
        if(validDelayA[i]==1)
        {
          findNextError = 1;
        }
      }
      else
      {
        if(validDelayA[i]==0)
        {
          minTap = i+1;
          break;
        }
      }
    }

    // Find the longest valid delay group and its middle value for other channels
    // Channel B
    delayGroupCnt = 0;
    delayGroupMax = 0;
    for(i=minTap; i<=maxTap; i++)
    {
      if(validDelayB[i]==0)
      {
        delayGroupCnt++;
        if(delayGroupCnt>delayGroupMax)
        {
          delayGroupMax = delayGroupCnt;
          delayGroupMiddleB = i-delayGroupCnt/2;
        }
      }
      else
      {
        delayGroupCnt = 0;
      }
    }
    // Channel C
    delayGroupCnt = 0;
    delayGroupMax = 0;
    for(i=minTap; i<=maxTap; i++)
    {
      if(validDelayC[i]==0)
      {
        delayGroupCnt++;
        if(delayGroupCnt>delayGroupMax)
        {
          delayGroupMax = delayGroupCnt;
          delayGroupMiddleC = i-delayGroupCnt/2;
        }
      }
      else
      {
        delayGroupCnt = 0;
      }
    }
    // Channel D
    delayGroupCnt = 0;
    delayGroupMax = 0;
    for(i=minTap; i<=maxTap; i++)
    {
      if(validDelayD[i]==0)
      {
        delayGroupCnt++;
        if(delayGroupCnt>delayGroupMax)
        {
          delayGroupMax = delayGroupCnt;
          delayGroupMiddleD = i-delayGroupCnt/2;
        }
      }
      else
      {
        delayGroupCnt = 0;
      }
    }

    board->core->calibA.idelay = delayGroupMiddleA;
    board->core->calibB.idelay = delayGroupMiddleB;
    board->core->calibC.idelay = delayGroupMiddleC;
    board->core->calibD.idelay = delayGroupMiddleD;
    board->core->calibA.idelay_we = 0x3FF;
    board->core->calibB.idelay_we = 0x3FF;
    board->core->calibC.idelay_we = 0x3FF;
    board->core->calibD.idelay_we = 0x3FF;
    Carrier_usleep(board->hCarrier, 100);
    
    if (board->core->calibErrorChA == 0)
    {
      board->calibration_done |= 1;
    }
    if (board->core->calibErrorChB == 0)
    {
      board->calibration_done |= (1 << 1);
    }
    if (board->core->calibErrorChC == 0)
    {
      board->calibration_done |= (1 << 2);
    }
    if (board->core->calibErrorChD == 0)
    {
      board->calibration_done |= (1 << 3);
    }
    
    //Disable test mode
    ret = adc5000_settestmode(board, EV10AQ190_DISABLE);
    if(FAILURE(ret)) return ret;

    // Clear FIFO to align data
    board->core->control.rst_fifo = 1;
    board->core->control.rst_fifo = 0;
        
    // restore adc5000 standby
    ret = com_write(&board->com, COM_ADC, 1, &ori_regs_ev10aq190.Register_01.raw);
    if(FAILURE(ret)) return ret;
    
    debug("EV10AQ190 Standby: 0x%x\n",ori_regs_ev10aq190.Register_01.Bit.standby);
    debug("Calibration done: 0x%x\n",board->calibration_done);
    // Set FPGA ready signals depending on calibration success and channel standby mode
    if(ori_regs_ev10aq190.Register_01.Bit.standby == 0 || ori_regs_ev10aq190.Register_01.Bit.standby == 2)
    {
      board->core->control.a_aligned = (board->calibration_done >> 0) & 0x1;
      board->core->control.b_aligned = (board->calibration_done >> 1) & 0x1;    
    }
    else
    {
      board->core->control.a_aligned = 0;
      board->core->control.b_aligned = 0;    
    }
    
    if(ori_regs_ev10aq190.Register_01.Bit.standby == 0 || ori_regs_ev10aq190.Register_01.Bit.standby == 1)
    {
      board->core->control.c_aligned = (board->calibration_done >> 2) & 0x1;
      board->core->control.d_aligned = (board->calibration_done >> 3) & 0x1;    
    }
    else
    {
      board->core->control.c_aligned = 0;
      board->core->control.d_aligned = 0;    
    }
    
    // Restore sync source
    ret = com_write(&board->com, COM_CPLD, 0, &ori_regs.Register_00.raw);
    if(FAILURE(ret)) return ret;
    
    if(board->calibration_done != 0xF)
    {
      debug("Align error: 0x%x, ",board->core->calibErrorChA);
      debug("0x%x, ",board->core->calibErrorChB);
      debug("0x%x, ",board->core->calibErrorChC);
      debug("0x%x\n",board->core->calibErrorChD);
      ret = ADC5000ALIGN;
      return ret;
    }

    return ADC5000OK;
}

Result adc5000_getadcstatus(adc5000 * board, unsigned char * adc_A_active, unsigned char * adc_B_active, unsigned char * adc_C_active, unsigned char * adc_D_active)
{
    Result ret = ADC5000OK;

    ret = ev10aq190_getstatus(&board->adc, adc_A_active, adc_B_active, adc_C_active, adc_D_active);
    if(FAILURE(ret)) return ret;

    return ADC5000OK;
}

Result adc5000_setmode(adc5000 * board, ev10aq190_mode mode)
{
    Result ret = ADC5000OK;

    ret = ev10aq190_setmode(&board->adc, mode);
    if(FAILURE(ret)) return ret;

    // Sync
    board->core->control.sync_cmd = 1;

    //Reset the FPGA SERDES clk, iodelay & data_ios
    // Resets are self cleared/writing 1 generates a pulse
    board->core->control.clk_reset = 1;
    board->core->control.io_reset = 1;

    board->core->control.sync_cmd = 0;

    board->core->control.rst_fifo = 1;
    board->core->control.rst_fifo = 0;

    return ADC5000OK;

}

Result adc5000_setstandby(adc5000 * board, ev10aq190_standby standby)
{
    Result ret = ADC5000OK;
    
    debug("EV10AQ190 Standby: 0x%x\n",standby);
    debug("Calibration done: 0x%x\n",board->calibration_done);
    if(standby == 0 || standby == 2)
    {
      board->core->control.a_aligned = (board->calibration_done >> 0) & 0x1;
      board->core->control.b_aligned = (board->calibration_done >> 1) & 0x1;    
    }
    else
    {
      board->core->control.a_aligned = 0;
      board->core->control.b_aligned = 0;    
    }
    
    if(standby == 0 || standby == 1)
    {
      board->core->control.c_aligned = (board->calibration_done >> 2) & 0x1;
      board->core->control.d_aligned = (board->calibration_done >> 3) & 0x1;    
    }
    else
    {
      board->core->control.c_aligned = 0;
      board->core->control.d_aligned = 0;    
    }

    ret = ev10aq190_setstandby(&board->adc, standby);
    if(FAILURE(ret)) return ret;

    // Sync
    board->core->control.sync_cmd = 1;

    //Reset the FPGA SERDES clk, iodelay & data_ios
    // Resets are self cleared/writing 1 generates a pulse
    board->core->control.clk_reset = 1;
    board->core->control.io_reset = 1;

    board->core->control.sync_cmd = 0;

    board->core->control.rst_fifo = 1;
    board->core->control.rst_fifo = 0;

    return ADC5000OK;
}

Result adc5000_setcoding(adc5000 * board, ev10aq190_coding coding)
{
    Result ret = ADC5000OK;

    ret = ev10aq190_setcoding(&board->adc, coding);
    if(FAILURE(ret)) return ret;

    return ADC5000OK;
}

Result adc5000_setbandwidth(adc5000 * board, ev10aq190_bandwidth bandwidth)
{
    Result ret = ADC5000OK;

    ret = ev10aq190_setbandwidth(&board->adc, bandwidth);
    if(FAILURE(ret)) return ret;

    return ADC5000OK;
}

Result adc5000_pll_calculate(ad9517_param * param, unsigned ref_freq, unsigned adc_freq, unsigned clock_output_freq, unsigned fmc_freq, unsigned sync_output_freq, adc5000_clock_source clock_source)
{
    Result ret = ADC5000OK;

    memset(param, 0, sizeof(ad9517_param));
    param->ref_freq = ref_freq;
    param->pfd_freq_max = AD9517_PFD_MAX_FREQ;
    param->pfd_freq_min = AD9517_PFD_MIN_FREQ;
    param->vco_freq_max = ADC5000_VCO_FREQ_MAX;
    param->vco_freq_min = ADC5000_VCO_FREQ_MIN;
    param->out_freq[0] = clock_output_freq;
    param->out_freq[1] = adc_freq;
    param->out_freq[2] = sync_output_freq;
    param->out_freq[3] = fmc_freq;
    switch(clock_source)
    {
    case ADC5000_CLOCK_EXTERNAL:
        param->external_clock = 2;
        param->vco_freq_max = ref_freq;
        param->vco_freq_min = ref_freq;
        break;
    case ADC5000_CLOCK_EXTERNAL_REF:
        param->external_clock = 1;
        break;
    default:
    case ADC5000_CLOCK_INTERNAL_REF:
        param->external_clock = 0;
        break;
    }

    ret = ad9517_calculate(param);
    if(FAILURE(ret)) return ret;

    return ADC5000OK;
}

Result adc5000_pll_setparam(adc5000 * board, ad9517_param * param)
{
    Result ret = ADC5000OK;
    adc5000_cpld_Register_Map regs;


    com_read(&board->com, COM_CPLD, 0x0, &regs.Register_00.raw);
    if(param->external_clock ==1)
    {
        regs.Register_00.Bit.clk_src = ADC5000_CLOCK_EXTERNAL_REF;
    }
    else if(param->external_clock ==2)
    {
        regs.Register_00.Bit.clk_src = ADC5000_CLOCK_EXTERNAL;
    }
    else
    {
        regs.Register_00.Bit.clk_src = ADC5000_CLOCK_INTERNAL_REF;
    }
    com_write(&board->com, COM_CPLD, 0x0, &regs.Register_00.raw);

    ret = ad9517_setparam(&board->pll, param);
    if(FAILURE(ret)) return ret;

    board->core->control.sync_cmd = 1;

    // Reset the FPGA SERDES clk, iodelay & data_ios
    // Resets are self cleared/writing 1 generates a pulse
    board->core->control.clk_reset = 1;
    board->core->control.io_reset = 1;

    board->core->control.sync_cmd = 0;

    board->core->control.rst_fifo = 1;
    board->core->control.rst_fifo = 0;

    return ADC5000OK;
}

Result adc5000_setsync(adc5000 * board, adc5000_sync_source sync_source)
{
    Result ret;
    adc5000_cpld_Register_Map regs;

    ret = com_read(&board->com, COM_CPLD, 0, &regs.Register_00.raw);
    if(FAILURE(ret)) return ret;

    regs.Register_00.Bit.sync_src = sync_source;
    ret = com_write(&board->com, COM_CPLD, 0, &regs.Register_00.raw);
    if(FAILURE(ret)) return ret;

    return ADC5000OK;
}

Result adc5000_pll_lock(adc5000 * board, ad9517_lock_status * status)
{
    Result ret;
    int timeout = 1000;
    int i;
    ad9517_lock_status state;

    i = 10; //check 10 times that the PLL is locked, to see if it is stable
    while(timeout-- > 0)
    {
        if(!i) break;

        ret = ad9517_lock_state(&board->pll, &state);
        if(FAILURE(ret)) return ret;

        if(state == AD9517_PLL_LOCKED)
        {
            i--;
        }else{
            i = 10;
        }
    }

    if(timeout <= 0)
    {
        *status = AD9517_PLL_UNLOCKED;
        debug("PLL Unlocked: %x\n", timeout );
    }
    else
    {
        *status = AD9517_PLL_LOCKED;
        debug("PLL Locked: %x\n", timeout );
    }

    return ADC5000OK;
}

/* Commented out because the value read are not right
Result adc5000_read_temperature(adc5000 * board, adc5000_sensor sensor, float * temp)
{
    Result ret;
    short raw;

    switch(sensor)
    {
    case ADC5000_BOARD_TEMPERATURE:
        ret = adt7411_read_sensor_raw(&board->monitoring, AD7411_SENSOR_INTERNAL_TEMP, &raw);
        if(FAILURE(ret)) return ret;
        break;

    case ADC5000_ADC_TEMPERATURE:
        ret = adt7411_read_sensor_raw(&board->monitoring, AD7411_SENSOR_EXTERNAL_TEMP, &raw);
        if(FAILURE(ret)) return ret;
        break;

    default:
        return ADT7411UNIMPLEMENTED;
    }

    if(temp)
    {
        *temp = ((float)raw) / 4;
    }

    return ADC5000OK;
}
*/

Result adc5000_settestmode(adc5000 * board, ev10aq190_testmode state)
{
  Result ret;

  ret = ev10aq190_settestmode(&board->adc, state);
  if(FAILURE(ret)) return ret;

  if(state != EV10AQ190_DISABLE)
  {
    // Sync
    board->core->control.sync_cmd = 1;

    // Reset the FPGA SERDES clk, iodelay & data_ios
    // Resets are self cleared/writing 1 generates a pulse
    board->core->control.clk_reset = 1;
    board->core->control.io_reset = 1;

    board->core->control.sync_cmd = 0;

    board->core->control.rst_fifo = 1;
    board->core->control.rst_fifo = 0;
  }

  return ADC5000OK;
}

Result adc5000_setadjtriggerdelay(adc5000 * board, char delay)
{

	if(delay > 32)
	{
		return ADC5000_INVALID_TRIGGER_DELAY;
	}
	if(delay < 1)
	{
		return ADC5000_INVALID_TRIGGER_DELAY;
	}

  board->core->trig_control.trigger_delay = delay - 1;

	return ADC5000OK;
}

Result adc5000_setoffset(adc5000 * board, ev10aq190_channel channel, unsigned short offset)
{
  return ev10aq190_offset(&board->adc, channel, offset);
}

Result adc5000_setgain(adc5000 * board, ev10aq190_channel channel, unsigned short gain)
{
  return ev10aq190_gain(&board->adc, channel, gain);
}

Result adc5000_setphase(adc5000 * board, ev10aq190_channel channel, unsigned short phase)
{
  return ev10aq190_phase(&board->adc, channel, phase);
}
