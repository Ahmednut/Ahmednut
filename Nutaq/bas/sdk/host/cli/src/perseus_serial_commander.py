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
# @file      perseus_serial_commander.py
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
import time
import os
import serial
from serial.tools import list_ports

class perseus_serial_commander(bas_commander):
    def __init__(self):
        super(perseus_serial_commander, self).__init__()
        self.perseus_port = list(self.perseus_serial_get_port())
        self.perseus_chosen_port = ""
        self.perseus_login = False

    def perseus_serial_ports(self):
        """
        Returns a generator for all available serial ports
        """
        if os.name == 'nt':
            # windows
            for i in range(256):
                try:
                    s = serial.Serial(i)
                    s.close()
                    yield 'COM' + str(i + 1)
                except serial.SerialException:
                    pass
        else:
            # unix
            for port in list_ports.comports():
                yield port[0]
            
    def perseus_serial_get_port(self):
        """
        Returns all serial ports for Perseus
        """
        perseus_list_of_ports = list(self.perseus_serial_ports())
        for port in perseus_list_of_ports:
            try :
                ser = serial.Serial(port, 115200, timeout=0.1)
            except serial.SerialException:
                continue
            if ser.isOpen(): #port is already opened
                ser.close()
                
            ser.open()
            ser.flush()
            ser.write(chr(0xD)) #send 'enter' key      
            lines = ser.readlines()
            if lines == []: #not Perseus
                ser.close()            
                continue
                
            toEnd = True
            for line in lines:
                if "Perseus login:" in line: # check if Nutaq Perseus login
                    toEnd = False                
                    break
                elif "~ #" in line:
                    toEnd = False
                    ser.flush()
                    ser.write("exit")
                    ser.write(chr(0xD)) #enter key
            if toEnd == True:
                ser.close()
                continue            
            ser.close()
            yield port

    def perseus_serial_get_mac_address(self,port):
        """
        Get Perseus MAC Address
        """
        mac_addr = "MAC address not found"
        try:            
            ser = serial.Serial(port, 115200, timeout=0.1)
        except :
            print "Error : Could not find Perseus port"
            return mac_addr         
	if ser.isOpen(): #port is already opened
            ser.close()
            
        ser.open()
        ser.flush()
        ser.write("root") #log in
        ser.write(chr(0xD)) #enter key
        time.sleep(0.1)
        ser.write("root")
        ser.write(chr(0xD)) #enter key
        time.sleep(0.1)
        ser.write("ifconfig")
        ser.write(chr(0xD)) #enter key
        lines = ser.readlines()
        for line in lines:
            if "HWaddr " in line:
                mac_addr = line[-21:-4]
        ser.write("exit")
        ser.write(chr(0xD)) #enter key
        return mac_addr
    
    def perseus_serial_login(self):
        """
        Perseus Login
        """
        if self.perseus_login == True: #already log in
            return True
        else :
            try :
                ser = serial.Serial(self.perseus_chosen_port, 115200, timeout=0.1)
            except :
                print "Error : Could not login. Make sure that serial port " + self.perseus_chosen_port + " isn't open with another program"
                return False
            if ser.isOpen(): #port is already opened
                ser.close()
              
            ser.open()
            ser.flush()            
            ser.write(chr(0xD)) #send 'enter' key      
            lines = ser.readlines()            
            if lines == []: #not Perseus
                ser.close()            
                return False
            toEnd = True
            for line in lines:
                if "Perseus login:" in line: # check if Nutaq Perseus login
                    toEnd = False                
                    break
            if toEnd == True:
                ser.close()
                return False
            else :
                ser.write("root") #log in
                ser.write(chr(0xD)) #enter key
                time.sleep(0.1)
                ser.write("root")
                ser.write(chr(0xD)) #enter key
                time.sleep(0.1)
                self.perseus_login = True
                ser.close()
                return True

    def perseus_serial_logout(self):
        """
        Perseus Logout
        """
        if self.perseus_login == False: #already logout
            return True
        else :
            try:
                ser = serial.Serial(self.perseus_chosen_port, 115200, timeout=0.1)
            except :
                print "Error : Could not logout. Make sure that serial port " + self.perseus_chosen_port + " isn't open with another program"
                return False
            if ser.isOpen(): #port is already opened
                ser.close()
                
            ser.open()
            ser.flush()
            ser.write(chr(0xD)) #send 'enter' key      
            lines = ser.readlines()
            if lines == []: #not Perseus
                ser.close()            
                return False
            toEnd = True
            for line in lines:
                if "#" in line: # check if Nutaq Perseus command line
                    toEnd = False                
                    break
            if toEnd == True:
                ser.close()
                return False
            else :
                ser.write("exit")
                ser.write(chr(0xD)) #enter key
                self.perseus_login = False
                ser.close()
                return True

    def perseus_serial_write_command_line(self,command):
        """
        Write command line to Perseus via serial port
        """
        if(self.perseus_serial_login() != True): #unable to log in
            return []
        try:            
            ser = serial.Serial(self.perseus_chosen_port, 115200, timeout=0.1)
        except :
                print "Error : Could not send command. Make sure that serial port " + self.perseus_chosen_port + " isn't open with another program"
                return
        if ser.isOpen(): #port is already opened
            ser.close()
        
        ser.open()
        ser.flush()
        ser.write(command)
        ser.write(chr(0xD)) #enter key
        lines = ser.readlines()
        ser.close()
        return lines
