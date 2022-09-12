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
# @file      osconfig_cli.py
# 
# @brief     Command-line interface for OS configuration
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
import os
import eapi
from bas_exception import *

class osconfig_cli:
    
    def do_osconfig_create_static_entry(self,line):
        args = line.split()
        if len(args) == 4:
            key = "network.lan.proto"
            value = "static"
            ret = eapi.os_config_set_parameter(self.c.state, key, value)
            if ret >0:
                raise bas_exception(ret)
            key = "network.lan.ipaddr"
            value = args[1]
            ret = eapi.os_config_set_parameter(self.c.state, key, value)
            if ret >0:                
                raise bas_exception(ret)
            key = "network.lan.gateway"
            value = args[2]
            ret = eapi.os_config_set_parameter(self.c.state, key, value)
            if ret >0:                
                raise bas_exception(ret)
            key = "network.lan.netmask"
            value = args[3]
            ret = eapi.os_config_set_parameter(self.c.state, key, value)
            if ret >0:               
                raise bas_exception(ret)
            name = args[0]
            ret = eapi.os_config_save_file(self.c.state, name)
            if ret >0:               
                raise bas_exception(ret)
        else:
            self.help_osconfig_create_static_entry()

    def help_osconfig_create_static_entry(self):
         print "osconfig_create_static_entry 'name' 'ip' 'gateway' 'netmask'"    
            		
    def do_osconfig_create_dhcp_entry(self, line):
        args = line.split()
        if len(args) == 1:
            key = "network.lan.proto"
            value = "dhcp"
            ret = eapi.os_config_set_parameter(self.c.state, key, value)
            if ret >0:               
                raise bas_exception(ret)
            name = args[0]
            ret = eapi.os_config_save_file(self.c.state, name)
            if ret >0:               
                raise bas_exception(ret)
        else:
            self.help_osconfig_create_dhcp_entry()

    def help_osconfig_create_dhcp_entry(self):
        print "osconfig_create_dhcp_entry 'name'"

    def do_osconfig_useentry(self, line):
        args = line.split()
        if len(args) == 1:
            name = args[0]
            ret = eapi.os_config_load_file(self.c.state,name)
            if ret >0:               
                raise bas_exception(ret)
            ret = eapi.os_config_select_file(self.c.state)
            if ret >0:               
                raise bas_exception(ret)
        else:
            self.help_osconfig_useentry()

    def help_osconfig_useentry(self):
        print "osconfig_createdhcpentry 'name' : Entry is used for the next Linux boot"

    def do_osconfig_listentries(self, line):
        args = line.split()
        if len(args) == 0:
            print ""
            print "     Current loaded config"
            ret, key, value = eapi.os_config_get_first_parameter(self.c.state)
            if ret<0:
                raise bas_exception(ret)
            else:
                print "         " + key + value
            param = 1
            while param:
                ret, key, value = eapi.os_config_get_next_parameter(self.c.state)
                if ret<0:
                    param = 0
                else:
                    print "         " + key + value
            ret, name = eapi.os_config_get_first_file(self.c.state)
            if ret >0:       
                raise bas_exception(ret)
            else:
                print "     " + name
            ret = eapi.os_config_load_file(self.c.state, name)
            if ret >0:       
                raise bas_exception(ret)
            ret, key, value = eapi.os_config_get_first_parameter(self.c.state)
            if ret<0:
                raise bas_exception(ret)
            else:
                print "         " + key + value
            param = 1
            while param:
                ret, key, value = eapi.os_config_get_next_parameter(self.c.state)
                if ret<0:
                    param = 0
                    break
                else:
                    print "         " + key + value
            files = 10
            while files:
                files -= 1
                ret, name = eapi.os_config_get_next_file(self.c.state)
                if ret >0:
                    raise bas_exception(ret)
                if len(name) == 1:
                    files = 0
                    break
                print "     " + name
                ret = eapi.os_config_load_file(self.c.state, name)
                if ret >0:
                    raise bas_exception(ret)
                ret, key, value = eapi.os_config_get_first_parameter(self.c.state)
                if ret<0:
                    raise bas_exception(ret)
                else:
                    print "         " + key + value
                param = 1
                while param:
                    ret, key, value = eapi.os_config_get_next_parameter(self.c.state)
                    if ret<0:
                        param = 0
                    else:
                        print "         " + key + value
            print " " 
        else:
            self.help_osconfig_listentries()
           
    def help_osconfig_listentries(self):
        print "osconfig_listentries : List network config entries"


