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
# @file      mo1000_cli.py
# 
# @brief     Command-line interface for MO1000
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

class mo1000_cli:

    def do_mo1000_powerup(self, line):
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            self.c.mo1000_powerup(nb)
        else:
            self.help_mo1000_powerup()

    def help_mo1000_powerup(self):
        print "mo1000_powerup 'nb': Powers up MO1000. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_powerup 1"

    def do_mo1000_reset(self, line):
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            self.c.mo1000_reset(nb)
        else:
            self.help_mo1000_reset()

    def help_mo1000_reset(self):
        print "mo1000_reset 'nb': Resets a MO1000 to its default non operating condition. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_reset 1"

    def do_mo1000_init(self, line):
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            self.c.mo1000_init(nb)
        else:
            self.help_mo1000_init()

    def help_mo1000_init(self):
        print "mo1000_init 'nb': Initialize a MO1000 in default operating condition. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_init 1"

    def do_mo1000_writereg(self, line):
        args = line.split()
        if len(args) == 4:
            nb = int(args[0], 0)
            device = args[1]
            addr =  int(args[2], 16)
            value = int(args[3], 16)
            try:
                device = getattr(eapi, "eMo1000Device" + device.capitalize())
            except AttributeError:
                print "invalid peripheral device name %s" % device
                return
            devicedac1 = getattr(eapi, "eMo1000DeviceDac1")
            devicedac2 = getattr(eapi, "eMo1000DeviceDac2")
            devicepll = getattr(eapi, "eMo1000DevicePll")
            deviceports = getattr(eapi, "eMo1000DevicePorts")
            devicecore = getattr(eapi, "eMo1000DeviceCore")
            if devicedac1 == device or devicedac2 == device:
                if addr < 0 or addr > 0x72: 
                    print "invalid peripheral device address: ",
                    print hex(addr)
                    raise ValueError
                
                if value < 0 or value > 0xff: 
                    print "invalid peripheral device register value: ",
                    print hex(value)
                    raise ValueError
                
            elif devicepll == device:
                if addr < 0 or addr > 0x8: 
                    print "invalid peripheral device address: ",
                    print hex(addr)
                    raise ValueError
                
                if value < 0 or value > 0xfffffff: 
                    print "invalid peripheral device register value: ",
                    print hex(value)
                    raise ValueError
                
            elif devicecore == device:
                if addr < 0 or addr > 0x8: 
                    print "invalid peripheral device address: ",
                    print hex(addr)
                    raise ValueError
                
            elif deviceports == device:
                if addr < 0 or addr > 1: 
                    print "invalid peripheral device address: ",
                    print hex(addr)
                    raise ValueError
                
                if value < 0 or value > 0xff: 
                    print "invalid peripheral device register value: ",
                    print hex(value)
                    raise ValueError
                
            self.c.mo1000_writereg(nb, device, addr, value)
        else:
            self.help_mo1000_writereg()
            
    def help_mo1000_writereg(self):
       print "mo1000_writereg 'nb' 'device' 'addr' 'value': Writes a value in specified device register address. 'nb': Choose which MO1000 you are controlling (1 or 2). 'device': Peripheral device to access. 'addr': Register address (in hex) to access. 'value': Value to be written (in hex)."
       print "Ex: mo1000_writereg 1 dac1 0 0x10"
       print "WARNING: use for test purpose only, it may corrupt board behavior or damage it if used incorrectly."
       print """
device           
###########
dac1  #(addr: [hexadecimal 0x00-0x72] value: [hexadecimal 0x00-0xFF])
dac2  #(addr: [hexadecimal 0x00-0x72] value: [hexadecimal 0x00-0xFF])
pll   #(addr: [hexadecimal 0x00-0x08] value: [hexadecimal 0x0000000-0xFFFFFFFF)
ports #(addr: [hexadecimal 0x00-0x01] value: [hexadecimal 0x00-0xFF])
core  #(addr: [hexadecimal 0x00-0x08] value: [hexadecimal 0x00000000-0xFFFFFFFFF)
###########
"""

    def do_mo1000_readreg(self, line):
        args = line.split()
        if len(args) == 3:
            nb = int(args[0], 0)
            device = args[1]
            addr =  int(args[2], 16)
            try:
                device = getattr(eapi, "eMo1000Device" + device.capitalize())
            except AttributeError:
                print "invalid peripheral device name %s" % device
                return
            devicedac1 = getattr(eapi, "eMo1000DeviceDac1")
            devicedac2 = getattr(eapi, "eMo1000DeviceDac2")
            devicepll = getattr(eapi, "eMo1000DevicePll")
            deviceports = getattr(eapi, "eMo1000DevicePorts")
            devicecore = getattr(eapi, "eMo1000DeviceCore")
            if devicedac1 == device or devicedac2 == device:
                if addr < 0 or addr > 0x72: 
                    print "invalid peripheral device address: ",
                    print hex(addr)
                    raise ValueError
                
            elif devicepll == device or devicecore == device:
                if addr < 0 or addr > 0x8: 
                    print "invalid peripheral device address: ",
                    print hex(addr)
                    raise ValueError

            elif deviceports == device:
                if addr < 0 or addr > 1: 
                    print "invalid peripheral device address: ",
                    print hex(addr)
                    raise ValueError
                
            value = self.c.mo1000_readreg(nb, device, addr)
            print " value (HEX): ",
            print hex(value)
            print " value (BIN): ",
            print self.alt_bin(value, 32)
        else:
            self.help_mo1000_readreg()
            
    def help_mo1000_readreg(self):
       print "mo1000_readreg 'nb' 'device' 'addr': Reads a value from specified device register address. 'nb': Choose which MO1000 you are controlling (1 or 2). 'device': Peripheral device to access. 'addr': Register address to access (in hex)."
       print "Ex: mo1000_readreg 1 dac1 0 (register read is 0x00)"
       print """
device           
###########
dac1  #(addr: [hexadecimal 0x00-0x72])
dac2  #(addr: [hexadecimal 0x00-0x72])
pll   #(addr: [hexadecimal 0x00-0x08])
ports #(addr: [hexadecimal 0x00-0x01])
core  #(addr: [hexadecimal 0x00-0x08])
###########
"""

    def do_mo1000_getstatus(self, line): 
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            status, compare  = self.c.mo1000_getstatus(nb)
            print "MO1000 Board status (BIN):   ",
            print self.alt_bin(ord(status[3]), 8),
            print " ",
            print self.alt_bin(ord(status[2]), 8),
            print " ",
            print self.alt_bin(ord(status[1]), 8),
            print " ",
            print self.alt_bin(ord(status[0]), 8)
            print "MO1000 Compare errors (BIN): ",
            print self.alt_bin(ord(compare[3]), 8),
            print " ",
            print self.alt_bin(ord(compare[2]), 8),
            print " ",
            print self.alt_bin(ord(compare[1]), 8),
            print " ",
            print self.alt_bin(ord(compare[0]), 8)
            print "Status error(s):",
            if ord(status[0]) != 0 or ord(status[1]) != 0 or ord(status[2]) != 0 or (ord(status[3]) & 247) != 0 :
                print "detected"
            else:
                print "not detected"
            print "Pll status:",
            if (ord(status[3]) & 8) != 0 :
                print "locked"
            else:
                print "not locked"
            print "Compare error(s):",
            if ord(compare[0]) != 0 | ord(compare[1]) != 0 | ord(compare[2]) != 0 | ord(compare[3]) != 0 :
                print "detected"
            else:
                print "not detected"
        else:
            self.help_mo1000_getstatus()

    def help_mo1000_getstatus(self):
        print "mo1000_getstatus 'nb': Gets MO1000 board status and pattern test errors. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_getstatus 1"

    def do_mo1000_gettemperature(self, line): 
        args = line.split()
        if len(args) == 2:
            nb = int(args[0], 0)
            mode = args[1]
            try:
                mode = getattr(eapi, "eMo1000" + mode.capitalize())
            except AttributeError:
                print "invalid temperature mode name %s" % mode
                return
            temp, tdac1, tdac2  = self.c.mo1000_gettemperature(nb, mode)
            print "Pcb temperature is:  ",
            print temp
            print "Dac1 temperature is: ",
            print tdac1
            print "Dac2 temperature is: ",
            print tdac2
        else:
            self.help_mo1000_gettemperature()

    def help_mo1000_gettemperature(self):
        print "mo1000_gettemperature 'nb' 'tempmode': Gets board pcb and dacs temperatures. 'nb': Choose which MO1000 you are controlling (1 or 2). 'tempmode': Temperature data output format to use."
        print "Ex: mo1000_gettemperature 1 temp1c"
        print """
tempmode           
##########
temp1c     
temp0dot1c        
##########
"""

    def do_mo1000_getchannelcalibstatus(self, line): 
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            uChannelLaneCalib, uChannelFrameCalib, uChannelSyncCalib, uCalibStatus  = self.c.mo1000_getchannelcalibstatus(nb)
            # Check if the last calibration was ok
            if uCalibStatus != 0:
                print "All channels calibration successful!"
            else:
                print "Calibration problem, DAC data lanes status (bit=1->bad, bit=0->ok): "
                print self.alt_bin(uChannelLaneCalib, 32)
                print "Frame calibration status (bit=1->bad, bit=0->ok): "
                print self.alt_bin(uChannelFrameCalib, 32)
                print "Dac Synchronisation status (bit=1->bad, bit=0->ok): "
                print self.alt_bin(uChannelSyncCalib, 32)
        else:
            self.help_mo1000_getchannelcalibstatus()

    def help_mo1000_getchannelcalibstatus(self):
        print "mo1000_getchannelcalibstatus 'nb': Gets MO1000 board calibration status. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_getchannelcalibstatus 1"

    def do_mo1000_dodaccalibration(self, line):
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            self.c.mo1000_dodaccalibration(nb)
        else:
            self.help_mo1000_dodaccalibration()

    def help_mo1000_dodaccalibration(self):
        print "mo1000_dodaccalibration 'nb': Perform a MO1000 calibration. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_dodaccalibration 1"

    def do_mo1000_setdacoutctrl(self, line):
        args = line.split()
        if len(args) == 3:
            nb = int(args[0], 0)
            channel = args[1]
            state =  args[2]
            try:
                channel = getattr(eapi, "eMo1000SelectDac" + channel.capitalize())
            except AttributeError:
                print "invalid dac channel name %s" % channel
                return
            try:
                state = getattr(eapi, "eMo1000Output" + state.capitalize())
            except AttributeError:
                print "invalid dac output state %s" % state
                return
            self.c.mo1000_setdacoutctrl(nb, channel, state)
        else:
            self.help_mo1000_setdacoutctrl()
            
    def help_mo1000_setdacoutctrl(self):
       print "mo1000_setdacoutctrl 'nb' 'channel' 'state': Control the dac output state for the specified channel. 'nb': Choose which MO1000 you are controlling (1 or 2). 'channel': Which dac channel to control (1 to 8). 'state': Needed dac output state."
       print "Ex: mo1000_setdacoutctrl 1 1 enable"
       print """
state           
###########
enable  # set dac output enable    
disable # set dac output disable    
###########
"""

    def do_mo1000_setclockconfig(self, line):
        args = line.split()
        if len(args) == 6:
            nb = int(args[0], 0)
            SrcClk = args[1]
            uFreqSrcClk = int(args[2], 0)
            uFreqDacClock = int(args[3], 0)
            MasterClkMode =  args[4]
            uFreqMasterClock = int(args[5], 0)
            try:
                SrcClk = getattr(eapi, "eMo1000ClkSrc" + SrcClk.capitalize())
            except AttributeError:
                print "invalid source clock name %s" % SrcClk
                return
            try:
                MasterClkMode = getattr(eapi, "eMo1000MasterClk" + MasterClkMode.capitalize())
            except AttributeError:
                print "invalid master clock mode %s" % MasterClkMode
                return
            self.c.mo1000_setclockconfig(nb, SrcClk, uFreqSrcClk, uFreqDacClock, MasterClkMode, uFreqMasterClock)
        else:
            self.help_mo1000_setclockconfig()
            
    def help_mo1000_setclockconfig(self):
       print "mo1000_setclockconfig 'nb' 'SrcClk' 'FreqSrcClk' 'FreqDacClock' 'MasterClkMode' 'FreqMasterClock': Sets up the clocks of the module. 'nb': Choose which MO1000 you are controlling (1 or 2). 'SrcClk': Which clock source desired. 'FreqSrcClk': Source clock frequency when not 125 MHz internal clock. 'FreqDacClock': Dac clock frequency needed. 'MasterClkMode': master clock operation mode. 'FreqMasterClock': master clock frequency when MasterClkMode is in manual mode."
       print "Ex: mo1000_setclockconfig 1 125mhz 0 250000000 auto 0"
       print """
SrcClk           
######################################################################
125mhz      # set clock source to internal 125 MHz clock    
ext         # set clock source to external clock    
bottomfmc   # set clock source to bottom adapter (for top adapter only)    
fmccarrier2 # set clock source to fmc carrier clock CLK2    
fmccarrier3 # set clock source to fmc carrier clock CLK3    
######################################################################
MasterClkMode           
#########################################################################
auto    # set master clock mode automatic (for a double mo1000 stack)    
manual  # set  master clock mode manual (when top adapter is not a mo1000)    
#########################################################################
"""

    def do_mo1000_writeclockconfig(self, line):
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            self.c.mo1000_writeclockconfig(nb)
        else:
            self.help_mo1000_writeclockconfig()
            
    def help_mo1000_writeclockconfig(self):
       print "mo1000_writeclockconfig 'nb': Writes the module clocks setup prepared with mo1000_setclockconfig. 'nb': Choose which MO1000 you are controlling (1 or 2)."
       print "Ex: mo1000_writeclockconfig 1"

    def do_mo1000_getpllconfig(self, line):
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            self.c.mo1000_getpllconfig(nb)
        else:
            self.help_mo1000_getpllconfig()
            
    def help_mo1000_getpllconfig(self):
       print "mo1000_getpllconfig 'nb': Gets the module pll parameters prepared with mo1000_setclockconfig. 'nb': Choose which MO1000 you are controlling (1 or 2)."
       print "Ex: mo1000_getpllconfig 1"

    def do_mo1000_setpllconfig(self, line):
        args = line.split()
        if len(args) == 7:
            nb = int(args[0], 0)
            ChargePump = args[1]
            C1Value = args[2]
            R2Value = args[3]
            C2Value = args[4]
            R3Value = args[5]
            C3Value = args[6]
            try:
                ChargePump = getattr(eapi, "eCdce62005ChargePump" + ChargePump.upper())
            except AttributeError:
                print "invalid charge pump name %s" % ChargePump
                return
            try:
                C1Value = getattr(eapi, "eCdce62005C1Val" + C1Value.upper())
            except AttributeError:
                print "invalid C1 name %s" % C1Value
                return
            try:
                R2Value = getattr(eapi, "eCdce62005R2Val" + R2Value.upper())
            except AttributeError:
                print "invalid R2 name %s" % R2Value
                return
            try:
                C2Value = getattr(eapi, "eCdce62005C2Val" + C2Value.upper())
            except AttributeError:
                print "invalid C2 name %s" % C2Value
                return
            try:
                R3Value = getattr(eapi, "eCdce62005R3Val" + R3Value.upper())
            except AttributeError:
                print "invalid R3 name %s" % R3Value
                return
            try:
                C3Value = getattr(eapi, "eCdce62005C3Val" + C3Value.upper())
            except AttributeError:
                print "invalid C3 name %s" % C3Value
                return
            self.c.mo1000_setpllconfig(nb, ChargePump, C1Value, R2Value, C2Value, R3Value, C3Value)
        else:
            self.help_mo1000_setpllconfig()
            
    def help_mo1000_setpllconfig(self):
       print "mo1000_setpllconfig 'nb' 'ChargePump' 'C1Value' 'R2Value' 'C2Value' 'R3Value' 'C3Value': Sets the module pll loop filter parameters. 'nb': Choose which MO1000 you are controlling (1 or 2)."
       print "Ex: mo1000_setpllconfig 1 100D0UA 23D0PF 13D0KO 461D0PF 5D0KO 27D5PF"
       print """
ChargePump           
###########################################
50D0UA:     Charge pump current is 50.0 uA  
100D0UA:    Charge pump current is 100.0 uA 
150D0UA:    Charge pump current is 150.0 uA 
200D0UA:    Charge pump current is 200.0 uA 
300D0UA:    Charge pump current is 300.0 uA 
400D0UA:    Charge pump current is 400.0 uA 
600D0UA:    Charge pump current is 600.0 uA 
750D0UA:    Charge pump current is 750.0 uA 
1D0MA:      Charge pump current is 1.0 mA   
1D25MA:     Charge pump current is 1.25 mA  
1D5MA:      Charge pump current is 1.5 mA   
2D0MA:      Charge pump current is 2.0 mA   
2D5MA:      Charge pump current is 2.5 mA   
3D0MA:      Charge pump current is 3.0 mA   
3D5MA:      Charge pump current is 3.5 mA   
3D75MA:     Charge pump current is 3.75 mA  
###########################################
C1Value           
###############################
0D0PF:      C1 value is 0.0 pF 
1D5PF:      C1 value is 1.5 pF 
6D5PF:      C1 value is 6.5 pF 
8D0PF:      C1 value is 8.0 pF 
10D0PF:     C1 value is 10.0 pF
11D5PF:     C1 value is 11.5 pF
16D5PF:     C1 value is 16.5 pF
18D0PF:     C1 value is 18.0 pF
21D5PF:     C1 value is 21.5 pF
23D0PF:     C1 value is 23.0 pF
28D0PF:     C1 value is 28.0 pF
29D5PF:     C1 value is 29.5 pF
31D5PF:     C1 value is 31.5 pF
33D0PF:     C1 value is 33.0 pF
37D5PF:     C1 value is 37.5 pF
38D0PF:     C1 value is 38.0 pF
39D0PF:     C1 value is 39.0 pF
39D5PF:     C1 value is 39.5 pF
44D0PF:     C1 value is 44.0 pF
45D5PF:     C1 value is 45.5 pF
47D5PF:     C1 value is 47.5 pF
49D0PF:     C1 value is 49.0 pF
54D0PF:     C1 value is 54.0 pF
55D5PF:     C1 value is 55.5 pF
59D0PF:     C1 value is 59.0 pF
60D5PF:     C1 value is 60.5 pF
65D5PF:     C1 value is 65.5 pF
67D0PF:     C1 value is 67.0 pF
69D0PF:     C1 value is 69.0 pF
70D5PF:     C1 value is 70.5 pF
75D5PF:     C1 value is 75.5 pF
77D0PF:     C1 value is 77.0 pF
###############################
R2Value           
###################################
127D6KO:    R2 value is 127.6 KOhms
123D6KO:    R2 value is 123.6 KOhms
118D6KO:    R2 value is 118.6 KOhms
114D6KO:    R2 value is 114.6 KOhms
107D6KO:    R2 value is 107.6 KOhms
103D6KO:    R2 value is 103.6 KOhms
98D6KO:     R2 value is 98.6 KOhms 
94D6KO:     R2 value is 94.6 KOhms 
89D4KO:     R2 value is 89.4 KOhms 
85D4KO:     R2 value is 85.4 KOhms 
80D4KO:     R2 value is 80.4 KOhms 
76D4KO:     R2 value is 76.4 KOhms 
71D2KO:     R2 value is 71.2 KOhms 
69D4KO:     R2 value is 69.4 KOhms 
67D2KO:     R2 value is 67.2 KOhms 
65D4KO:     R2 value is 65.4 KOhms 
62D2KO:     R2 value is 62.2 KOhms 
60D4KO:     R2 value is 60.4 KOhms 
58D2KO:     R2 value is 58.2 KOhms 
56D4KO:     R2 value is 56.4 KOhms 
51D2KO:     R2 value is 51.2 KOhms 
47D2KO:     R2 value is 47.2 KOhms 
42D2KO:     R2 value is 42.2 KOhms 
38D2KO:     R2 value is 38.2 KOhms 
33D0KO:     R2 value is 33.0 KOhms 
29D0KO:     R2 value is 29.0 KOhms 
24D0KO:     R2 value is 24.0 KOhms 
20D0KO:     R2 value is 20.0 KOhms 
13D0KO:     R2 value is 13.0 KOhms 
9D0KO:      R2 value is 19.0 KOhms 
4D0KO:      R2 value is 4.0 KOhms  
0D1KO:      R2 value is 0.1 KOhms  
###################################
C2Value           
################################
0D0PF:      C2 value is 0.0 pF  
12D5PF:     C2 value is 12.5 pF 
25D0PF:     C2 value is 25.0 pF 
37D5PF:     C2 value is 37.5 pF 
87D0PF:     C2 value is 87.0 pF 
95D5PF:     C2 value is 95.5 pF 
112D0PF:    C2 value is 112.0 pF
123D0PF:    C2 value is 123.0 pF
124D5PF:    C2 value is 124.5 pF
135D5PF:    C2 value is 135.5 pF
148D0PF:    C2 value is 148.0 pF
160D5PF:    C2 value is 160.5 pF
210D0PF:    C2 value is 210.0 pF
222D5PF:    C2 value is 222.5 pF
226D0PF:    C2 value is 226.0 pF
235D0PF:    C2 value is 235.0 pF
238D5PF:    C2 value is 238.5 pF
247D5PF:    C2 value is 247.5 pF
251D0PF:    C2 value is 251.0 pF
263D5PF:    C2 value is 263.5 pF
313D0PF:    C2 value is 313.0 pF
325D5PF:    C2 value is 325.5 pF
338D0PF:    C2 value is 338.0 pF
349D0PF:    C2 value is 349.0 pF
350D5PF:    C2 value is 350.5 pF
361D5PF:    C2 value is 361.5 pF
374D0PF:    C2 value is 374.0 pF
386D5PF:    C2 value is 386.5 pF
436D0PF:    C2 value is 436.0 pF
448D5PF:    C2 value is 448.5 pF
461D0PF:    C2 value is 461.0 pF
473D5PF:    C2 value is 473.5 pF
################################
R3Value           
##################################
20D0KO:     R3 value is 20.0 KOhms
15D0KO:     R3 value is 15.0 KOhms
10D0KO:     R3 value is 10.0 KOhms
5D0KO:      R3 value is 5.0 KOhms 
##################################
C3Value           
################################
0D0PF:      C3 value is 0.0 pF  
2D5PF:      C3 value is 2.5 pF  
5D5PF:      C3 value is 5.5 pF  
8D0PF:      C3 value is 8.0 pF  
19D5PF:     C3 value is 19.5 pF 
22D0PF:     C3 value is 22.0 pF 
25D0PF:     C3 value is 25.0 pF 
27D5PF:     C3 value is 27.5 pF 
85D0PF:     C3 value is 85.0 pF 
87D5PF:     C3 value is 87.5 pF 
90D5PF:     C3 value is 90.5 pF 
93D0PF:     C3 value is 93.0 pF 
104D5PF:    C3 value is 104.5 pF
107D0PF:    C3 value is 107.0 pF
110D0PF:    C3 value is 110.0 pF
112D5PF:    C3 value is 112.5 pF
################################
"""

    def do_mo1000_setdacparinterpolation(self, line):
        args = line.split()
        if len(args) == 2:
            nb = int(args[0], 0)
            mode = args[1]
            try:
                mode = getattr(eapi, "eAd9148Inter" + mode.capitalize())
            except AttributeError:
                print "invalid interpolation mode name %s" % mode
                return
            self.c.mo1000_setdacparinterpolation(nb, mode)
        else:
            self.help_mo1000_setdacparinterpolation()
            
    def help_mo1000_setdacparinterpolation(self):
       print "mo1000_setdacparinterpolation 'nb' 'mode': Configures interpolation mode for all dac channels. 'nb': Choose which MO1000 you are controlling (1 or 2). 'mode': Needed interpolation mode."
       print "Ex: mo1000_setdacparinterpolation 1 1x"
       print """
mode           
###############################################################################
1x        :    Interpolation 1x                                                
2x        :    Interpolation 2x no Fcenter offset                              
2x_fsdiv4 :    Interpolation 2x with Fcenter offset Fs/4 (with premodulation)  
2x_fsdiv2 :    Interpolation 2x with Fcenter offset Fs/2                       
2x_3fsdiv4:    Interpolation 2x with Fcenter offset 3Fs/4 (with premodulation) 
4x        :    Interpolation 4x no Fcenter offset                              
4x_fsdiv8 :    Interpolation 4x with Fcenter offset Fs/8 (with premodulation)  
4x_fsdiv4 :    Interpolation 4x with Fcenter offset Fs/4                       
4x_3fsdiv8:    Interpolation 4x with Fcenter offset 3Fs/8 (with premodulation) 
4x_fsdiv2 :    Interpolation 4x with Fcenter offset Fs/2                       
4x_5fsdiv8:    Interpolation 4x with Fcenter offset 5Fs/8 (with premodulation) 
4x_3fsdiv4:    Interpolation 4x with Fcenter offset 3Fs/4                      
4x_7fsdiv8:    Interpolation 4x with Fcenter offset 7Fs/8 (with premodulation) 
8x        :    Interpolation 8x no Fcenter offset                              
8x_fsdiv8 :    Interpolation 8x with Fcenter offset Fs/8                       
8x_fsdiv4 :    Interpolation 8x with Fcenter offset Fs/4                       
8x_3fsdiv8:    Interpolation 8x with Fcenter offset 3Fs/8                      
8x_fsdiv2 :    Interpolation 8x with Fcenter offset Fs/2                       
8x_5fsdiv8:    Interpolation 8x with Fcenter offset 5Fs/8                      
8x_3fsdiv4:    Interpolation 8x with Fcenter offset 3Fs/4                      
8x_7fsdiv8:    Interpolation 8x with Fcenter offset 7Fs/8                      
###############################################################################
"""

    def do_mo1000_setdacpardcoffset(self, line):
        args = line.split()
        if len(args) == 3:
            nb = int(args[0], 0)
            channel = args[1]
            offset =  int(args[2], 0)
            try:
                channel = getattr(eapi, "eMo1000SelectDac" + channel.capitalize())
            except AttributeError:
                print "invalid dac channel name %s" % channel
                return
            if (offset > 32767 or offset < -32768):
                print "invalid dc offset [-32768,32767]:"
                print offset
                raise ValueError
            self.c.mo1000_setdacpardcoffset(nb, channel, offset)
        else:
            self.help_mo1000_setdacpardcoffset()
            
    def help_mo1000_setdacpardcoffset(self):
       print "mo1000_setdacpardcoffset 'nb' 'channel' 'offset': Control the output dc offset for the specified dac channel. 'nb': Choose which MO1000 you are controlling (1 or 2). 'channel': Which dac channel to control (1 to 8). 'offset': Needed dac output dc offset ([-32768,32767])."
       print "Ex: mo1000_setdacpardcoffset 1 1 100"

    def do_mo1000_setdacpargain(self, line):
        args = line.split()
        if len(args) == 3:
            nb = int(args[0], 0)
            channel = args[1]
            gain =  int(args[2], 0)
            try:
                channel = getattr(eapi, "eMo1000SelectDac" + channel.capitalize())
            except AttributeError:
                print "invalid dac channel name %s" % channel
                return
            if (gain < 0 or gain > 255):
                print "invalid gain [0-255]:"
                print gain
                raise ValueError
            self.c.mo1000_setdacpargain(nb, channel, gain)
        else:
            self.help_mo1000_setdacpargain()
            
    def help_mo1000_setdacpargain(self):
       print "mo1000_setdacpargain 'nb' 'channel' 'gain': Control the digital gain for the specified dac channel. 'nb': Choose which MO1000 you are controlling (1 or 2). 'channel': Which dac channel to control (1 to 8). 'gain': Needed dac gain correction (ScaleFactor = gain / 64)."
       print "Warning: gain greater than 64 (scale factor of 1) could cause signal saturation if this causes an overrange."
       print "Ex: mo1000_setdacpargain 1 1 64"

    def do_mo1000_dodacupdate(self, line):
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            self.c.mo1000_dodacupdate(nb)
        else:
            self.help_mo1000_dodacupdate()

    def help_mo1000_dodacupdate(self):
        print "mo1000_dodacupdate 'nb': Configure the dacs for operation with all the current parameters (defaults after 'mo1000_reset'). 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_dodacupdate 1"

    def do_mo1000_setdacparactchannel(self, line):
        args = line.split()
        if len(args) == 2:
            nb = int(args[0], 0)
            ActChannel = args[1]
            try:
                ActChannel = getattr(eapi, "eMo1000ActChannels0" + ActChannel.capitalize())
            except AttributeError:
                print "invalid active channels name %s" % ActChannel
                return
            self.c.mo1000_setdacparactchannel(nb, ActChannel)
        else:
            self.help_mo1000_setdacparactchannel()
            
    def help_mo1000_setdacparactchannel(self):
       print "mo1000_setdacparactchannel 'nb' 'actchannel': Configure the number of dac active channels (inactive are in power down). 'nb': Choose which MO1000 you are controlling (1 or 2). 'actchannel': Number of active channels."
       print "Ex: mo1000_setdacparactchannel 1 8"
       print """
actchannel           
#############################################
2        :        2 active channels
4        :        4 active channels
6        :        6 active channels
8        :        8 active channels (default)                       
#############################################
"""

    def do_mo1000_getclkmaster(self, line): 
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            master  = self.c.mo1000_getclkmaster(nb)
            # Check if the module is clock master or not
            if master != 0:
                print "Module is a clock master!"
            else:
                print "Module is not a clock master!"
        else:
            self.help_mo1000_getclkmaster()

    def help_mo1000_getclkmaster(self):
        print "mo1000_getclkmaster 'nb': Gets if this module is a clock master. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_getclkmaster 1"

    def do_mo1000_setdacparisinc(self, line):
        args = line.split()
        if len(args) == 12:
            nb = int(args[0], 0)
            DacPair = args[1]
            C0Real = int(args[2], 0)
            C1Real = int(args[3], 0)
            C2Real = int(args[4], 0)
            C3Real = int(args[5], 0)
            C4Real = int(args[6], 0)
            C0Img = int(args[7], 0)
            C1Img = int(args[8], 0)
            C2Img = int(args[9], 0)
            C3Img = int(args[10], 0)
            C4Img = int(args[11], 0)
            try:
                DacPair = getattr(eapi, "eMo1000SelectIQ" + DacPair.capitalize())
            except AttributeError:
                print "invalid dac pair name %s" % DacPair
                return
            if C0Real > 3 or C0Real < -4:
                print "invalid C0Real value, must be -4 <= C0Real <= 3"
                raise ValueError
            if C1Real > 7 or C1Real < -8:
                print "invalid C1Real value, must be -8 <= C1Real <= 7"
                raise ValueError
            if C2Real > 15 or C2Real < -16:
                print "invalid C2Real value, must be -16 <= C2Real <= 15"
                raise ValueError
            if C3Real > 63 or C3Real < -64:
                print "invalid C3Real value, must be -64 <= C3Real <= 63"
                raise ValueError
            if C4Real > 511 or C4Real < -512:
                print "invalid C4Real value, must be -512 <= C4Real <= 511"
                raise ValueError
            if C0Img > 3 or C0Img < -4:
                print "invalid C0Img value, must be -4 <= C0Img <= 3"
                raise ValueError
            if C1Img > 7 or C1Img < -8:
                print "invalid C1Img value, must be -8 <= C1Img <= 7"
                raise ValueError
            if C2Img > 15 or C2Img < -16:
                print "invalid C2Img value, must be -16 <= C2Img <= 15"
                raise ValueError
            if C3Img > 63 or C3Img < -64:
                print "invalid C3Img value, must be -64 <= C3Img <= 63"
                raise ValueError
            if C4Img > 511 or C4Img < -512:
                print "invalid C4Img value, must be -512 <= C4Img <= 511"
                raise ValueError
            self.c.mo1000_setdacparisinc(nb, DacPair, C0Real, C1Real, C2Real, C3Real, C4Real, C0Img, C1Img, C2Img, C3Img, C4Img)
        else:
            self.help_mo1000_setdacparisinc()
            
    def help_mo1000_setdacparisinc(self):
       print "mo1000_setdacparisinc 'nb' 'dacpair' 'C0Real' 'C1Real' 'C2Real' 'C3Real' 'C4Real' 'C0Img' 'C1Img' 'C2Img' 'C3Img' 'C4Img': Configure inverse sinc filter coefficients for the specified dac channels pair.. 'nb': Choose which MO1000 you are controlling (1 or 2). 'dacpair': Which dacs pair to configure. 'CxReal': real filter coeeficients. 'CxImg': Imaginary filter coefficients."
       print "Ex: mo1000_setdacparisinc 1 dac12 2 -4 10 -35 401 0 0 0 0 0"
       print """
dacpair
######################################
dac12     :    Dacs pair 1 (I) & 2 (Q)
dac34     :    Dacs pair 3 (I) & 4 (Q)
dac56     :    Dacs pair 5 (I) & 6 (Q)
dac78     :    Dacs pair 7 (I) & 8 (Q)
######################################
CxReal
##################################################
C0Real  # C0 real (I) coefficient [-4  , 3]
C1Real  # C1 real (I) coefficient [-8  , 7]
C2Real  # C2 real (I) coefficient [-16 , 15]
C3Real  # C3 real (I) coefficient [-64 , 63]
C4Real  # C4 real (I) coefficient [-512, 511]
##################################################
CxImg
##################################################
C0Img   # C0 imaginary (Q) coefficient [-4  , 3]
C1Img   # C1 imaginary (Q) coefficient [-8  , 7]
C2Img   # C2 imaginary (Q) coefficient [-16 , 15]
C3Img   # C3 imaginary (Q) coefficient [-64 , 63]
C4Img   # C4 imaginary (Q) coefficient [-512, 511]
##################################################
"""

    def do_mo1000_setdacparisincctrl(self, line):
        args = line.split()
        if len(args) == 3:
            nb = int(args[0], 0)
            group = args[1]
            state =  args[2]
            try:
                group = getattr(eapi, "eMo1000Dac" + group.capitalize())
            except AttributeError:
                print "invalid dac channel group name %s" % group
                return
            try:
                state = getattr(eapi, "eMo1000Isinc" + state.capitalize())
            except AttributeError:
                print "invalid dac inverse sinc filter state %s" % state
                return
            self.c.mo1000_setdacparisincctrl(nb, group, state)
        else:
            self.help_mo1000_setdacparisincctrl()
            
    def help_mo1000_setdacparisincctrl(self):
       print "mo1000_setdacparisincctrl 'nb' 'group' 'state': Control the dac inverse sinc filter state for the specified channels group. 'nb': Choose which MO1000 you are controlling (1 or 2). 'group': Which dac channels group to set the filter. 'state': Needed dac isinc filter state."
       print "Ex: mo1000_setdacparisincctrl 1 group14 enable"
       print """
group
###########
group14  # dac 1,2,3,4 channels group
group58  # dac 5,6,7,8 channels group
###########
state
###########
enable  # set dac inverse sinc filter enable
disable # set dac inverse sinc filter disable
###########
"""

    def do_mo1000_setdacparfinemod(self, line):
        args = line.split()
        if len(args) == 5:
            nb = int(args[0], 0)
            group = args[1]
            ftw =  int(args[2], 0)
            NcoPhaseOffset =  int(args[3], 0)
            sideband =  args[4]
            try:
                group = getattr(eapi, "eMo1000Dac" + group.capitalize())
            except AttributeError:
                print "invalid dac channel group name %s" % group
                return
            if ftw > 4294967295 or ftw < 0:
                print "invalid ftw value, must be 0 <= ftw <= 4294967295"
                raise ValueError
            if NcoPhaseOffset > 65535 or NcoPhaseOffset < 0:
                print "invalid NcoPhaseOffset value, must be 0 <= NcoPhaseOffset <= 65535"
                raise ValueError
            try:
                sideband = getattr(eapi, "eMo1000Nco" + sideband.capitalize())
            except AttributeError:
                print "invalid fine modulation side band name %s" % sideband
                return
            self.c.mo1000_setdacparfinemod(nb, group, ftw, NcoPhaseOffset, sideband)
        else:
            self.help_mo1000_setdacparfinemod()
            
    def help_mo1000_setdacparfinemod(self):
       print "mo1000_setdacparfinemod 'nb' 'group' 'ftw' 'ncophaseoffset' 'sideband': Configure the fine modulation parameters for the specified channels group. 'nb': Choose which MO1000 you are controlling (1 or 2). 'group': Which dac channels group to configure the Nco for. 'ftw': Desired frequency tuning word (uFtw = Fcenter/Fdac X 4294967296). 'ncophaseoffset': Desired Nco phase offset adjustement from 0 to 360 deg (each count adds 0.0054931640625 deg). 'sideband': which sideband is desired."
       print "Ex: mo1000_setdacparfinemod 1 group14 1223338666 16384 highside"
       print """
group
###########
group14  # dac 1,2,3,4 channels group
group58  # dac 5,6,7,8 channels group
###########
sideband
###########
highside # choose high side band
lowside  # choose low side band
###########
"""

    def do_mo1000_setdacparfinemodctrl(self, line):
        args = line.split()
        if len(args) == 3:
            nb = int(args[0], 0)
            group = args[1]
            state =  args[2]
            try:
                group = getattr(eapi, "eMo1000Dac" + group.capitalize())
            except AttributeError:
                print "invalid dac channel group name %s" % group
                return
            try:
                state = getattr(eapi, "eMo1000FineMod" + state.capitalize())
            except AttributeError:
                print "invalid dac fine modulation state %s" % state
                return
            self.c.mo1000_setdacparfinemodctrl(nb, group, state)
        else:
            self.help_mo1000_setdacparfinemodctrl()
            
    def help_mo1000_setdacparfinemodctrl(self):
       print "mo1000_setdacparfinemodctrl 'nb' 'group' 'state': Control the fine modulation state for the specified dac channels group. 'nb': Choose which MO1000 you are controlling (1 or 2). 'group': Which dac channels group to control. 'state': Needed dac fine modulation state."
       print "Ex: mo1000_setdacparfinemodctrl 1 group14 enable"
       print """
group
###########
group14  # dac 1,2,3,4 channels group
group58  # dac 5,6,7,8 channels group
###########
state
###########
enable  # set dac fine modulation enable
disable # set dac fine modulation disable
###########
"""

    def do_mo1000_setdacpardataformat(self, line):
        args = line.split()
        if len(args) == 3:
            nb = int(args[0], 0)
            group = args[1]
            format =  args[2]
            try:
                group = getattr(eapi, "eMo1000Dac" + group.capitalize())
            except AttributeError:
                print "invalid dac channel group name %s" % group
                return
            try:
                format = getattr(eapi, "eMo1000Format" + format.capitalize())
            except AttributeError:
                print "invalid dac format name %s" % format
                return
            self.c.mo1000_setdacpardataformat(nb, group, format)
        else:
            self.help_mo1000_setdacpardataformat()
            
    def help_mo1000_setdacpardataformat(self):
       print "mo1000_setdacpardataformat 'nb' 'group' 'format': Control the dac digital data format for the specified channels group. 'nb': Choose which MO1000 you are controlling (1 or 2). 'group': Which dac channels group to control. 'format': Needed dac data format."
       print "Ex: mo1000_setdacpardataformat 1 group14 2complement"
       print """
group
###########
group14  # dac 1,2,3,4 channels group
group58  # dac 5,6,7,8 channels group
###########
format
###########
2complement  # 2 complement data format (default)
binary       # binary data format
###########
"""

    def do_mo1000_setdacparquadphase(self, line):
        args = line.split()
        if len(args) == 4:
            nb = int(args[0], 0)
            DacPair = args[1]
            realphase = int(args[2], 0)
            imagphase = int(args[3], 0)
            try:
                DacPair = getattr(eapi, "eMo1000SelectIQ" + DacPair.capitalize())
            except AttributeError:
                print "invalid dac pair name %s" % DacPair
                return
            if realphase > 511 or realphase < -512:
                print "invalid realphase value, must be -512 <= realphase <= 511"
                raise ValueError
            if imagphase > 511 or imagphase < -512:
                print "invalid imagphase value, must be -512 <= imagphase <= 511"
                raise ValueError
            self.c.mo1000_setdacparquadphase(nb, DacPair, realphase, imagphase)
        else:
            self.help_mo1000_setdacparquadphase()
            
    def help_mo1000_setdacparquadphase(self):
       print "mo1000_setdacparquadphase 'nb' 'dacpair' 'realphase' 'imgphase': Configures quadrature phase for the specified dac channels pair.. 'nb': Choose which MO1000 you are controlling (1 or 2). 'dacpair': Which dacs pair to configure. 'realphase': real phase correction. 'imgphase': imaginary phase correction."
       print "Ex: mo1000_setdacparquadphase 1 dac12 26315 0"
       print """
dacpair           
######################################
dac12     :    Dacs pair 1 (I) & 2 (Q)
dac34     :    Dacs pair 3 (I) & 4 (Q)
dac56     :    Dacs pair 5 (I) & 6 (Q)
dac78     :    Dacs pair 7 (I) & 8 (Q)
######################################
##########################################################################
realphase  # real phase correction (0.00342 deg per count)      [-512,511]
imgphase   # imaginary phase correction (0.00342 deg per count) [-512,511]
##########################################################################
"""

    def do_mo1000_getversions(self, line): 
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            CoreVersion, DriverVersion  = self.c.mo1000_getversions(nb)
            print "Core version is:   ",
            print hex(CoreVersion)
            print "Driver version is: ",
            print hex(DriverVersion)
        else:
            self.help_mo1000_getversions()

    def help_mo1000_getversions(self):
        print "mo1000_getversions 'nb': Gets fpga core and mo1000 driver versions. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_getversions 1"

    def do_mo1000_testlocaloscsw(self, line): 
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            Result  = self.c.mo1000_testlocaloscsw(nb)
            if Result != 0:
                print "Test failed with error code:",
                print Result
            else:
                print "Test passed"
        else:
            self.help_mo1000_testlocaloscsw()

    def help_mo1000_testlocaloscsw(self):
        print "mo1000_testlocaloscsw 'nb': Test the local onboard clock oscillator switch control for diagnostic purpose. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_reset 1"
        print "    mo1000_testlocaloscsw 1"

    def do_mo1000_testbridgereset(self, line): 
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            Result  = self.c.mo1000_testbridgereset(nb)
            if Result != 0:
                print "Test failed with error code:",
                print Result
            else:
                print "Test passed"
        else:
            self.help_mo1000_testbridgereset()

    def help_mo1000_testbridgereset(self):
        print "mo1000_testbridgereset 'nb': Test the bridge reset operation for diagnostic purpose. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_reset 1"
        print "    mo1000_testbridgereset 1"

    def do_mo1000_testdacreset(self, line): 
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            Result  = self.c.mo1000_testdacreset(nb)
            if Result != 0:
                print "Test failed with error code:",
                print Result
            else:
                print "Test passed"
        else:
            self.help_mo1000_testdacreset()

    def help_mo1000_testdacreset(self):
        print "mo1000_testdacreset 'nb': Test the dacs reset operation for diagnostic purpose. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_reset 1"
        print "    mo1000_testdacreset 1"

    def do_mo1000_testpllsel(self, line): 
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            Result  = self.c.mo1000_testpllsel(nb)
            if Result != 0:
                print "Test failed with error code:",
                print Result
            else:
                print "Test passed"
        else:
            self.help_mo1000_testpllsel()

    def help_mo1000_testpllsel(self):
        print "mo1000_testpllsel 'nb': Test the pll mux selection function for diagnostic purpose. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_reset 1"
        print "    mo1000_testpllsel 1"
        print "    mo1000_reset 1"
        print "    mo1000_reset 2"
        print "    mo1000_testpllsel 2"

    def do_mo1000_testpllpowdown(self, line): 
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            Result  = self.c.mo1000_testpllpowdown(nb)
            if Result != 0:
                print "Test failed with error code:",
                print Result
            else:
                print "Test passed"
        else:
            self.help_mo1000_testpllpowdown()

    def help_mo1000_testpllpowdown(self):
        print "mo1000_testpllpowdown 'nb': Test the pll power down function for diagnostic purpose. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_reset 1"
        print "    mo1000_testpllpowdown 1"

    def do_mo1000_testpllsync(self, line): 
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            Result  = self.c.mo1000_testpllsync(nb)
            if Result != 0:
                print "Test failed with error code:",
                print Result
            else:
                print "Test passed"
        else:
            self.help_mo1000_testpllsync()

    def help_mo1000_testpllsync(self):
        print "mo1000_testpllsync 'nb': Test the pll sync function for diagnostic purpose. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_reset 1"
        print "    mo1000_testpllsync 1"

    def do_mo1000_testdacirq(self, line): 
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            Result  = self.c.mo1000_testdacirq(nb)
            if Result != 0:
                print "Test failed with error code:",
                print Result
            else:
                print "Test passed"
        else:
            self.help_mo1000_testdacirq()

    def help_mo1000_testdacirq(self):
        print "mo1000_testdacirq 'nb': Test the dac irq internal signals for diagnostic purpose. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_reset 1"
        print "    mo1000_testdacirq 1"

    def do_mo1000_testdacclks(self, line): 
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            Result  = self.c.mo1000_testdacclks(nb)
            if Result != 0:
                print "Test failed with error code:",
                print Result
            else:
                print "Test passed"
        else:
            self.help_mo1000_testdacclks()

    def help_mo1000_testdacclks(self):
        print "mo1000_testdacclks 'nb': Test the dac clocks internal signals for diagnostic purpose. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_reset 1"
        print "    mo1000_testdacclks 1"

    def do_mo1000_testdciframe(self, line): 
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            Result  = self.c.mo1000_testdciframe(nb)
            if Result != 0:
                print "Test failed with error code:",
                print Result
            else:
                print "Test passed"
        else:
            self.help_mo1000_testdciframe()

    def help_mo1000_testdciframe(self):
        print "mo1000_testdciframe 'nb': Test the dci+frame paths configuration for diagnostic purpose. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_powerup 1"
        print "    mo1000_reset 1"
        print "    mo1000_testdciframe 1"
        print "    mo1000_reset 2"
        print "    mo1000_testdciframe 2"

    def do_mo1000_checki2c(self, line): 
        args = line.split()
        if len(args) == 2:
            nb = int(args[0], 0)
            maxscan = int(args[1], 0)
            if maxscan < 0 or maxscan > 127:
                print 'ERROR: maxscan must be in range [0,127]!'
                raise ValueError
            if nb < 1:
                print 'ERROR: nb must be >= 1!'
                raise ValueError
            ScanRaw, NumDevices = self.c.i2c_bus_scan(((nb - 1) / 2) + 1)
            scan_res, detect_ok = self.c.mo1000_checki2c(nb, maxscan, ScanRaw, NumDevices)
            # Check if the scan was successful
            if detect_ok != 0:
                print "Module i2c scan test successful!"
            else:
                print "Module i2c scan test failure!"
                print "Number of devices found: ",NumDevices
                if scan_res[0] != '\x00':
                    print "Eeprom detected: yes."
                else:
                    print "Eeprom detected: no."
                if scan_res[1] != '\x00':
                    print "Spi bridge detected: yes."
                else:
                    print "Spi bridge detected: no."
                if scan_res[2] != '\x00':
                    print "Sensor detected: yes."
                else:
                    print "Sensor detected: no."
                if scan_res[3] != '\x00':
                    print "IO expander detected: yes."
                else:
                    print "IO expander detected: no."
        else:
            self.help_mo1000_checki2c()

    def help_mo1000_checki2c(self):
        print "mo1000_checki2c 'nb' 'maxscan': Scan and verify i2c bus to detect all module devices for diagnostic purpose. 'nb': Choose which MO1000 you are controlling (1 or 2). 'maxscan': Maximum number of devices that should be detected (On perseus, =5 for 1 mo1000 instance, =9 for 2 mo1000 instances), from 0 to 127 (0 means do not care about this)."
        print "Ex: mo1000_checki2c 1 5"

    def do_mo1000_getcorefrequency(self, line): 
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            CoreRefClock, DacRefClock  = self.c.mo1000_getcorefrequency(nb)
            print "FPGA core reference frequency is: ",
            print CoreRefClock
            print "Dacs reference frequency is:      ",
            print DacRefClock
        else:
            self.help_mo1000_getcorefrequency()

    def help_mo1000_getcorefrequency(self):
        print "mo1000_getcorefrequency 'nb': Gets fpga core and dacs reference frequencies. 'nb': Choose which MO1000 you are controlling (1 or 2)."
        print "Ex: mo1000_getcorefrequency 1"

    def do_mo1000_settestmode(self, line):
        args = line.split()
        if len(args) == 4:
            nb = int(args[0], 0)
            testmode = args[1]
            evenpattern = int(args[2], 0)
            oddpattern = int(args[3], 0)
            try:
                testmode = getattr(eapi, "eMo1000Test" + testmode.capitalize())
            except AttributeError:
                print "invalid test mode configuration name %s" % testmode
                return
            if evenpattern > 65535 or evenpattern < 0:
                print "invalid evenpattern value, must be 0 <= evenpattern <= 65535"
                raise ValueError
            if oddpattern > 65535 or oddpattern < 0:
                print "invalid oddpattern value, must be 0 <= oddpattern <= 65535"
                raise ValueError
            self.c.mo1000_settestmode(nb, testmode, evenpattern, oddpattern)
        else:
            self.help_mo1000_settestmode()
            
    def help_mo1000_settestmode(self):
       print "mo1000_settestmode 'nb' 'testmode' 'evenpattern' 'oddpattern': Configures DAC test mode. 'nb': Choose which MO1000 you are controlling (1 or 2). 'testmode': Which test mode to used. 'evenpattern': Which even 16 bits test pattern used when needed. 'oddpattern': Which odd 16 bits test pattern used when needed."
       print "Ex(user test mode): mo1000_settestmode 1 mode2 0x55AA 0xAA55"
       print "                    mo1000_getstatus 1"
       print "                    mo1000_settestmode 1 modeoff 0 0"
       print """
testmode
###########################################################
modeoff  : test mode off
mode1    : production test mode
mode2    : user test mode (patterns must be provided that a
           bit transition occurs between the even and odd
           patterns on each bits that need to be tested)
###########################################################
"""

    def alt_bin(self, x, size):
        sign = '-' if x < 0 else ''
        x = abs(x)
        bits = []
        while x:
                x, rmost = divmod(x, 2)
                bits.append(rmost)
        return sign + ''.join(str(b) for b in reversed(bits or [0])).zfill(size)

