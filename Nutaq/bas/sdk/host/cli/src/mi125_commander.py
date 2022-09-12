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
# @file      mi125_commander.py
# 
# @brief     Commander for MI125
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

class mi125_commander(bas_commander):
    def __init__(self):
        super(mi125_commander, self).__init__()
        #self.mi125_mode = 0
        
    def mi125_powerup(self,nb):
        ret = eapi.MI125_powerup_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)

    def mi125_reset(self,nb):
        ret = eapi.MI125_mi125_reset_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)

    def mi125_digital_adccalibration(self,nb):
        ret = eapi.MI125_mi125_digital_adccalibration_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)

    def mi125_set_config(self, nb, groupch, lvds, randmode, binmode):
        ret = eapi.MI125_mi125_set_config_send(self.state, nb, groupch, lvds, randmode, binmode)
        if ret<0:
            raise bas_exception(ret)       

    def mi125_set_clksrc(self, nb, clksrc):
        ret = eapi.MI125_mi125_set_clksrc_send(self.state, nb, clksrc)
        if ret<0:
            raise bas_exception(ret)

    def mi125_checkcore(self, nb):
        ret,detected = eapi.MI125_mi125_checkcore_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return detected

    def mi125_set_testmode(self, nb, testmode, pattern):
        ret = eapi.MI125_mi125_set_testmode_send(self.state, nb, testmode, pattern)
        if (ret<0):
            raise bas_exception(ret)

    def mi125_get_temperature(self, nb, tempmode):
        ret,temperature = eapi.MI125_mi125_get_temperature_send(self.state, nb, tempmode)
        if ret<0:
            raise bas_exception(ret)
        return temperature

    def mi125_get_channelcalibstatus(self, nb):
        ret,channellanecalib,channelcalibstatus = eapi.MI125_mi125_get_channelcalibstatus_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return channellanecalib,channelcalibstatus

    def mi125_get_versions(self, nb):
        ret,core_version,drv_version = eapi.MI125_mi125_get_versions_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return core_version,drv_version

    def mi125_checkadc(self, nb, adcid):
        ret,written,verify,detected = eapi.MI125_mi125_checkadc_send(self.state, nb, adcid)
        if (ret<0):
            raise bas_exception(ret)
        return written,verify,detected

    def mi125_set_trigout(self, nb, trigout):
        ret = eapi.MI125_mi125_set_trigout_send(self.state, nb, trigout)
        if (ret<0):
            raise bas_exception(ret)

    def mi125_get_clkmaster(self, nb):
        ret,master = eapi.MI125_mi125_get_clkmaster_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return master

    def mi125_clockreset(self,nb):
        ret = eapi.MI125_mi125_clockreset_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)

    def mi125_scan_i2c(self, nb, maxscan):
        ret, scanraw, scan_res, ndevices, detect_ok = eapi.MI125_mi125_scan_i2c_send(self.state, nb, maxscan)
        if ret<0:
            raise bas_exception(ret)
        return scanraw, scan_res, ndevices, detect_ok

    def mi125_test_localoscsw(self, nb):
        ret,detect = eapi.MI125_mi125_test_localoscsw_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return detect

