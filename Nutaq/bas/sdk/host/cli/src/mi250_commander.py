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
# @file      mi250_commander.py
# 
# @brief     Commander for MI250
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

class mi250_commander(bas_commander):
    def __init__(self):
        super(mi250_commander, self).__init__()
        
    def mi250_select(self, num):
        ret = eapi.MI250_Select_send(self.state, num)
        if ret < 0:
            raise bas_exception(ret)
        
    def mi250_init(self):
        ret = eapi.MI250_Init_send(self.state)
        if ret<0:
            raise bas_exception(ret)

    def mi250_powerup(self):
        ret = eapi.MI250_Powerup_send(self.state)
        if ret<0:
            raise bas_exception(ret)

    def mi250_presence(self):
        ret = eapi.MI250_Presence_send(self.state)
        if ret<0:
            raise bas_exception(ret)

    def mi250_pllconfig(self, reffreq, adcfreq, clksrc):
        ret = eapi.MI250_PLLConfig_send(self.state, reffreq, adcfreq, clksrc)
        if ret<0:
            raise bas_exception(ret)
        
    def mi250_set_config(self, freq, device, dataformat, mode, octa, octb):
        ret = eapi.MI250_SetConfig_send(self.state, freq, device, dataformat, mode, octa, octb)
        if ret<0:
            raise bas_exception(ret)
        
    def mi250_setgain(self, channel, device, gain):
        ret = eapi.MI250_SetGain_send(self.state, channel, device, gain)
        if ret<0:
            raise bas_exception(ret)
        
    def mi250_setfinegain(self, channel, device, gain):
        ret = eapi.MI250_SetFineGain_send(self.state, channel, device, gain)
        if ret<0:
            raise bas_exception(ret)
        
    def mi250_setpedestal(self, channel, device, value):
        ret = eapi.MI250_SetPedestal_send(self.state, channel, device, value)
        if ret<0:
            raise bas_exception(ret)

    def mi250_adcreset(self):
        ret = eapi.MI250_ADCreset_send(self.state)
        if ret<0:
            raise bas_exception(ret)
        
    def mi250_adcarm(self, armedstatus):
        ret = eapi.MI250_ADCarm_send(self.state, armedstatus)
        if ret<0:
            raise bas_exception(ret)

    def mi250_set_data_format(self, device, dataformat):
        ret = eapi.MI250_SetDataFormat_send(self.state, device, dataformat)
        if ret<0:
            raise bas_exception(ret)
        
    def mi250_set_sleep_mode(self, device, sleepmode):
        ret = eapi.MI250_SetSleepMode_send(self.state, device, sleepmode)
        if ret<0:
            raise bas_exception(ret)  
        
    def mi250_set_oct(self, device, octa, octb):
        ret = eapi.MI250_SetOffsetCorrectionTime_send(self.state, device, octa, octb)
        if ret<0:
            raise bas_exception(ret)    


