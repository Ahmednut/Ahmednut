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
# @file      backplane_cli.py
# 
# @brief     Command-line interface for Perseus
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
import string
import optparse
import re
from bas_exception import *
from cmd import Cmd
from cli_parser import parserException

class backplane_cli:
    def do_power_cycle_a(self,line):
        self.c.backplane_send_command('a')
    def help_power_cycle_a(self):
        print "power_cycle_a : Power cycle SLOT A"
        
    def do_power_cycle_b(self,line):
        self.c.backplane_send_command('b')
    def help_power_cycle_b(self):
        print "power_cycle_b : Power cycle SLOT B"
        
    def do_power_cycle_a_and_b(self,line):
        self.c.backplane_send_command('c')
    def help_power_cycle_a_and_b(self):
        print "power_cycle_a_and_b : Power cycle SLOT A & SLOT B"
        
    def do_shutdown_a_and_b(self,line):
        self.c.backplane_send_command('d')
    def help_shutdown_a_and_b(self):
        print "shutdown_a_and_b : Shutdown SLOT A & SLOT B"

    def do_toggle_pcie_clocksource(self,line):
        self.c.backplane_send_command('e')
    def help_toggle_pcie_clocksource(self):
        print "toggle_pcie_clocksource : Toggle PCIe clock source between 'CABLE PCIE CLOCK' and 'BACKPLANE PCIE CLOCK'"

    def do_toggle_autostart_mode(self,line):
        self.c.backplane_send_command('s')
    def help_toggle_autostart_mode(self):
        print "toggle_autostart_mode : Toggle Autostart mode between 'ON' and 'OFF'"
   		
