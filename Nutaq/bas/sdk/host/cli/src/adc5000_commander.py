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
# @file      adc5000_commander.py
# 
# @brief     Commander for ADC5000
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

class adc5000_commander(bas_commander):
    def __init__(self):
        super(adc5000_commander, self).__init__()
        self.adc5000_mode = eapi.EV10AQ190_4CHANNEL_A_B_C_D
        self.adc5000_sync = eapi.ADC5000_SYNC_PLL
        self.adc5000_standby = eapi.EV10AQ190_FULL_ACTIVE
        self.adc5000_coding = eapi.EV10AQ190_BINARY_CODING
        self.adc5000_bandwidth = eapi.EV10AQ190_NOMINAL_BANDWIDTH
        self.adc5000_sensor = 0
        self.adc5000_reffreq = 0
        self.adc5000_adcfreq = 0
        self.adc5000_clockoutfreq = 0
        self.adc5000_fmcfreq = 0
        self.adc5000_syncoutfreq = 0
        self.adc5000_clocksource = eapi.ADC5000_CLOCK_EXTERNAL
        self.adc5000_testmode = eapi.EV10AQ190_DISABLE
        self.adc5000_standby = eapi.EV10AQ190_FULL_ACTIVE
        self.adc5000_channel = eapi.EV10AQ190_CHANNELA

    def adc5000_presence(self):
        ret = eapi.ADC5000_Presence_send(self.state)
        if ret<0:
            raise bas_exception(ret)
        else:
            print "Detected ADC5000 hardware and FPGA core"
    def adc5000_powerup(self):
        ret = eapi.ADC5000_Powerup_send(self.state)
        if ret<0:
            raise bas_exception(ret)

    def adc5000_reset(self):
        ret = eapi.ADC5000_Reset_send(self.state)
        if ret<0:
            raise bas_exception(ret)

    def adc5000_setmode(self,mode):
        ret = eapi.ADC5000_SetMode_send(self.state,mode)
        if ret<0:
            raise bas_exception(ret)

    def adc5000_setsync(self, sync):
        ret = eapi.ADC5000_SetSync_send(self.state, sync)
        if ret<0:
            raise bas_exception(ret)

    def adc5000_configurepll(self, reffreq, adcfreq, clockoutfreq, fmcfreq, syncoutfreq, clocksource):
        ret = eapi.ADC5000_PLLConfigure_send(self.state, reffreq, adcfreq, clockoutfreq, fmcfreq, syncoutfreq, clocksource)
        if ret<0:
            raise bas_exception(ret)

    def adc5000_settestmode(self, testmode):
        ret = eapi.ADC5000_SetTestMode_send(self.state, testmode)
        if ret<0:
            raise bas_exception(ret)

    def adc5000_calibrateiodelay(self):
        ret = eapi.ADC5000_CalibrateIoDelay_send(self.state)
        if ret<0:
            raise bas_exception(ret)

    def adc5000_setstandby(self, standby):
        ret = eapi.ADC5000_SetStandby_send(self.state, standby);
        if ret<0:
            raise bas_exception(ret)

    def adc5000_setcoding(self, coding):
        ret = eapi.ADC5000_SetCoding_send(self.state, coding);
        if ret<0:
            raise bas_exception(ret)

    def adc5000_setbandwidth(self, bandwidth):
        ret = eapi.ADC5000_SetBandwidth_send(self.state, bandwidth);
        if ret<0:
            raise bas_exception(ret)

    def adc5000_setadjtriggerdelay(self, delay):
        ret = eapi.ADC5000_SetAdjTriggerDelay_send(self.state, delay);
        if ret<0:
            raise bas_exception(ret)

    def adc5000_setoffset(self, channel,offset):
        ret = eapi.ADC5000_SetOffset_send(self.state, channel, offset);
        if ret<0:
            raise bas_exception(ret)

    def adc5000_setgain(self, channel,gain):
        ret = eapi.ADC5000_SetGain_send(self.state, channel, gain);
        if ret<0:
            raise bas_exception(ret)

    def adc5000_setphase(self, channel,phase):
        ret = eapi.ADC5000_SetPhase_send(self.state, channel, phase);
        if ret<0:
            raise bas_exception(ret)
