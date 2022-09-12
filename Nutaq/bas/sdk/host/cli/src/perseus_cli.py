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
# @file      perseus_cli.py
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

class perseus_cli:
    def do_fmc_info(self, line):
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0) - 1
            if nb < 0:
                nb = 0
        else:
            nb = 0
        print "Board#", nb + 1
        self.c.info_cmd_send(0x00500004, nb) # FMCEEPROM_EEPROM_GET_BOARD_STATE
        buf = self.c.info_buf_recv()
        self.c.info_result_recv()
        print "Board State:            ", buf[:len(buf)-1]
        self.c.info_cmd_send(0x00500005, nb) # FMCEEPROM_EEPROM_GET_BOARD_TYPE
        buf = self.c.info_buf_recv()
        self.c.info_result_recv()
        print "Board Identified type:  ", buf[:len(buf)-1]
        self.c.info_cmd_send(0x00500006, nb) # FMCEEPROM_EEPROM_GET_CORE
        buf = self.c.info_buf_recv()
        self.c.info_result_recv()
        print "Board FPGA core id:     ", buf[:len(buf)-1]
        self.c.info_cmd_send(0x0050000E, nb) # FMCEEPROM_EEPROM_GET_CORENAME
        buf = self.c.info_buf_recv()
        self.c.info_result_recv()
        print "Board FPGA core name:   ", buf[:len(buf)-1]
        self.c.info_cmd_send(0x00500007, nb) # FMCEEPROM_EEPROM_GET_CARRIERVADJ
        buf = self.c.info_buf_recv()
        self.c.info_result_recv()
        print "Board vAdj carrier set: ", buf[:len(buf)-1]
        self.c.info_cmd_send(0x00500003, nb) # FMCEEPROM_EEPROM_GET_IDSTRING
        buf = self.c.info_buf_recv()
        self.c.info_result_recv()
        print "Module Identification:  ", buf[:len(buf)-1]
        self.c.info_cmd_send(0x0050000D, nb) # FMCEEPROM_EEPROM_GET_MANUFACTURER
        buf = self.c.info_buf_recv()
        self.c.info_result_recv()
        print "Module Manufacturer:    ", buf[:len(buf)-1]
        self.c.info_cmd_send(0x00500008, nb) # FMCEEPROM_EEPROM_GET_PARTNUM
        buf = self.c.info_buf_recv()
        self.c.info_result_recv()
        print "Module Part Number:     ", buf[:len(buf)-1]
        self.c.info_cmd_send(0x00500009, nb) # FMCEEPROM_EEPROM_GET_SERIALNUM
        buf = self.c.info_buf_recv()
        self.c.info_result_recv()
        print "Module Serial Number:   ", buf[:len(buf)-1]
        self.c.info_cmd_send(0x0050000A, nb) # FMCEEPROM_EEPROM_GET_HARDREV
        buf = self.c.info_buf_recv()
        self.c.info_result_recv()
        print "Module Hardware Rev:    ", buf[:len(buf)-1]
        self.c.info_cmd_send(0x0050000B, nb) # FMCEEPROM_EEPROM_GET_FIRMREV
        buf = self.c.info_buf_recv()
        self.c.info_result_recv()
        print "Module Firmware Rev:    ", buf[:len(buf)-1]
        self.c.info_cmd_send(0x0050000C, nb) # FMCEEPROM_EEPROM_GET_MODVADJ
        buf = self.c.info_buf_recv()
        self.c.info_result_recv()
        print "Module vAdj info:       ", buf[:len(buf)-1]

    def help_fmc_info(self):
        print "fmc_info ['nb']: Print information about a carrier FMC daughter card.  'nb': Choose which board your are requesting info (1 or 2)."
        print "Ex: fmc_info 1"

    def do_fpgaflash(self, line):

        args = [p for p in re.split("( |\\\".*?\\\"|'.*?')", line) if p.strip()]

        if len(args)>=2:

            print "Please wait, this could take some time..."
            index = int(args[0], 0)

            #remove " or ' at the end of the string
            args[1] = re.sub(r'^"|"$', '', args[1])
            args[1] = re.sub(r"^'|'$", '', args[1])

            if len(args)>=3:
                #remove " or ' at the end of the string
                args[2] = re.sub(r'^"|"$', '', args[2])
                args[2] = re.sub(r"^'|'$", '', args[2])

                self.c.fpgaflash(index,args[1],args[2])
            else:
                self.c.fpgaflash(index,args[1],None)
        else:
            self.help_fpgaflash()

    def help_fpgaflash(self):
        print "fpgaflash index 'bitstream name' ['comment']: Program a bitstream onto the on-board flash memory. The index values supported are 1 and 2. The comment is optional but will be displayed when fpgaflash_get_info will be executed. If no comment is provided, the bitstream file name will be used instead."

    def do_fpgaflash_get_info(self, line):
        self.c.fpgaflash_get_info()

    def help_fpgaflash_get_info(self):
        print "fpgaflash_get_info : Print the bitstream index that will be loaded into the FPGA at system boot up. Print the information of bitstream 1 and bitstream 2 if they are present in the flash."

    def do_fpgaflash_set_index(self, line):
        args = line.split()
        if len(args)==1:
            index = int(args[0], 0)
            self.c.fpgaflash_set_index(index)
        else:
            self.help_fpgaflash_set_index()

    def help_fpgaflash_set_index(self):
        print "fpgaflash_set_index : Set the bitstream index that will be loaded into the FPGA at system boot up. The index values supported are 1 and 2."

    def do_read(self, line):
        args = line.split()
        if len(args) == 1:
            try:
                addr = int(args[0], 0)
                print hex(self.c.read(addr))
            except ValueError:
                self.help_read()
        elif len(args) >= 2:
            try:
                addr = int(args[0], 0)
                k = int(args[1], 0)
                for i in xrange(k):
                    addr_2_read = addr + i * 4
                    print hex(addr_2_read), ":", hex(self.c.read(addr_2_read))
            except ValueError:
                self.help_read()
        else:
            self.help_read()

    def help_read(self):
        print "read 'address' ['length']: Read 32 bits of data from an absolute address on the Microblaze AXI bus. Available addresses are in the following range: 0x60000000-0x7FFFFFFF and 0x85000000-0x8FFFFFFF."

    def do_i2c_bus_scan(self, line):
        args = line.split()
        if len(args) == 1:
            try:
                bus = int(args[0], 0)
                scanraw, ndevices = self.c.i2c_bus_scan(bus)
                # Check if the scan was successful
                print "Number of devices found:",ndevices
                print "Devices scanned (a detected one is surrounded with () ):"
                devcount = 0
                while devcount < 128:
                    mystring = ""
                    linecount = 0
                    while linecount < 8:
                        if scanraw[devcount + linecount] != '\x00':
                            mystring = mystring + "(0x" + '%.2x' % (devcount + linecount) + ") "
                        else:
                            mystring = mystring + " 0x" + '%.2x' % (devcount + linecount) + "  "
                        linecount = linecount + 1
                    print mystring
                    devcount = devcount + 8
            except ValueError:
                self.help_i2c_bus_scan()
        else:
            self.help_i2c_bus_scan()

    def help_i2c_bus_scan(self):
        print "i2c_bus_scan 'bus': Scan the specified i2c bus to detect all available devices.  'bus': Choose which bus to scan (currently: 1)."
        print "Ex: i2c_bus_scan 1"

    def do_write(self, line):
        args = line.split()
        if len(args) >=2:
            try:
                self.c.write(int(args[0], 0), int(args[1], 0))
            except ValueError:
                self.help_write()
        else:
            self.help_write()

    def help_write(self):
        print "write 'address' 'data' : Write 32 bits of data to an absolute address on the Microblaze AXI bus. Available addresses are in the following range: 0x60000000-0x7FFFFFFF and 0x85000000-0x8FFFFFFF."

    def do_custom_register_read(self, line):
        args = line.split()
        if len(args) == 1:
            try:
                addr_id = int(args[0], 0)
                print hex(self.c.custom_register_read(addr_id))
            except ValueError:
                self.help_custom_register_read()
        else:
            self.help_custom_register_read()

    def help_custom_register_read(self):
        print "custom_register_read 'custom register ID' : Read 32 bits of data from a custom register ID."

    def do_custom_register_write(self, line):
        args = line.split()
        if len(args) == 2:
            try:
                self.c.custom_register_write(int(args[0], 0), int(args[1], 0))
            except ValueError:
                self.help_custom_register_write()
        else:
            self.help_custom_register_write()

    def help_custom_register_write(self):
        print "custom_register_write 'custom register ID' 'data' : Write 32 bits of data to a custom register ID."

    def do_version(self, line):
        print self.c.version()

    def help_version(self):
        print "version : Print version of CCE to which we are currently connected"
        
    def do_release_version(self, line):
        print self.c.release_version()

    def help_release_version(self):
        print "release_version : Print ADP release version"    

    def do_getmac(self, line):
        mac = self.c.getmac()
        print "MAC address is: " + mac
        self.targetmac = mac

    def help_getmac(self):
        print "getmac : Prints the mac address of the target"

    def do_getip(self, line):
        ip = eapi.get_ip_address()
        print "IP address is: " + ip
        self.targetip = ip

    def help_getip(self):
        print "getip : Prints the ip address of the target"

    def do_targetinfo(self, line):
        print "Target MAC: " + self.targetmac
        print "Target IP: " + self.targetip

    def help_targetinfo(self):
        print "targetinfo : Prints the mac and ip addresses of the target"

