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
# @file      fmclvds_commander.py
# 
# @brief     Commander for FMC LVDS
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

class fmclvds_commander(bas_commander):
    def __init__(self):
        super(fmclvds_commander, self).__init__()
        
    def fmclvds_reset(self):
        ret = eapi.fmclvds_reset_send(self.state)
        if ret<0:
            raise bas_exception(ret)

    def fmclvds_powerup(self):
        ret = eapi.fmclvds_powerup_send(self.state)
        if ret<0:
            raise bas_exception(ret)

    def fmclvds_select(self, position):
        ret = eapi.fmclvds_select_send(self.state, position)
        if ret<0:
            raise bas_exception(ret)

    def fmclvds_set_channel_dir(self, group, direction):
        if group == 1:
            gr = eapi.FMCLVDS_GROUP_1
        else: 
            gr = eapi.FMCLVDS_GROUP_0
        if direction == 0:
            di = eapi.FMCLVDS_OUTPUT
        else: 
            di = eapi.FMCLVDS_INPUT
        ret = eapi.fmclvds_set_channel_dir_send(self.state, gr, di)
        if ret<0:
            raise bas_exception(ret)

    def fmclvds_set_channel_powerdown(self, group, powerdown):
        if group == 1:
            gr = eapi.FMCLVDS_GROUP_1
        else: 
            gr = eapi.FMCLVDS_GROUP_0
        if powerdown == 0:
            pw = eapi.FMCLVDS_PCF8574_POWERDOWN
        else: 
            pw = eapi.FMCLVDS_PCF8574_POWERUP
        ret = eapi.fmclvds_set_channel_powerdown_send(self.state, gr, pw)
        if ret<0:
            raise bas_exception(ret)

    def fmclvds_set_channel_preemphasis(self, group, preemp):
        if group == 1:
            gr = eapi.FMCLVDS_GROUP_1
        else: 
            gr = eapi.FMCLVDS_GROUP_0
        if preemp == 0:
            pr = eapi.FMCLVDS_PCF8574_PREEMPHASISOFF
        else: 
            pr = eapi.FMCLVDS_PCF8574_PREEMPHASISON
        ret = eapi.fmclvds_set_channel_preemphasis_send(self.state, gr, pr)
        if ret<0:
            raise bas_exception(ret)
