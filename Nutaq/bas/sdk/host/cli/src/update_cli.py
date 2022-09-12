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
# @file      update_cli.py
# 
# @brief     Command-line interface for update
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
    
class update_cli:
		
    def do_update_kernel(self,line):
        args = [p for p in re.split("( |\\\".*?\\\"|'.*?')", line) if p.strip()]
        if len(args)>=1:
            partition = 0
            name = ".ub"
            #remove " or ' at the end of the string
            args[0] = re.sub(r'^"|"$', '', args[0])
            args[0] = re.sub(r"^'|'$", '', args[0])
            if(name in args[0]):
                print "Please wait, this could take some time..."
                self.c.update_flash(partition,args[0])
            else :
                print "The Kernel filename should have the extension '.ub'"
                print "Aborting to update Kernel"
        else:
            self.help_update_kernel()

    def help_update_kernel(self):
        print "update_kernel 'kernel name' : Program a kernel image onto the onboard FLASH"

    def do_update_cce(self,line):
        args = [p for p in re.split("( |\\\".*?\\\"|'.*?')", line) if p.strip()]
        if len(args)>=1:
            partition = 1
            name = "cce"
            #remove " or ' at the end of the string
            args[0] = re.sub(r'^"|"$', '', args[0])
            args[0] = re.sub(r"^'|'$", '', args[0])
            if(name in args[0]):
                print "Please wait, this could take some time..."
                self.c.update_flash(partition,args[0])
            else :
                print "The CCE filename should be 'cce'"
                print "Aborting to update CCE"
        else:
            self.help_update_cce()

    def help_update_cce(self):
        print "update_cce 'cce name' : Program the CCE onto the onboard FLASH"

    def do_update_dtb(self,line):
        args = [p for p in re.split("( |\\\".*?\\\"|'.*?')", line) if p.strip()]
        if(len(args)==1):
            name = ".dtb"
            if(name in args[0]):
                print "Please wait, this could take some time..."
                #remove " or ' at the end of the string
                args[0] = re.sub(r'^"|"$', '', args[0])
                args[0] = re.sub(r"^'|'$", '', args[0])
                self.c.update_dtb(args[0],int(0),"Default")
            else:
                print "The DTB filename should have the extension '.dtb'"
                print "Aborting to update DTB"
        elif(len(args)==3):
            name = ".dtb"            
            if(name in args[0]):
                print "Please wait, this could take some time..."
                #remove " or ' at the end of the string
                args[0] = re.sub(r'^"|"$', '', args[0])
                args[0] = re.sub(r"^'|'$", '', args[0])
                args[2] = re.sub(r'^"|"$', '', args[2])
                args[2] = re.sub(r"^'|'$", '', args[2])
                self.c.update_dtb(args[0],int(args[1],0),args[2])        
            else:
                print "The DTB filename should have the extension '.dtb'"
                print "Aborting to update DTB"
        else:
            self.help_update_dtb()

    def help_update_dtb(self):
        print "update_dtb 'dtb name' : Program a DTB file onto the onboard FLASH"

    def do_update_uboot(self,line):
        args = [p for p in re.split("( |\\\".*?\\\"|'.*?')", line) if p.strip()]
        if len(args)>=1:
            partition = 2
            name = ".bin"
            #remove " or ' at the end of the string
            args[0] = re.sub(r'^"|"$', '', args[0])
            args[0] = re.sub(r"^'|'$", '', args[0])
            if(name in args[0]):
                print "Please wait, this could take some time..."
                self.c.update_flash(partition,args[0])
            else:
                print "The uBoot filename should have the extension '.bin'"
                print "Aborting to update uBoot"
        else:
            self.help_update_uboot()

    def help_update_uboot(self):
        print "update_uboot 'uboot name' : Program a uBoot image onto the onboard FLASH"

    def do_update_jffs2(self,line):
        args = [p for p in re.split("( |\\\".*?\\\"|'.*?')", line) if p.strip()]
        if len(args)>=1:
            partition = 3
            name = ".jffs2"
            #remove " or ' at the end of the string
            args[0] = re.sub(r'^"|"$', '', args[0])
            args[0] = re.sub(r"^'|'$", '', args[0])
            if(name in args[0]):
                print "Please wait, this could take some time..."
                self.c.update_flash(partition,args[0])
            else:
                print "The JFFS2 filename should have the extension '.jffs2'"
                print "Aborting to update JFFS2"
        else:
            self.help_update_jffs2()

    def help_update_jffs2(self):
        print "update_jffs2 'jffs2 name' : Program a JFFS2 image onto the onboard FLASH"

    def do_reboot(self, line):
        print "Rebooting Perseus. Restart CLI"
        ret = eapi.Carrier_Reboot_send(self.c.state)
        self.c.disconnect()
        raise ExitRequest

    def help_reboot(self):
        print "reboot : Reboot system"
        
