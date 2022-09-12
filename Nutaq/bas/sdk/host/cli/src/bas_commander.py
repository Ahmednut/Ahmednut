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
# @file      bas_commander.py
# 
# @brief     Commander for BAS
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
from bas_exception import *
import eapi

class bas_commander(object):
    def __init__(self):
        super(bas_commander, self).__init__()
        self.state = eapi.connection_state()
        self.update_only = 0;
        self.mac   = "00:00:00:00:00:00"

    def connect(self, ip):
        if self.state.stream:
            self.disconnect()
        ret = eapi.connect_cce(ip, self.state)
        
        self.update_only = 0;
        if (ret == eapi.EAPICCELIMITEDACCESS):
            self.update_only = 1;
        elif (ret == eapi.EAPICARRIERLIMITEDACCESS):
            self.update_only = 2;
        elif ret:
            raise bas_exception(ret)
    def disconnect(self):
        if self.state.stream:
            eapi.disconnect_cce(self.state)
    def cmd_send(self, cmd):
        ret = eapi.cmd_send(self.state, cmd)
        if (ret<0):
            raise bas_exception(ret)
    def buf_recv(self):
        ret, buf = eapi.buf_recv_alloc(self.state)
        if (ret<0):
            raise bas_exception(ret)
        return buf
    def turbo_mode(self):
        ret = eapi.set_turbo_mode(self.state)
        if (ret<0):
            raise bas_exception(ret)
    def normal_mode(self):
        ret = eapi.set_normal_mode(self.state)
        if (ret<0):
            raise bas_exception(ret)
