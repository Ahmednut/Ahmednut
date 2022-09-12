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
# @file      fmclvds_cli.py
# 
# @brief     Command-line interface for FMC LVDS
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
import eapi

class fmclvds_cli:


    def do_fmclvds_reset(self, line):
        self.c.fmclvds_reset()

    def help_fmclvds_reset(self):
        print "fmclvds_reset : resets fmclvds and initializes to default values"

    def do_fmclvds_powerup(self, line):
        self.c.fmclvds_powerup()

    def help_fmclvds_powerup(self):
        print "fmclvds_powerup : Powers up the FMC site for fmclvds operation"

    def do_fmclvds_select(self, line):
        args = line.split()
        if len(args)>=1:
            position = int(args[0], 0)
            self.c.fmclvds_select(position)
        else:
            self.help_fmclvds_select()
        
    def help_fmclvds_select(self):
        print "fmclvds_select 'position': Selects the bottom card (0) or the top card (1)"

    def do_fmclvds_set_channel_dir(self, line):
        args = line.split()
        if len(args)>=2:
            group = int(args[0], 0)
            direction = int(args[1], 0)
            self.c.fmclvds_set_channel_dir(group, direction)
        else:
            self.fmclvds_set_channel_dir()
        
    def help_fmclvds_set_channel_dir(self):
        print "fmclvds_set_channel_dir 'group' 'direction': Sets the direction of an LVDS group. (0 for output, 1 for input)"

    def do_fmclvds_set_channel_powerdown(self, line):
        args = line.split()
        if len(args)>=2:
            group = int(args[0], 0)
            powerdownval = int(args[1], 0)
            self.c.fmclvds_set_channel_powerdown(group, powerdownval)
        else:
            self.help_fmclvds_powerdown()
        
    def help_fmclvds_set_channel_powerdown(self):
        print "fmclvds_set_channel_powerdown 'group' 'value': Powers up (1) or down (0) the LVDS group"
   
    def do_fmclvds_set_channel_preemphasis(self, line):
        args = line.split()
        if len(args)>=2:
            group = int(args[0], 0)
            preempval = int(args[1], 0)
            self.c.fmclvds_set_channel_preemphasis(group, preempval)
        else:
            self.help_fmclvds_preemphasis()
        
    def help_fmclvds_set_channel_preemphasis(self):
        print "fmclvds_set_channel_preemphasis 'group' 'value': Sets the preemphasis to ON (1) or OFF (0) value for the group"
   
