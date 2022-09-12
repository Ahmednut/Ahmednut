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
# @file      timestamp_cli.py
# 
# @brief     Command-line interface for System Monitor
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

class timestamp_cli:

    def do_timestamp_set_time(self, line):
        args = line.split()
        if len(args)>=1:
            try:
                value = long(args[0], 0)
                ret = eapi.Timestamp_SetTime_send(self.c.state, value)
                if ret<0:
                    raise bas_exception(ret)
            except ValueError:
                print 'Invalid value'
        else:
            self.help_timestamp_set_time()
            
    def help_timestamp_set_time(self):
        print "timestamp_set_time : Set the timestamp value at the next PPS rising edge event"
        
    def do_timestamp_get_time(self, line):
        ret, value = eapi.Timestamp_GetTime_send(self.c.state)
        if ret<0:
            raise bas_exception(ret)
            
        print hex(value)
            
    def help_timestamp_get_time(self):
        print "timestamp_get_time : Get the current timestamp value"
        
    def do_timestamp_reset(self, line):
        ret = eapi.Timestamp_Reset_send(self.c.state)
        if ret<0:
            raise bas_exception(ret)
            
    def help_timestamp_reset(self):
        print "timestamp_reset : Reset the timestamp value"
        

    
   
