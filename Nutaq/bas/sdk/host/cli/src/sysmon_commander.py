﻿#****************************************************************************  
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
# @file      sysmon_commander.py
# 
# @brief     Commander for System Monitor
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

class sysmon_commander(bas_commander):
    def __init__(self):
        super(sysmon_commander, self).__init__()
        
    def sysmon_get_info(self):
        ret, temp = eapi.Sysmon_Read_send(self.state, eapi.Sysmon_eTemp)
        if ret<0:
            raise bas_exception(ret)
        ret, temp_min = eapi.Sysmon_Read_send(self.state, eapi.Sysmon_eTempMin)
        if ret<0:
            raise bas_exception(ret)
        ret, temp_max = eapi.Sysmon_Read_send(self.state, eapi.Sysmon_eTempMax)
        if ret<0:
            raise bas_exception(ret)
        ret, vint = eapi.Sysmon_Read_send(self.state, eapi.Sysmon_eVint)
        if ret<0:
            raise bas_exception(ret)
        ret, vint_min = eapi.Sysmon_Read_send(self.state, eapi.Sysmon_eVintMin)
        if ret<0:
            raise bas_exception(ret)
        ret, vint_max = eapi.Sysmon_Read_send(self.state, eapi.Sysmon_eVintMax)
        if ret<0:
            raise bas_exception(ret)
        ret, vaux = eapi.Sysmon_Read_send(self.state, eapi.Sysmon_eVaux)
        if ret<0:
            raise bas_exception(ret)
        ret, vaux_min = eapi.Sysmon_Read_send(self.state, eapi.Sysmon_eVauxMin)
        if ret<0:
            raise bas_exception(ret)
        ret, vaux_max = eapi.Sysmon_Read_send(self.state, eapi.Sysmon_eVauxMax)
        if ret<0:
            raise bas_exception(ret)
        
        print " "
        print "                       Min  Current    Max"
        print " Temperature (C) : %7s %7s %7s" % ("{:3.1f}".format(temp_min),"{:3.1f}".format(temp),"{:3.1f}".format(temp_max))
        print " VINT (V)        : %7s %7s %7s" % ("{:1.3f}".format(vint_min),"{:1.3f}".format(vint),"{:1.3f}".format(vint_max))
        print " VAUX (V)        : %7s %7s %7s" % ("{:1.3f}".format(vaux_min),"{:1.3f}".format(vaux),"{:1.3f}".format(vaux_max))
        print " "
    
