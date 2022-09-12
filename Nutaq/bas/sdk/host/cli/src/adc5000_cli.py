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
# @file      adc5000_cli.py
# 
# @brief     Command-line interface for ADC5000
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

class adc5000_cli:

    def do_adc5000_presence(self, line):
        self.c.adc5000_presence()
    def help_adc5000_powerup(self):
        print "adc5000_presence : Detects if ADC5000 is present"

    def do_adc5000_powerup(self, line):
        self.c.adc5000_powerup()
    def help_adc5000_powerup(self):
        print "adc5000_powerup : Powers ADC5000"

    def do_adc5000_reset(self, line):
        self.c.adc5000_reset()
    def help_adc5000_reset(self):
        print "adc5000_reset : Resets and initializes ADC5000(ADC, PLL, clock routing) with default values"

    def do_adc5000_setmode(self, line):
        args = line.split()
        if len(args) >= 1:
            value = int(args[0], 0)
            if value == 1:
                self.c.adc5000_mode = eapi.EV10AQ190_1CHANNEL_A
                self.c.adc5000_setmode(self.c.adc5000_mode)
            elif value == 2:
                self.c.adc5000_mode = eapi.EV10AQ190_2CHANNEL_A_C
                self.c.adc5000_setmode(self.c.adc5000_mode)
            elif value == 4:
                self.c.adc5000_mode = eapi.EV10AQ190_4CHANNEL_A_B_C_D
                self.c.adc5000_setmode(self.c.adc5000_mode)
            else:
                self.help_adc5000_setmode()
        else:
            self.help_adc5000_setmode()

    def help_adc5000_setmode(self):
        print "adc5000_setmode 'mode': Set ADC mode. 1 for 1 channel, 2 for 2 channels. 4 for 4 channels."

    def do_adc5000_setsync(self, line):
        args = line.split()
        if len(args) >= 1:
            value = int(args[0], 0)
            if value == 0:
                self.c.adc5000_sync = eapi.ADC5000_SYNC_EXT
                self.c.adc5000_setsync(self.c.adc5000_sync)
            elif value == 1:
                self.c.adc5000_sync = eapi.ADC5000_SYNC_CARRIER
                self.c.adc5000_setsync(self.c.adc5000_sync)
            elif value == 2:
                self.c.adc5000_sync = eapi.ADC5000_SYNC_PLL
                self.c.adc5000_setsync(self.c.adc5000_sync)
            elif value == 3:
                self.c.adc5000_sync = eapi.ADC5000_SYNC_OFF
                self.c.adc5000_setsync(self.c.adc5000_sync)
            else:
                self.help_adc5000_setsync()
        else:
            self.help_adc5000_setsync()
    def help_adc5000_setsync(self):
        print "adc5000_setsync 'sync': Set ADC sync source. 0 for external clock, 1 for carrier. 2 for PLL. 3 for no sync."

    def do_adc5000_plllock(self, line):
        ret, status = eapi.ADC5000_PLLLock_send(self.c.state)
        print "PLL status: ",
        if status == eapi.AD9517_PLL_UNLOCKED:
            print "unlocked"
        elif status == eapi.AD9517_PLL_LOCKED:
            print "locked"
        else:
            print "undefined"
    def help_adc5000_plllock(self):
        print "adc5000_plllock : Print current status of ADC5000 PLL."

    def do_adc5000_configurepll(self, line):
        args = line.split()
        if len(args) >= 6:
            reffreq = int(args[0], 0)
            adcfreq = int(args[1], 0)
            clockoutfreq = int(args[2], 0)
            fmcfreq = int(args[3], 0)
            syncoutfreq = int(args[4], 0)
            clocksource = int(args[5], 0)
            if clocksource == 0:
                self.c.adc5000_configurepll(reffreq, adcfreq, clockoutfreq, fmcfreq, syncoutfreq, 0)
            elif clocksource == 1:
                self.c.adc5000_configurepll(reffreq, adcfreq, clockoutfreq, fmcfreq, syncoutfreq, 3)
            elif clocksource == 2:
                self.c.adc5000_configurepll(reffreq, adcfreq, clockoutfreq, fmcfreq, syncoutfreq, 6)
            else:
                self.help_adc5000_configurepll()
        else:
            self.help_adc5000_configurepll()

    def help_adc5000_configurepll(self):
       print "adc5000_pllconfigure 'reffreq' 'adcfreq' 'clockoutfreq' 'fmcfreq' 'syncoutfreq' 'clocksource'  Clock source is 0 for external clock, 1 for external ref and 2 for internal ref"

    def do_adc5000_settestmode(self, line):
        args = line.split()
        if len(args) >= 1:
            value = int(args[0], 0)
            if value == 0:
                self.c.adc5000_testmode = eapi.EV10AQ190_DISABLE
                self.c.adc5000_settestmode(self.c.adc5000_testmode)
            elif value == 1:
                self.c.adc5000_testmode = eapi.EV10AQ190_BITFLASH
                self.c.adc5000_settestmode(self.c.adc5000_testmode)
            elif value == 2:
                self.c.adc5000_testmode = eapi.EV10AQ190_RAMP
                self.c.adc5000_settestmode(self.c.adc5000_testmode)
            else:
                self.help_adc5000_settestmode()
        else:
            self.help_adc5000_settestmode()
    def help_adc5000_settestmode(self):
        print "adc5000_settestmode 'testmode' 0 for normal mode, 1 for bitflash and 2 for ramp"

    def do_adc5000_calibrateiodelay(self, line):
        self.c.adc5000_calibrateiodelay()
    def help_adc5000_calibrateiodelay(self):
        print "adc5000_calibrateiodelay : Calibrate ADC5000 iodelay for the digital communication"

    def do_adc5000_getadcstatus(self, line):
        ret, statusA, statusB, statusC, statusD = eapi.ADC5000_GetAdcStatus_send(self.c.state)
        if ret < 0:
            print "ADC5000 could not get ADC status"
        else:
            if statusA == 1:
                print "ADC A : enable"
            else:
                print "ADC A : disable"
            if statusB == 1:
                print "ADC B : enable"
            else:
                print "ADC B : disable"
            if statusC == 1:
                print "ADC C : enable"
            else:
                print "ADC C : disable"
            if statusD == 1:
                print "ADC D : enable"
            else:
                print "ADC D : disable"
    def help_adc5000_getadcstatus(self):
        print "adc5000_getadcstatus : Print current status of ADC5000 ADCs."

    def do_adc5000_setstandby(self, line):
        args = line.split()
        if len(args) >= 1:
            value = int(args[0], 0)
            if value == 0:
                self.c.adc5000_standby = eapi.EV10AQ190_FULL_ACTIVE
                self.c.adc5000_setstandby(self.c.adc5000_standby)
            elif value == 1:
                self.c.adc5000_standby = eapi.EV10AQ190_STANDBY_A_B
                self.c.adc5000_setstandby(self.c.adc5000_standby)
            elif value == 2:
                self.c.adc5000_standby = eapi.EV10AQ190_STANDBY_C_D
                self.c.adc5000_setstandby(self.c.adc5000_standby)
            elif value == 3:
                self.c.adc5000_standby = eapi.EV10AQ190_FULL_STANDBY
                self.c.adc5000_setstandby(self.c.adc5000_standby)
            else:
                self.help_adc5000_setstandby()
        else:
            self.help_adc5000_setstandby()
    def help_adc5000_setstandby(self):
        print "adc5000_setstandby 'standby' 0 for full active, 1 for standby A and B, 2 for standby C and D and 3 for full standby."

    def do_adc5000_setcoding(self, line):
        args = line.split()
        if len(args) >= 1:
            value = int(args[0], 0)
            if value == 0:
                self.c.adc5000_coding = eapi.EV10AQ190_BINARY_CODING
                self.c.adc5000_setcoding(self.c.adc5000_coding)
            elif value == 1:
                self.c.adc5000_coding = eapi.EV10AQ190_GRAY_CODING
                self.c.adc5000_setcoding(self.c.adc5000_coding)
            else:
                self.help_adc5000_setcoding()
        else:
            self.help_adc5000_setcoding()
    def help_adc5000_setcoding(self):
        print "adc5000_setcoding 'coding' 0 for BINARY, 1 for GREY."

    def do_adc5000_setbandwidth(self, line):
        args = line.split()
        if len(args) >= 1:
            value = int(args[0], 0)
            if value == 0:
                self.c.adc5000_bandwidth = eapi.EV10AQ190_NOMINAL_BANDWIDTH
                self.c.adc5000_setbandwidth(self.c.adc5000_bandwidth)
            elif value == 1:
                self.c.adc5000_bandwidth = eapi.EV10AQ190_FULL_BANDWIDTH
                self.c.adc5000_setbandwidth(self.c.adc5000_bandwidth)
            else:
                self.help_adc5000_setbandwidth()
        else:
            self.help_adc5000_setbandwidth()
    def help_adc5000_setbandwidth(self):
        print "adc5000_setbandwidth 'bandwidth' 0 for NOMINAL bandwidth, 1 for FULL bandwidth."

    def do_adc5000_setadjtriggerdelay(self, line):
        args = line.split()
        if len(args) >= 1:
            value = int(args[0], 0)
            if value >= 1 and value <= 32:
                self.c.adc5000_setadjtriggerdelay(value)
            else:
                self.help_adc5000_setadjtriggerdelay()
        else:
            self.help_adc5000_setadjtriggerdelay()
    def help_adc5000_setadjtriggerdelay(self):
        print "adc5000_setadjtriggerdelay 'delay' the delay value must be between 1 and 32."

    def do_adc5000_setoffset(self, line):
        args = line.split()
        if len(args) >= 2:
            channel = int(args[0], 0)
            value = int(args[1], 0)
            if value >= 0 and value <= 1023:
                if channel == 1:
                    self.c.adc5000_channel = eapi.EV10AQ190_CHANNELA
                    self.c.adc5000_setoffset(self.c.adc5000_channel,value)
                elif channel == 2:
                    self.c.adc5000_channel = eapi.EV10AQ190_CHANNELB
                    self.c.adc5000_setoffset(self.c.adc5000_channel,value)
                elif channel == 3:
                    self.c.adc5000_channel = eapi.EV10AQ190_CHANNELC
                    self.c.adc5000_setoffset(self.c.adc5000_channel,value)
                elif channel == 4:
                    self.c.adc5000_channel = eapi.EV10AQ190_CHANNELD
                    self.c.adc5000_setoffset(self.c.adc5000_channel,value)
                else:
                    self.help_adc5000_setoffset()
            else:
                self.help_adc5000_setoffset()
        else:
            self.help_adc5000_setoffset()
    def help_adc5000_setoffset(self):
        print "adc5000_setoffset 'channel' 'offset'"
        print "  channel: ADC A = 1, ADC B = 2, ADC C = 3, ADC D = 4"
        print "  offset: value must be between 0 and 1023"

    def do_adc5000_setgain(self, line):
        args = line.split()
        if len(args) >= 2:
            channel = int(args[0], 0)
            value = int(args[1], 0)
            if value >= 0 and value <= 1023:
                if channel == 1:
                    self.c.adc5000_channel = eapi.EV10AQ190_CHANNELA
                    self.c.adc5000_setgain(self.c.adc5000_channel,value)
                elif channel == 2:
                    self.c.adc5000_channel = eapi.EV10AQ190_CHANNELB
                    self.c.adc5000_setgain(self.c.adc5000_channel,value)
                elif channel == 3:
                    self.c.adc5000_channel = eapi.EV10AQ190_CHANNELC
                    self.c.adc5000_setgain(self.c.adc5000_channel,value)
                elif channel == 4:
                    self.c.adc5000_channel = eapi.EV10AQ190_CHANNELD
                    self.c.adc5000_setgain(self.c.adc5000_channel,value)
                else:
                    self.help_adc5000_setgain()
            else:
                self.help_adc5000_setgain()
        else:
            self.help_adc5000_setgain()
    def help_adc5000_setgain(self):
        print "adc5000_setgain 'channel' 'gain'"
        print "  channel: ADC A = 1, ADC B = 2, ADC C = 3, ADC D = 4"
        print "  gain: value must be between 0 and 1023"

    def do_adc5000_setphase(self, line):
        args = line.split()
        if len(args) >= 2:
            channel = int(args[0], 0)
            value = int(args[1], 0)
            if value >= 0 and value <= 1023:
                if channel == 1:
                    self.c.adc5000_channel = eapi.EV10AQ190_CHANNELA
                    self.c.adc5000_setphase(self.c.adc5000_channel,value)
                elif channel == 2:
                    self.c.adc5000_channel = eapi.EV10AQ190_CHANNELB
                    self.c.adc5000_setphase(self.c.adc5000_channel,value)
                elif channel == 3:
                    self.c.adc5000_channel = eapi.EV10AQ190_CHANNELC
                    self.c.adc5000_setphase(self.c.adc5000_channel,value)
                elif channel == 4:
                    self.c.adc5000_channel = eapi.EV10AQ190_CHANNELD
                    self.c.adc5000_setphase(self.c.adc5000_channel,value)
                else:
                    self.help_adc5000_setphase()
            else:
                self.help_adc5000_setphase()
        else:
            self.help_adc5000_setphase()
    def help_adc5000_setphase(self):
        print "adc5000_setphase 'channel' 'phase'"
        print "  channel: ADC A = 1, ADC B = 2, ADC C = 3, ADC D = 4"
        print "  phase: value must be between 0 and 1023"

