#****************************************************************************  
#
#    ****                              *                                    
#   ******                            ***                                   
#   *******                           ****                                  
#   ********    ****  ****     **** *********    ******* ****    ***********
#   *********   ****  ****     **** *********  **************  *************
#   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
#   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
#  ****    *********  ****     ****   ****   ****       ****  ****      ****
#  ****     ********  ****    *****  ****    *****     *****  ****      ****
#  ****      ******   ***** ******   *****    ****** *******  ****** *******
#  ****        ****   ************    ******   *************   *************
#  ****         ***     ****  ****     ****      *****  ****     *****  ****
#                                                                       ****
#          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
#                                                                        ***      
#
#************************************************************************//**
#
# @file      adac250_commander.py
# 
# @brief     Commander for ADAC250
#
# Copyright (C) 2014, Nutaq, Canada. All rights reserved.
#
# This file is part of Nutaq’s BAS Software Suite.
#
# You may at your option receive a license to Nutaq’s BAS Software Suite under 
# either the terms of the GNU General Public License (GPL) or the 
# Nutaq Professional License, as explained in the note below.
#
# Nutaq’s BAS Software Suite may be used under the terms of the GNU General 
# Public License version 3 as published by the Free Software Foundation 
# and appearing in the file LICENSE.GPL included in the packaging of this file.
#
# Nutaq’s BAS Software Suite is provided AS IS WITHOUT WARRANTY OF 
# ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
# PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# NOTE:
#
# Using Nutaq’s BAS Software Suite under the GPL requires that your work based on 
# Nutaq’s BAS Software Suite, if  distributed, must be licensed under the GPL. 
# If you wish to distribute a work based on Nutaq’s BAS Software Suite but desire 
# to license it under your own terms, e.g. a closed source license, you may purchase 
# a Nutaq Professional License.
#
# The Professional License, gives you -- under certain conditions -- the right to
# use any license you wish for your work based on Nutaq’s BAS Software Suite. 
# For the full terms of the Professional License, please contact the Nutaq sales team.
#
#****************************************************************************/
from bas_commander import bas_commander
from bas_exception import *
import eapi

class adac250_commander(bas_commander):
    def __init__(self):
        super(adac250_commander, self).__init__()

    def adac250_presence(self):
        ret = eapi.ADAC250_Presence_send(self.state)
        if ret<0:
            raise bas_exception(ret)
        else:
            print "Detected ADAC250 hardware and FPGA core"

    def adac250_powerup(self):
        ret = eapi.ADAC250_PowerUp_send(self.state)
        if ret<0:
            raise bas_exception(ret)

    #PLL
    def adac250_pll_init(self, refclk, adcclk, dacclk, interpolation, source):
        ret = eapi.ADAC250_PLL_Init_send(self.state, refclk, adcclk, dacclk, interpolation, source)
        if ret<0:
            raise bas_exception(ret)
        
    def adac250_pll_setRefClkTuning(self, value):
        ret = eapi.ADAC250_PLL_setRefClkTuning_send(self.state, value)
        if ret<0:
            raise bas_exception(ret)


    #DAC
    def adac250_dac_init(self, sleepmode, interpolation, mixmode_a, mixmode_b ):
        eapi.ADAC250_DAC_Init_send(self.state, sleepmode, interpolation, mixmode_a, mixmode_b)

    def adac250_dac_setGain(self, channel, gain):
        ret = eapi.ADAC250_DAC_SetGain_send(self.state, channel, gain)
        if ret<0:
            raise bas_exception(ret)
        
    def adac250_dac_setOffset(self, channel, offset):
        ret = eapi.ADAC250_DAC_SetOffset_send(self.state, channel, offset)
        if ret<0:
            raise bas_exception(ret)

    def adac250_dac_sync(self, enable):
        ret = eapi.ADAC250_DAC_Sync_send(self.state,enable)
        if ret<0:
            raise bas_exception(ret)

    def adac250_dac_syncsource(self, source):
        ret = eapi.ADAC250_DAC_SyncSource_send(self.state, source)
        if ret<0:
            raise bas_exception(ret)

    def adac250_dac_calibrate(self):
        ret = eapi.ADAC250_DAC_Calibrate_send(self.state)
        if ret<0:
            raise bas_exception(ret)

    def adac250_adc_init(self):
        ret = eapi.ADAC250_ADC_init_send(self.state)
        if ret<0:
            raise bas_exception(ret)
        
    def adac250_adc_setGain(self, channel, gain):
        ret = eapi.ADAC250_ADC_setGain_send(self.state, channel, gain)
        if ret<0:
            raise bas_exception(ret)
        
    def adac250_adc_setFineGain(self, channel, gain):
        ret = eapi.ADAC250_ADC_setFineGain_send(self.state, channel, gain)
        if ret<0:
            raise bas_exception(ret)
        
    def adac250_adc_setPedestal(self, channel, value):
        ret = eapi.ADAC250_ADC_setPedestal_send(self.state, channel, value)
        if ret<0:
            raise bas_exception(ret)
        
    def adac250_adc_getStatus(self, channel):
        status = 0
        ret = eapi.ADAC250_ADC_getStatus_send(self.state, channel, status)
        if ret<0:
            raise bas_exception(ret)
        return status

    def adac250_adc_calibrate(self):
        ret = eapi.ADAC250_ADC_Calibrate_send(self.state)
        if ret<0:
            raise bas_exception(ret)

    def adac250_adc_settriggerdelay(self, delay):
        ret = eapi.ADAC250_ADC_SetTriggerDelay_send(self.state, delay)
        if ret<0:
            raise bas_exception(ret)

    #MUX
    def adac250_mux_configClockOutput(self, out, inp):
        eapi.ADAC250_MUX_ConfigClockOutput_send(self.state, out, inp)
