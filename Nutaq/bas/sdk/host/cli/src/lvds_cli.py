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
# @file      lvds_cli.py
# 
# @brief     Command-line interface for LVDS
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

class lvds_cli:


    def do_lvds_presence(self, line):
        args = line.split()
        if len(args)>=1:
            group = int(args[0], 0)
            self.c.lvds_presence(group)
        else:
            self.help_lvds_presence()    
    def help_lvds_presence(self):
        print "lvds_presence 'group': Verifies the LVDS presence"
        
    def do_lvds_getmode(self, line):
        args = line.split()
        if len(args)>=1:
            group = int(args[0], 0)
            self.c.lvds_getmode(group)
        else:
            self.help_lvds_getmode()
    def help_lvds_getmode(self):
        print "lvds_getmode 'group': Get the LVDS mode"
        
    def do_lvds_setoutputenable(self, line):
        args = line.split()
        if len(args)>=2:
            group = int(args[0], 0)
            outputenable = int(args[1], 0)
            self.c.lvds_setoutputenable(group, outputenable)
        else:
            self.help_lvds_setoutputenable()
    def help_lvds_setoutputenable(self):
        print "lvds_setoutputenable 'group' 'outputenable': Sets the output enable value to the LVDS core."
        
    def do_lvds_setvalue(self, line):
        args = line.split()
        if len(args)>=2:
            group = int(args[0], 0)
            value = int(args[1], 0)
            self.c.lvds_setvalue(group, value)
        else:
            self.help_lvds_setvalue()
    def help_lvds_setvalue(self):
        print "lvds_setvalue 'group' 'value': Sets the output value of the LVDS GPIO"
        
    def do_lvds_getvalue(self, line):
        args = line.split()
        if len(args)>=1:
            group = int(args[0], 0)
            ret, value = eapi.lvds_getvalue_send(self.c.state, group)
            if ret<0:
                raise bas_exception(ret)
            else:
                print "LVDS group ", group, ": ", hex(value)
        else:
            self.help_lvds_getvalue()
    def help_lvds_getvalue(self):
        print "lvds_getvalue 'group': Gets the input value of the LVDS GPIO"
        
    def do_lvds_reset(self, line):
        args = line.split()
        if len(args)>=1:
            group = int(args[0], 0)
            self.c.lvds_reset(group)
        else:
            self.help_lvds_reset()    
    def help_lvds_reset(self):
        print "lvds_reset 'group': Resets the LVDS core"
        
    def do_lvds_reset_fifo(self, line):
        args = line.split()
        if len(args)>=1:
            group = int(args[0], 0)
            self.c.lvds_reset_fifo(group)
        else:
            self.help_lvds_reset_fifo()    
    def help_lvds_reset_fifo(self):
        print "lvds_reset_fifo 'group': Resets the LVDS core"

    
   
