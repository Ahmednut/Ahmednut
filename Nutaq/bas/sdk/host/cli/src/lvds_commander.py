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
# @file      lvds_commander.py
# 
# @brief     Commander for LVDS
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

class lvds_commander(bas_commander):
    def __init__(self):
        super(lvds_commander, self).__init__()
        
    def lvds_presence(self, group):
        ret = eapi.lvds_presence_send(self.state, group)
        if ret<0:
            raise bas_exception(ret)
        else:
            print "Detected LVDS core, for group ", group
        
    def lvds_getmode(self, group):
        ret = eapi.lvds_getmode_send(self.state, group)
        if ret==eapi.LVDS_GPIO:
            print "LVDS GPIO core detected, for group ", group
        elif ret==eapi.LVDS_SYNC:
            print "LVDS Synchronous core detected, for group ", group
        elif ret<0:
            raise bas_exception(ret)
        else:
            print "No core detected, for group ", group

    def lvds_setoutputenable(self, group, outputenable):
        ret = eapi.lvds_setoutputenable_send(self.state, group, outputenable)
        if ret<0:
            raise bas_exception(ret)

    def lvds_setvalue(self, group, value):
        ret = eapi.lvds_setvalue_send(self.state, group, value)
        if ret<0:
            raise bas_exception(ret)

    def lvds_reset(self, group):
        ret = eapi.lvds_reset_send(self.state, group)
        if ret<0:
            raise bas_exception(ret)

    def lvds_reset_fifo(self, group):
        ret = eapi.lvds_reset_fifo_send(self.state, group)
        if ret<0:
            raise bas_exception(ret)

    
