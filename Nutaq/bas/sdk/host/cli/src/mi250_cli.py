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
# @file      mi250_cli.py
# 
# @brief     Command-line interface for MI250
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

class mi250_cli:

    def do_mi250_select(self, line):
        args = line.split()
        if len(args) >= 1:
            num = int(args[0], 0)
            self.c.mi250_select(num)
        else:
            self.help_mi250_select()
            
    def help_mi250_select(self):
        print "mi250_select 'num' : Choose the MI250 on which FMC connector will be controlled by the next commands. 'num' starts at 1."

    def do_mi250_init(self, line):
        self.c.mi250_init()

    def help_mi250_init(self):
        print "mi250_init : resets MI250 and initialized ADC to default values"

    def do_mi250_powerup(self, line):
        self.c.mi250_powerup()

    def help_mi250_powerup(self):
        print "mi250_powerup : Powers up the FMC site for MI250 operation"

    def do_mi250_presence(self, line):
        self.c.mi250_presence()

    def help_mi250_presence(self):
        print "mi250_presence : Verifies the presence of the MI250 core and FMC daughter card"

    def do_mi250_pllconfig(self, line):
        args = line.split()
        if len(args) >= 2:
            try:
                refClk = int(args[0], 0)
                AdcClk = int(args[1], 0)
                Clocksource = int(args[2], 0)
                self.c.adc_clock = AdcClk
                self.c.ref_clock = refClk
                self.c.Clocksource = Clocksource
                if Clocksource == 2:
                    self.c.mi250_pllconfig(refClk, AdcClk, eapi.MI250_CLOCKIN_EXTERNALCLOCK)
                    print "PLL will be bypassed"
                elif Clocksource == 1:
                    self.c.mi250_pllconfig(refClk, AdcClk, eapi.MI250_CLOCKIN_EXTERNALREF)
                else:
                    self.c.mi250_pllconfig(refClk, AdcClk, eapi.MI250_CLOCKIN_100MHZ)
            except ValueError:
                print "invalid value"
        else:
            self.help_mi250_pll_config()

    def help_mi250_pllconfig(self):
        print "mi250_pll_config 'reference frequency' 'ADC sample frequency'  'Clock source' Configures the MI250 PLL to provide clock to ADC at specified frequency (Hz)."
        print  "Clock source is 0 for onboard reference (100MHz), 1 for external reference and 2 for external clock"

    def do_mi250_pllstatus(self, line):
        ret, status = eapi.MI250_PLLGetStatus_send(self.c.state)
        print "PLL status: ",
        if status == eapi.MI250_PLL_LOCKED:
            print "locked"
        elif status == eapi.MI250_PLL_UNLOCKED:
            print "unlocked"

    def help_mi250_pllstatus(self):
        print "mi250_status : prints current status of the MI250 PLL"

    def do_mi250_set_data_format(self, line):
        args = line.split()
        if len(args)>=2:
            device = int(args[0], 0)
            dataformat = int(args[1], 0)
            self.c.mi250_set_data_format(device, dataformat + 2)

    def help_mi250_set_data_format(self):
        print "mi250_set_data_format 'device' 'dataformat' : Sets the MI250 data format for a device"
        print "Device is the ADC chip to be configured. 0 for channels 1 and 2. 1 for channels 3 and 4. 2 for channels 5 and 6. 3 for channels 7 and 8."
        print "Data format is 0 for 2's complement and 1 for offset"

    def do_mi250_set_sleep_mode(self, line):
        args = line.split()
        if len(args)>=2:
            device = int(args[0], 0)
            sleepmode = int(args[1], 0)
            self.c.mi250_set_sleep_mode(device, sleepmode + 8)

    def help_mi250_set_sleep_mode(self):
        print "mi250_set_sleep_mode 'device' 'sleepmode' : Sets the MI250 sleep mode for a device"
        print "Device is the ADC chip to be configured. 0 for channels 1 and 2. 1 for channels 3 and 4. 2 for channels 5 and 6. 3 for channels 7 and 8."
        print "Sleep mode is 0 for normal operation"
        print "              1 for channel 2 disabled"
        print "              2 for channel 1 disabled"
        print "              3 for channel 1 and 2 disabled"
        print "              4 for global powerdown"
        print "              5 for channel 2 standby"
        print "              6 for channel 1 standby"

    def do_mi250_set_offset_correction_time(self, line):
        args = line.split()
        if len(args)>=3:
            device = int(args[0], 0)
            octa = int(args[1], 0)
            octb = int(args[1], 0)
            self.c.mi250_set_oct(device, octa, octb)

    def help_mi250_set_offset_correction_time(self):
        print "mi250_set_sleep_mode 'device' 'octa' 'octb'  : Sets the MI250 offset correction time for each channel of a device"
        print "Device is the ADC chip to be configured. 0 for channels 1 and 2. 1 for channels 3 and 4. 2 for channels 5 and 6. 3 for channels 7 and 8."
        print "Oct value is 256K * 2^octa to a maximum if 512M for octa = 11"
            

    def do_mi250_set_gain(self, line):
        args = line.split()
        if len(args)>=2:
            channel = int(args[0], 0)
            if channel == 1:
                channel = eapi.MI250_ADC_CHANNEL_1
                device = eapi.MI250_ADC_12
            elif channel == 2:
                channel = eapi.MI250_ADC_CHANNEL_2
                device = eapi.MI250_ADC_12
            elif channel == 3:
                channel = eapi.MI250_ADC_CHANNEL_1
                device = eapi.MI250_ADC_34
            elif channel == 4:
                channel = eapi.MI250_ADC_CHANNEL_2
                device = eapi.MI250_ADC_34
            elif channel == 5:
                channel = eapi.MI250_ADC_CHANNEL_1
                device = eapi.MI250_ADC_56
            elif channel == 6:
                channel = eapi.MI250_ADC_CHANNEL_2
                device = eapi.MI250_ADC_56
            elif channel == 7:
                channel = eapi.MI250_ADC_CHANNEL_1
                device = eapi.MI250_ADC_78
            elif channel == 8:
                channel = eapi.MI250_ADC_CHANNEL_2
                device = eapi.MI250_ADC_78
            else:
                print "Invalid channel number"
                self.help_mi250_set_gain()
                return
            gain = int(args[1], 0)
            if gain > 12:
                print "Gain is too hign"
                self.help_mi250_set_gain()
                return
            self.c.mi250_setgain(channel, device, gain)
        else:
            self.help_mi250_set_gain()

    def help_mi250_set_gain(self):
        print "mi250_set_gain 'channel'  'gain' : Set ADC gain. 1 for channel 1, 2 for channel 2, etc.. Gain is a value from 0 to 12 (0.5 dB gain per step)."

    def do_mi250_set_finegain(self, line):
        args = line.split()
        if len(args)>=2:
            channel = int(args[0], 0)
            if channel == 1:
                channel = eapi.MI250_ADC_CHANNEL_1
                device = eapi.MI250_ADC_12
            elif channel == 2:
                channel = eapi.MI250_ADC_CHANNEL_2
                device = eapi.MI250_ADC_12
            elif channel == 3:
                channel = eapi.MI250_ADC_CHANNEL_1
                device = eapi.MI250_ADC_34
            elif channel == 4:
                channel = eapi.MI250_ADC_CHANNEL_2
                device = eapi.MI250_ADC_34
            elif channel == 5:
                channel = eapi.MI250_ADC_CHANNEL_1
                device = eapi.MI250_ADC_56
            elif channel == 6:
                channel = eapi.MI250_ADC_CHANNEL_2
                device = eapi.MI250_ADC_56
            elif channel == 7:
                channel = eapi.MI250_ADC_CHANNEL_1
                device = eapi.MI250_ADC_78
            elif channel == 8:
                channel = eapi.MI250_ADC_CHANNEL_2
                device = eapi.MI250_ADC_78
            gain = int(args[1], 0)
            if gain > 127:
                print "Gain is too hign"
                self.mi250_set_finegain()
                return
            self.c.mi250_setfinegain(channel, device, gain)
        else:
            self.help_mi250_gain()

    def help_mi250_set_finegain(self):
        print "mi250_set_finegain 'channel' 'gain' : Set ADC fine gain. 1 for channel 1, 2 for channel 2, etc... Gain value are 0 to 127 and cover range of +0dB to + 0.134dB"

    def do_mi250_set_pedestal(self, line):
        args = line.split()
        if len(args)>=2:
            channel = int(args[0], 0)
            if channel == 0:
                channel = eapi.MI250_ADC_CHANNEL_1
                device = eapi.MI250_ADC_12
            elif channel == 1:
                channel = eapi.MI250_ADC_CHANNEL_2
                device = eapi.MI250_ADC_12
            elif channel == 2:
                channel = eapi.MI250_ADC_CHANNEL_1
                device = eapi.MI250_ADC_34
            elif channel == 3:
                channel = eapi.MI250_ADC_CHANNEL_2
                device = eapi.MI250_ADC_34
            elif channel == 4:
                channel = eapi.MI250_ADC_CHANNEL_1
                device = eapi.MI250_ADC_56
            elif channel == 5:
                channel = eapi.MI250_ADC_CHANNEL_2
                device = eapi.MI250_ADC_56
            elif channel == 6:
                channel = eapi.MI250_ADC_CHANNEL_1
                device = eapi.MI250_ADC_78
            elif channel == 7:
                channel = eapi.MI250_ADC_CHANNEL_2
                device = eapi.MI250_ADC_78
            value = int(args[1], 0)
            if value > 31:
                print "Pedestal value is too hign"
                self.help_mi250_set_pedestal()
                return
            if value < -32:
                print "Pedestal value is too low"
                self.help_mi250_et_pedestal()
                return

            self.c.mi250_setpedestal(channel, device, value)
        else:
            self.help_mi250_set_pedestal()

    def help_mi250_set_pedestal(self):
        print "mi250_set_pedestal 'channel' 'value' : Set ADC pedestal. 1 for channel 1, 2 for channel 2, etc... Pedestal values are -32 to 31 and are the offset added to the ADC signal in ADC increments"

    def do_mi250_adcreset(self, line):
        self.c.mi250_adcreset()

    def help_mi250_adcreset(self):
        print "mi250_adcreset : Resets all ADCs"

    def do_mi250_adcarm(self, line):
        args = line.split()
        if len(args)>=1:
            armstatus = int(args[0], 0)
            self.c.mi250_adcarm(armstatus)
        else:
            self.help_mi250_adcarm()

    def help_mi250_adcarm(self):
        print "mi250_adcarm 'armed status': Arms ADC channels in FPGA"

    def do_mi250_freqs(self, line):
        ret, value = eapi.memory_read_send(self.c.state, 0x8500004C)
        print "Reference clock %dMHz" % (value & 0xFFFF)
        print "To FPGA: %dMHz" % (value >> 16)
        ret, value = eapi.memory_read_send(self.c.state, 0x85000050)
        print "ch 1 and 2 ADC clock: %dMHz" % (value & 0xFFFF)
        print "ch 3 and 4 ADC clock: %dMHz" % (value >> 16)
        ret, value = eapi.memory_read_send(self.c.state, 0x85000054)
        print "ch 5 and 6 ADC clock: %dMHz" % (value & 0xFFFF)
        print "ch 7 and 8 ADC clock: %dMHz" % (value >> 16)

    def help_mi250_freqs(self):
        print "mi250_freqs : Prints the measures mi250 ADC frequencies"
