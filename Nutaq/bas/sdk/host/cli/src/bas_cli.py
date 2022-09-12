#!/usr/bin/python
# -*- coding: utf-8 -*-
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
# @file      bas_cli.py
# 
# @brief     Command-line interface for BAS
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
import sys
import os
import string
import optparse
from time import sleep
from cmd import Cmd
from cli_parser import parserException

from bas_exception import *
from bas_commander import bas_commander
from adac250_commander import adac250_commander
from adc5000_commander import adc5000_commander
from aurora_commander import aurora_commander
from mi125_commander import mi125_commander
from mo1000_commander import mo1000_commander
from mi250_commander import mi250_commander
from fmclvds_commander import fmclvds_commander
from lvds_commander import lvds_commander
from sysmon_commander import sysmon_commander
from fmcradio_commander import fmcradio_commander
from ppssync_commander import ppssync_commander
from perseus_commander import perseus_commander
from update_commander import update_commander
from adac250_cli import adac250_cli
from adc5000_cli import adc5000_cli
from aurora_cli import aurora_cli
from mi125_cli import mi125_cli
from mo1000_cli import mo1000_cli
from mi250_cli import mi250_cli
from ppssync_cli import ppssync_cli
from fmclvds_cli import fmclvds_cli
from lvds_cli import lvds_cli
from sysmon_cli import sysmon_cli
from timestamp_cli import timestamp_cli
from perseus_cli import perseus_cli
from update_cli import update_cli
from fmcradio_cli import fmcradio_cli
from osconfig_cli import osconfig_cli
from rtdex_recplay_cli import rtdex_recplay_cli

class system_cli:
    def do_shell(self, s):
        os.system(s)

    def help_shell(self):
        print "execute shell commands"

    def do_sleep(self, line):
        args = line.split()
        if len(args)>=1:
            sleep(int(args[0]))
        else:
            self.help_sleep()

    def help_sleep(self):
        print "sleep 'n' : Sleep for n seconds"

    def do_quit(self, line):
        sys.exit(0)

    def help_quit(self):
        print "quit : Exit the Command Line Interface"

    def help_help(self):
        print "help ['command name'] : When a command name is given, the help for that command will be written. If no command name is specified, a list of available command will be written."

    def do_exit(self, line):
        sys.exit(0)

    def help_exit(self):
        print "exit : Exit the Command Line Interface."

    def do_EOF(self, line):
        sys.exit(0)

    def help_EOF(self):
        print "End-of-file character : Exit the Command Line Interface."    

class start_cli(Cmd, system_cli):
    def __init__(self, commander, cur_input=sys.stdin, prompt="BAS$ ", script=0):
        Cmd.__init__(self, stdin=cur_input)
        self.c = commander
        self.prompt = prompt
        self.input = cur_input
        self.script = script
        if self.script == 1:
            self.abortonerror = 1
        else:
            self.abortonerror = 0

    def do_abort_on_error(self, line):
        self.abortonerror = 1
        print "now set to abort cli on error."

    def help_abort_on_error(self):
        print "set to abort cli on error (default for a script)"

    def do_cont_on_error(self, line):
        self.abortonerror = 0
        print "now set to continue cli on error."

    def help_cont_on_error(self):
        print "set to continue cli on error"

    def do_connect(self, line):
        args = line.split()
        #assert not self.c.state.stream
        if(len(args)>=1):
            try:
                ip=args[0]
                self.c.connect(args[0])
                if self.prompt:
                    newprompt = self.prompt[:-2]+"-"+ip+"$ "
                else:
                    newprompt=""
                module_list = [object, Cmd, system_cli]
                
                if(self.c.update_only == 1) or (self.c.update_only == 2):
                    ret = eapi.Carrier_Presence_send(self.c.state)
                    if ret >= 0:
                    
                        print "Connected to Perseus hardware with limited access"
                        if(self.c.update_only == 1):
                            print "since the CCE version does not match your API version."
                        elif(self.c.update_only == 2):
                            print "since the carrier board has not been identified."
                            
                        module_list.append(update_cli)
                        
                        if(self.c.update_only == 2):
                            module_list.append(perseus_cli)
                        
                else:
                    ret, carrier_type = eapi.Carrier_GetType_send(self.c.state)
                    if ret >= 0:
                        if carrier_type == eapi.Carrier_ePerseus601X:
                            print "Connected to Perseus601X carrier board"
                        if carrier_type == eapi.Carrier_ePerseus611X:
                            print "Connected to Perseus611X carrier board"
                    
                        module_list.append(perseus_cli)
                        module_list.append(update_cli)
                        module_list.append(rtdex_recplay_cli) 
                        module_list.append(sysmon_cli)
                        module_list.append(osconfig_cli)
                            
                    ret = eapi.fmc_radio_presence(self.c.state)
                    if ret >= 0:
                        print "Detected FMC Radio"
                        module_list.append(fmcradio_cli)
                    ret = eapi.ADAC250_Presence_send(self.c.state)
                    if ret >= 0:
                        print "Detected ADAC250 FMC"
                        module_list.append(adac250_cli)
                    ret = eapi.ADC5000_Presence_send(self.c.state)
                    if ret >= 0:
                        print "Detected ADC5000 FMC"
                        module_list.append(adc5000_cli)
                    ret = eapi.MI125_Presence_send(self.c.state,1)
                    ret1 = eapi.MI125_Presence_send(self.c.state,2)
                    if ret >= 0 or ret1 >= 0:
                        print "Detected MI125 FMC"
                        module_list.append(mi125_cli)
                    ret = eapi.Mo1000_Presence_send(self.c.state,1)
                    ret1 = eapi.Mo1000_Presence_send(self.c.state,2)
                    if ret >= 0 or ret1 >= 0:
                        print "Detected MO1000 FMC"
                        module_list.append(mo1000_cli)
                    ret = eapi.MI250_Presence_send(self.c.state)
                    if ret >= 0:
                        print "Detected MI250 FMC"
                        module_list.append(mi250_cli)
                    ret,ret2,ret3 = eapi.PPSSync_Presence_send(self.c.state)
                    if ret >= 0:
                        print "Detected PPS-SYNC"
                        module_list.append(ppssync_cli)                    
                    ret = eapi.fmclvds_presence_send(self.c.state)
                    if ret >= 0:
                        print "Detected LVDS FMC"
                        module_list.append(fmclvds_cli)
                        
                    ret0 = eapi.lvds_presence_send(self.c.state,0)
                    ret1 = eapi.lvds_presence_send(self.c.state,1)
                    ret2 = eapi.lvds_presence_send(self.c.state,2)
                    ret3 = eapi.lvds_presence_send(self.c.state,3)
                    if (ret0 >= 0):
                        print "Detected LVDS core group 0"
                    if (ret1 >= 0):
                        print "Detected LVDS core group 1"
                    if (ret2 >= 0):
                        print "Detected LVDS core group 2"
                    if (ret3 >= 0):
                        print "Detected LVDS core group 3"
                    if (ret0 >= 0) or (ret1 >= 0) or (ret2 >= 0) or (ret3 >= 0):
                        module_list.append(lvds_cli)
                        
                    ret = eapi.Timestamp_Presence_send(self.c.state)
                    if (ret >= 0):
                        print "Detected Timestamp module"
                        module_list.append(timestamp_cli)
                        
                    ret1, version = eapi.Aurora_GetVersion_send(self.c.state, 1)
                    ret2, version = eapi.Aurora_GetVersion_send(self.c.state, 2)
                    ret3, version = eapi.Aurora_GetVersion_send(self.c.state, 3)
                    ret4, version = eapi.Aurora_GetVersion_send(self.c.state, 4)
                    ret5, version = eapi.Aurora_GetVersion_send(self.c.state, 5)
                    ret6, version = eapi.Aurora_GetVersion_send(self.c.state, 6)
                    ret7, version = eapi.Aurora_GetVersion_send(self.c.state, 7)
                    ret8, version = eapi.Aurora_GetVersion_send(self.c.state, 8)
                    if (ret1 >= 0):
                        print "Detected Aurora core 1"
                    if (ret2 >= 0):
                        print "Detected Aurora core 2"
                    if (ret3 >= 0):
                        print "Detected Aurora core 3"
                    if (ret4 >= 0):
                        print "Detected Aurora core 4"
                    if (ret5 >= 0):
                        print "Detected Aurora core 5"
                    if (ret6 >= 0):
                        print "Detected Aurora core 6"
                    if (ret7 >= 0):
                        print "Detected Aurora core 7"
                    if (ret8 >= 0):
                        print "Detected Aurora core 8"
                    if (ret1 >= 0) or (ret2 >= 0) or (ret3 >= 0) or (ret4 >= 0) or (ret5 >= 0)or (ret6 >= 0) or (ret7 >= 0) or (ret8 >= 0):
                        module_list.append(aurora_cli)
                        
                    ret = eapi.os_config_load_file(self.c.state, "");
                    
                module_list.append(base_cli)
                module_list.reverse()
                self.c.getmac()
                bas_cli = type("bas_cli", tuple(module_list),dict())
                cmd = bas_cli(self.c, cur_input=self.input, prompt=newprompt)
                if not self.prompt:
                    cmd.use_rawinput=False
                try:
                    while True:
                        try:
                            cmd.cmdloop()
                        except ValueError:
                            print "Invalid value"
                        except IOError:
                            print 'File not found'
                        except TypeError as detail:
                            print 'Type error'
                            print detail
                        except OverflowError:
                            print 'Overflow error'
                        except bas_exception, e:
                            print e
                            if self.abortonerror == 1:
                                sys.exit(1)
                                
                        except parserException:
                            pass
                except ExitRequest:
                    pass
            except bas_exception, e:
                print e
                if self.abortonerror == 1:
                    sys.exit(1)
                    
        else:
            self.help_connect()

    def help_connect(self):
        print "connect 'ip_address' : Connect to a remote CCE"

class turbo_cli:
    def do_turbo_mode(self, line):
        self.c.turbo_mode()
    def help_turbo_mode(self):
        print "turbo_mode : Don't wait for result before sending next command. Commands will not return error."
    def do_normal_mode(self, line):
        self.c.normal_mode()
    def help_normal_mode(self):
        print "normal_mode : Switch to normal operating mode."

class base_cli:
    def __init__(self, commander, cur_input=sys.stdin, prompt="BAS$ "):
        Cmd.__init__(self, stdin=cur_input)
        self.c = commander
        self.prompt = prompt
        self.input = cur_input
        self.targetmac = "00:00:00:00:00:00"
        self.targetip = "000.000.000.000"

    def emptyline(self):
        pass

    def do_disconnect(self, line):
        self.c.disconnect()
        raise ExitRequest

    def help_disconnect(self):
        print "disconnect : Disconnect from a CCE process"

    def do_exit(self, line):
        self.do_disconnect(line)
        raise ExitRequest

    def do_quit(self, line):
        self.do_disconnect(line)
        raise ExitRequest

    def do_pause(self, line):
        print "Press enter to continue"
        sys.stdin.readline()

    def help_pause(self, line):
        print "pause : Wait for user input"

        
if __name__ == '__main__':
    commander_list = [fmcradio_commander, adac250_commander, adc5000_commander, aurora_commander, mi125_commander, mo1000_commander, mi250_commander, fmclvds_commander, lvds_commander, sysmon_commander, perseus_commander, update_commander, ppssync_commander, object]
    com_class = type("com_class", tuple(commander_list), dict())
    com = com_class()    
    if len(sys.argv) == 2:
        cmdfile = open(sys.argv[1])
        com = start_cli(com, cmdfile, prompt="", script=1)
        com.use_rawinput=False
    else:
        version_release = com.release_version()
        start_header = """Boards And Systems """ + version_release + """ - Command Line Interface
Nutaq"""
        print start_header
        com = start_cli(com)
    while True:
        try:
            ret = com.cmdloop()
            if ret:
                break
        except bas_exception, e:
            print e


