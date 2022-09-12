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
# @file      backplane_commander.py
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
import os
import serial
from serial.tools import list_ports

class backplane_commander(bas_commander):
    def __init__(self):
        super(backplane_commander, self).__init__()
        self.backplane_port = list(self.backplane_get_port())
        self.backplane_chosen_port = ""

    def backplane_serial_ports(self):
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
            
    def backplane_get_port(self):
        """
        Returns all serial ports for Pico Backplane
        """        
        backplane_list_of_ports = list(self.backplane_serial_ports())
        for port in backplane_list_of_ports:
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
            if lines == []: #not Nutaq Pico Backplane
                ser.close()            
                continue
                
            toEnd = True
            for line in lines:
                if "NUTAQ" in line: # check if Nutaq Pico Backplane
                    toEnd = False                
                    break
            if toEnd == True:
                ser.close()
                continue            
            ser.close()
            yield port	    

    def backplane_send_command(self,command):
        """
        Write command line to Pico Backplane via serial port
        """
        try :
            ser = serial.Serial(self.backplane_chosen_port, 115200, timeout=0.1)
        except:
            print "Error : Could not send command. Make sure that serial port " + self.backplane_chosen_port + " isn't open with another program"
            return
	if ser.isOpen(): #port is already opened
            ser.close()
            
        ser.open()
        ser.flush()
        ser.write(command[0]) #send command key
        if command[0] == 'e' or command[0] == 's':
            lines = ser.readlines()
            for line in lines:
                if "E - Toggle PCIe clock source" in line and command[0] == 'e':
                    print line
                    break
                if "S - Toggle Autostart mode" in line and command[0] == 's':
                    print line
                    break
        ser.close()
