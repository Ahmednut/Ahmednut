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
# @file      perseus_commander.py
# 
# @brief     Commander for Perseus
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

class perseus_commander(bas_commander):
    def __init__(self):
        super(perseus_commander, self).__init__()
		
    def fpgaflash(self, index, name, comment):
        if comment is None:
            ret = eapi.fpgaflash_fromfile_send(self.state, name, index)
        else:
            ret = eapi.fpgaflash_fromfile_with_comment_send(self.state, name, index, comment)
        if (ret<0):
            raise bas_exception(ret)
            
    def fpgaflash_get_info(self):
    
        print ""
        
        ret, index = eapi.fpgaflash_get_index_send(self.state);
        if (ret >= 0 and index != 0):
            print "Current bitstream index is:"
            print " ", index
        elif (ret == eapi.CCEINVALIDBITSTREAM or (index == 0 and ret >= 0)):
            print "Current bitstream index is:"
            print " ", "undefined"
        else:
            raise bas_exception(ret)
        
        ret = eapi.fpgaflash_get_info_send(self.state, 1);
        if isinstance(ret, list):        
            print "Bitstream 1: "
            print " ", ret[1]
            print " ", ret[2]
            print " ", ret[3], ret[4]
            print " ", ret[5]
        elif (ret == eapi.CCEINVALIDBITSTREAM):
            print "Bitstream 1: "
            print " ", "no valid bitstream in flash"
        else:
            raise bas_exception(ret)
        
        ret = eapi.fpgaflash_get_info_send(self.state, 2);
        if isinstance(ret, list):        
            print "Bitstream 2: "
            print " ", ret[1]
            print " ", ret[2]
            print " ", ret[3], ret[4]
            print " ", ret[5]
        elif (ret == eapi.CCEINVALIDBITSTREAM):   
            print "Bitstream 2: "
            print " ", "no valid bitstream in flash"
        else:
            raise bas_exception(ret)
        
        print ""
        
    def fpgaflash_set_index(self, index):
        ret = eapi.fpgaflash_set_index_send(self.state, index);
        if (ret<0):
            raise bas_exception(ret)
			
    def read(self, addr):
        ret, value = eapi.memory_read_send(self.state, addr)
        if (ret<0):
            raise bas_exception(ret)
        return value
		
    def i2c_bus_scan(self, bus):
        ret, scanraw, ndevices = eapi.i2c_scan_devices_send(self.state, bus)
        if ret<0:
            raise bas_exception(ret)
        return scanraw, ndevices

    def write(self, addr, data):
        ret = eapi.memory_write_send(self.state, addr, data)
        if (ret<0):
            raise bas_exception(ret)

    def custom_register_read(self, addr_id):
        ret, value = eapi.custom_register_read_send(self.state, addr_id)
        if (ret<0):
            raise bas_exception(ret)
        return value

    def custom_register_write(self, addr_id, data):
        ret = eapi.custom_register_write_send(self.state, addr_id, data)
        if (ret<0):
            raise bas_exception(ret)

    def info_cmd_send(self, cmd, nb):
        ret = eapi.cmd_send(self.state, cmd)
        if (ret<0):
            raise bas_exception(ret)
        eapi.uint_send(self.state, nb)
    
    def info_buf_recv(self):
        ret, buf = eapi.buf_recv_alloc(self.state)
        if (ret<0):
            raise bas_exception(ret)
        return buf

    def info_result_recv(self):
        ret = eapi.result_recv(self.state)
        if (ret<0):
            raise bas_exception(ret)

    def version(self):
        self.cmd_send(0x00010002) # CCECTRL_VERSION
        buf = self.buf_recv()
        ret = eapi.result_recv(self.state)
        if (ret<0):
            raise bas_exception(ret)
        return buf
        
    def release_version(self):
        ret = eapi.get_eapi_version()
        if (ret<0):
            raise bas_exception(ret)           
        str_version = str(ret)
        if len(str_version) == 3:
            value = str_version[0] + '.' + str_version[1] + '.' + str_version[2]            
        elif len(str_version) == 4:
            value = str_version[0] + str_version[1] + '.' + str_version[2] + '.' + str_version[3]
        else:
            value = "Undefined"
        return value
    
    def getmac(self):
        self.cmd_send(0x00010020) # CCECTRL_GETMAC
        buf = self.buf_recv()
        ret = eapi.result_recv(self.state)
        if (ret<0):
            raise bas_exception(ret)
        return buf
