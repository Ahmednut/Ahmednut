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
# @file      mi125_cli.py
# 
# @brief     Command-line interface for MI125
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

class mi125_cli:

    def do_mi125_powerup(self, line):
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            self.c.mi125_powerup(nb)
        else:
            self.help_mi125_powerup()

    def help_mi125_powerup(self):
        print "mi125_powerup 'nb': Powers up MI125. 'nb': Choose which MI125 your are controlling (1 or 2)."
        print "Ex: mi125_powerup 1"

    def do_mi125_reset(self, line):
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            self.c.mi125_reset(nb)
        else:
            self.help_mi125_reset()

    def help_mi125_reset(self):
        print "mi125_reset 'nb': Resets and initializes MI125(ADC, clock routing, all options...) with default values. 'nb': Choose which MI125 your are controlling (1 or 2)."
        print "Ex: mi125_reset 1"

    def do_mi125_digital_adccalibration(self, line):
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            self.c.mi125_digital_adccalibration(nb)
        else:
            self.help_mi125_digital_adccalibration()

    def help_mi125_digital_adccalibration(self):
        print "mi125_digital_adccalibration 'nb': Forces ADC outputs digital calibration. 'nb': Choose which MI125 your are controlling (1 or 2)."
        print "Ex: mi125_digital_adccalibration 1"

    def do_mi125_set_config(self, line):
        args = line.split()
        if len(args) == 5:
            nb = int(args[0], 0)
            groupch = args[1]
            lvds = args[2]
            randmode = args[3]
            binmode = args[4]
            try:
                groupch = getattr(eapi, "MI125_" + groupch.upper())
            except AttributeError:
                print "invalid groupch channel configuration name %s" % groupch
                return
            try:
                lvds = getattr(eapi, "MI125_" + lvds.upper())
            except AttributeError:
                print "invalid lvds mode configuration name %s" % lvds
                return
            try:
                randmode = getattr(eapi, "MI125_" + randmode.upper())
            except AttributeError:
                print "invalid randmode random output configuration name %s" % randmode
                return
            try:
                binmode = getattr(eapi, "MI125_" + binmode.upper() + "FORMAT")
            except AttributeError:
                print "invalid binmode binary mode configuration name %s" % binmode
                return
            self.c.mi125_set_config(nb, groupch, lvds, randmode, binmode)
        else:
            self.help_mi125_set_config()
            
    def help_mi125_set_config(self):
       print "mi125_set_config 'nb' 'groupch' 'lvds' 'randmode' 'binmode': Configures board options. 'nb': Choose which MI125 your are controlling (1 or 2). 'groupch': channels group configuration. 'lvds': ADC output lvds configuration. 'randmode': ADC output randomize mode configuration. 'binmode': ADC output data format mode."
       print "Ex: mi125_set_config 1 16channels termon1750ua randomizeoff twocomplement"
       print """
GROUPCH    #  LVDS          # RANDMODE     # BINMODE
###########################################################
04CHANNELS #  TERMOFF1750UA # RANDOMIZEOFF # OFFSETBINARY
08CHANNELS #  TERMOFF2100UA # RANDOMIZEON  # TWOCOMPLEMENT
12CHANNELS #  TERMOFF2500UA #
16CHANNELS #  TERMOFF3000UA #
           #  TERMOFF3500UA #
           #  TERMOFF4000UA #
           #  TERMOFF4500UA #
           #  TERMON1750UA  #
           #  TERMON2100UA  #
           #  TERMON2500UA  #
###########################################################
"""

    def do_mi125_set_clksrc(self, line):
        args = line.split()
        if len(args) == 2:
            nb = int(args[0], 0)
            clksrc = args[1]
            try:
                clksrc = getattr(eapi, "MI125_CLKSRC" + clksrc.upper())
            except AttributeError:
                print "invalid clksrc ADC clock source configuration name %s" % clksrc
                return
            self.c.mi125_set_clksrc(nb, clksrc)
        else:
            self.help_mi125_set_clksrc()
            
    def help_mi125_set_clksrc(self):
       print "mi125_set_clksrc 'nb' 'clksrc': Configures ADC clock source. 'nb': Choose which MI125 your are controlling (1 or 2). 'clksrc': ADC clock source to be used."
       print "Ex: mi125_set_clksrc 1 125mhz"
       print """
CLKSRC           
###########
125MHZ     
EXT        
BOTTOMFMC  
FMCCARRIER 
###########
"""

    def do_mi125_checkcore(self, line): 
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            detected = self.c.mi125_checkcore(nb)
            print "MI125 Core status: ",
            if detected != 0:
                print "detected"
            else:
                print "not detected"
        else:
            self.help_mi125_checkcore()

    def help_mi125_checkcore(self):
        print "mi125_checkcore 'nb': Checks for MI125 fpga core presence. 'nb': Choose which MI125 your are controlling (1 or 2)."
        print "Ex: mi125_checkcore 1"

    def do_mi125_set_testmode(self, line):
        args = line.split()
        if len(args) == 3:
            nb = int(args[0], 0)
            testmode = args[1]
            pattern = int(args[2], 0)
            try:
                testmode = getattr(eapi, "MI125_" + testmode.upper())
            except AttributeError:
                print "invalid testmode ADC test mode configuration name %s" % testmode
                return
            self.c.mi125_set_testmode(nb, testmode, pattern)
        else:
            self.help_mi125_set_testmode()
            
    def help_mi125_set_testmode(self):
       print "mi125_set_testmode 'nb' 'testmode' 'pattern': Configures ADC test mode. 'nb': Choose which MI125 your are controlling (1 or 2). 'testmode': Which test mode to used. 'pattern': Which 14 bits (2 MSB bits will be 0) test pattern used when needed."
       print "Ex(user test mode): mi125_set_testmode 1 testmode2 0x33CC"
       print """
TESTMODE           
###########
TESTMODEOFF     
TESTMODE1        
TESTMODE2  
###########
"""

    def do_mi125_get_temperature(self, line):
        args = line.split()
        if len(args) == 2:
            nb = int(args[0], 0)
            tempmode = args[1]
            try:
                tempmode = getattr(eapi, "MI125_" + tempmode.upper())
            except AttributeError:
                print "invalid tempmode sensor temperature configuration name %s" % tempmode
                return
            temperature = self.c.mi125_get_temperature(nb, tempmode)
            print "Board pcb temperature is: ",
            print temperature
        else:
            self.help_mi125_get_temperature()

    def help_mi125_get_temperature(self):
        print "mi125_get_temperature 'nb' 'tempmode': Gets board pcb temperature. 'nb': Choose which MI125 your are controlling (1 or 2). 'tempmode': Temperature data output format to use."
        print "Ex: mi125_get_temperature 1 temp1c"
        print """
TEMPMODE           
##########
TEMP1C     
TEMP0DOT1C        
##########
"""

    def do_mi125_get_channelcalibstatus(self, line):
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            channellanecalib,channelcalibstatus = self.c.mi125_get_channelcalibstatus(nb)

            # Check if the last channels calibration was ok
            if channelcalibstatus != 0:
                print "All channels calibration successful!"
            else:
                print "Calibration problem, ADC lvds lane status (bit=1->ok, bit=0->bad): "
                print self.alt_bin(channellanecalib, 32)
        else:
            self.help_mi125_get_channelcalibstatus()

    def help_mi125_get_channelcalibstatus(self):
        print "mi125_get_channelcalibstatus 'nb': Gets ADC last digital calibration status. 'nb': Choose which MI125 your are controlling (1 or 2)."
        print "Ex: mi125_get_channelcalibstatus 1"

    def do_mi125_get_versions(self, line):
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            core_version,drv_version = self.c.mi125_get_versions(nb)
            print "MI125 fpga core version: ",
            print core_version
            print "MI125 firmware driver version: ",
            print drv_version
        else:
            self.help_mi125_get_versions()

    def help_mi125_get_versions(self):
        print "mi125_get_versions 'nb': Gets board fpga core and firmware versions. 'nb': Choose which MI125 your are controlling (1 or 2)."
        print "Ex: mi125_get_versions 1"

    def do_mi125_checkadc(self, line):
        args = line.split()
        if len(args) == 2:
            nb = int(args[0], 0)
            adcid = args[1]
            try:
                adcid = getattr(eapi, "MI125_" + adcid.upper())
            except AttributeError:
                print "invalid adcid ADC id configuration name %s" % adcid
                return
            written,verify,adctestok = self.c.mi125_checkadc(nb, adcid)
            if adctestok!=0:
                print "ADC test successful."
            else:
                print "ADC test failure."
                print " written: ",
                print hex(written)
                print " verify: ",
                print hex(verify)
        else:
            self.help_mi125_checkadc()

    def help_mi125_checkadc(self):
        print "mi125_checkadc 'nb' 'adcid': Runs an ADC check. 'nb': Choose which MI125 your are controlling (1 or 2). 'adcid': ADC id to verify."
        print "Ex: mi125_checkadc 1 adc0"
        print """
ADCID           
#####
ADC0     
ADC1        
ADC2
ADC3  
#####
"""

    def do_mi125_set_trigout(self, line):
        args = line.split()
        if len(args) == 2:
            nb = int(args[0], 0)
            trigout = args[1]
            try:
                trigout = getattr(eapi, "MI125_" + trigout.upper())
            except AttributeError:
                print "invalid trigout ADC trigger output configuration name %s" % trigout
                return
            self.c.mi125_set_trigout(nb, trigout)
        else:
            self.help_mi125_set_trigout()
            
    def help_mi125_set_trigout(self):
       print "mi125_set_trigout 'nb' 'trigout': Configures MI125 trigger output. 'nb': Choose which MI125 your are controlling (1 or 2). 'trigout': trigger output configuration."
       print "Ex: mi125_set_trigout 1 trigoutoff"
       print """
TRIGOUT           
##########
TRIGOUTON     
TRIGOUTOFF        
##########
"""

    def do_mi125_get_clkmaster(self, line):
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            master = self.c.mi125_get_clkmaster(nb)

            # Check if the module is clock master or not
            if master != 0:
                print "Module is a clock master!"
            else:
                print "Module is not a clock master!"
        else:
            self.help_mi125_get_clkmaster()

    def help_mi125_get_clkmaster(self):
        print "mi125_get_clkmaster 'nb': Gets if this module is a clock master. 'nb': Choose which MI125 your are controlling (1 or 2)."
        print "Ex: mi125_get_clkmaster 1"

    def do_mi125_clockreset(self, line):
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            self.c.mi125_clockreset(nb)
        else:
            self.help_mi125_clockreset()

    def help_mi125_clockreset(self):
        print "mi125_clockreset 'nb': Forces a MCM clock reset. 'nb': Choose which MI125 your are controlling (1 or 2)."
        print "Ex: mi125_clockreset 1"

    def do_mi125_scan_i2c(self, line):
        args = line.split()
        if len(args) == 2:
            nb = int(args[0], 0)
            maxscan = int(args[1], 0)
            if maxscan < 0 or maxscan > 127:
                print 'ERROR: maxscan must be in range [0,127]!'
                raise ValueError
            scanraw, scan_res, ndevices, detect_ok = self.c.mi125_scan_i2c(nb, maxscan)
            # Check if the scan was successful
            if detect_ok != 0:
                print "Module i2c scan test successful!"
            else:
                print "Module i2c scan test failure!"
                print "Number of devices found: ",ndevices
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
            self.help_mi125_scan_i2c()

    def help_mi125_scan_i2c(self):
        print "mi125_scan_i2c 'nb' 'maxscan': Do an i2c bus devices scan. 'nb': Choose which MI125 your are controlling (1 or 2). 'maxscan': Maximum number of devices that should be detected (On perseus, =5 for 1 mi125 instance, =9 for 2 mi125 instances), from 0 to 127 (0 means do not care about this)."
        print "Ex: mi125_scan_i2c 1 5"

    def do_mi125_test_localoscsw(self, line):
        args = line.split()
        if len(args) == 1:
            nb = int(args[0], 0)
            detect = self.c.mi125_test_localoscsw(nb)
            # Check if the clock oscillator test was successful
            if detect != 0:
                print "Module local oscillator test successful!"
            else:
                print "Module local oscillator test failure!"
        else:
            self.help_mi125_test_localoscsw()

    def help_mi125_test_localoscsw(self):
        print "mi125_test_localoscsw 'nb': Test local oscillator switch. 'nb': Choose which MI125 your are controlling (1 or 2)."
        print "Ex: mi125_test_localoscsw 1"

    def alt_bin(self, x, size):
        sign = '-' if x < 0 else ''
        x = abs(x)
        bits = []
        while x:
                x, rmost = divmod(x, 2)
                bits.append(rmost)
        return sign + ''.join(str(b) for b in reversed(bits or [0])).zfill(size)

