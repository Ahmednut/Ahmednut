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
# @file      adac250_cli.py
# 
# @brief     Command-line interface for ADAC250
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

class adac250_cli:

    def do_adac250_presence(self, line):
        if self.c.adac250_presence():
            print "ADAC250 not detected."
    def help_adac250_presence(self):
        print "adac250_presence : Verifies if the ADAC250 FMC card and FPGA core are present"
				
    def do_adac250_powerup(self, line):
        if self.c.adac250_powerup():
            print "Could not power up FMC site for ADAC250 operation"
    def help_adac250_powerup(self):
        print "adac250_powerup : Powers up FMC site for ADAC250 operation"

    def do_adac250_pll_init(self, line):
        args = line.split()
        if len(args) >= 5:
            try:
                refClk = int(args[0], 0)
                AdcClk = int(args[1], 0)
                DacClk = int(args[2], 0)
                Interp = int(args[3], 0)
                Source = int(args[4], 0)
                print "Configuring PLL for:"               
                print "  - Reference Clock %d Hz"   % refClk
                print "  - ADC Clock %d Hz  "   % AdcClk
                print "  - DAC Clock %d Hz  "   % DacClk
                print "  - Interpolation factor %dx" % Interp
                if Source == 0:
                    print "  - Clock source internal reference"
                elif Source == 1:
                    print "  - Clock source external reference"
                else:
                    print "  - Clock source external clock "
                ret = self.c.adac250_pll_init(refClk, AdcClk, DacClk, Interp, Source)
            except ValueError:
                print "invalid value"
        else:
            self.help_adac250_pll_init()

    def help_adac250_pll_init(self):
        print "adac250_pll_init 'reference frequency' 'ADC sample frequency' 'DAC data frequency' 'Interpolation factor' 'Clock source': Configures the ADAC250 PLL to provide clock to ADC and DAC at specified frequencies (Hz)."

    def do_adac250_pll_setrefClkTuning(self, line):
        args = line.split()
        if len(args)>=1:
            value = int(args[0])
            self.c.adac250_setRefClkTuning(value)
        else:
            self.help_adac250_setrefClkTuning()

    def help_adac250_pll_setrefClkTuning(self):
        print "adac250_pll_setRefClkTuning 'value' : Tune reference frequency generator. Value is 16-bit wide."

    def do_adac250_pll_getstatus(self, line):
        ret,status = eapi.ADAC250_PLL_getStatus_send(self.c.state)
        if ret<0:
            raise bas_exception(ret)
        if status == eapi.ADAC250_PLL_LOCKED:
            print "PLL is locked"
        else: 
            print "PLL is unlocked"
        
    def help_adac250_pll_getstatus(self):
        print "adac250_pll_getstatus : Verifies and prints the PLL lock status"

    def do_adac250_dac_init(self, line):
        args = line.split()
        if len(args) >= 4:
            sleepmode = int(args[0], 0)
            interpolation = int(args[1], 0)
            mixmode_a = int(args[2], 0)
            mixmode_b = int(args[3], 0)
            self.c.adac250_dac_init(sleepmode, interpolation, mixmode_a, mixmode_b)
        else:
            self.help_adac250_dac_init()

    def help_adac250_dac_init(self):
        print "adac250_dac_init 'sleep_mode' 'interpolation_factor' 'mixmode_channelA' 'mixmode_channelB'"
        print "Initializes the DAC. "

    def do_adac250_dac_gain(self, line):
        args = line.split()
        if len(args)>=2:
            channel = int(args[0],0)
            gain = int(args[1],0)
            if channel >1:
                print "Invalid channel ID"
                self.help_adac250_dac_gain()
            elif gain >16:
                print "Invalid gain value"
                self.help_adac250_dac_gain()
            else:
                self.c.adac250_dac_setGain(channel, gain)
        else:
            self.help_adac250_dac_gain()

    def help_adac250_dac_gain(self):
        print "adac250_dac_gain 'channel' 'gain' : Changes the DAC gain of the specified channel. Gain in 0 to 15. "

#    def do_adac250_dac_setOffset(self, line):
#        args = line.split()
#        if len(args)>=2:
#            channel = int(args[0],0)
#            offset = int(args[1],0)
#            self.c.adac250_dac_setOffset(channel, offset)
#        else:
#            self.help_adac250_dac_setOffset()
#
#    def help_adac250_dac_setOffset(self):
#        print "adac250_dac_gain 'channel' 'offset' : Set DAC offset. 0 for channel A, 1 for channel B."

    def do_adac250_dac_sync(self, line):
        args = line.split()
        if len(args)>=1:
            sync = int(args[0],0)
            self.c.adac250_dac_sync(sync)
        else:
            self.help_adac250_dac_sync()

    def help_adac250_dac_sync(self):
        print "adac250_dac_sync 'enable' : Enables the DAC output when sync source is from DAC register. enable:1, disable:0"
		
    def do_adac250_dac_syncsource(self, line):
        args = line.split()
        if len(args)>=1:
            source = int(args[0],0)
            self.c.adac250_dac_syncsource(source)
        else:
            self.help_adac250_dac_syncsource()

    def help_adac250_dac_syncsource(self):
        print "adac250_dac_syncsource 'source' : Source is 0 for FPGA and 1 from DAC register "
		
    def do_adac250_dac_calibrate(self, line):
        if self.c.adac250_dac_calibrate():
            print "Could not calibrate ADC"

    def help_adac250_dac_calibrate(self):
        print "adac250_dac_calibrate : Calibrates the DAC data bus timings"

    def do_adac250_adc_init(self, line):
        if self.c.adac250_adc_init():
            print "Could not init ADC"

    def help_adac250_adc_init(self):
        print "adac250_adc_init"

    def do_adac250_adc_gain(self, line):
        args = line.split()
        if len(args)>=2:
            channel = int(args[0], 0)
            gain = int(args[1], 0)
            if channel > 1:
                print "Invalid channel ID"
                self.help_adac250_adc_gain()
            elif gain > 12:
                print "Invalid gain value"
                self.help_adac250_adc_gain()
            else:
                self.c.adac250_adc_setGain(channel, gain)
        else:
            self.help_adac250_adc_gain()

    def help_adac250_adc_gain(self):
        print "adac250_adc_gain 'channel' 'gain' : Sets ADC gain. 0 for channel A, 1 for channel B. Gain is a value from 0 to 12 (0.5 dB gain per step)."

    def do_adac250_adc_finegain(self, line):
        args = line.split()
        if len(args)>=2:
            channel = int(args[0], 0)
            gain = int(args[1], 0)
            if channel > 1:
                print "Invalid channel ID"
                self.help_adac250_adc_finegain()
            elif gain > 128:
                print "Invalid gain value"
                self.help_adac250_adc_finegain()
            else:
                self.c.adac250_adc_setFineGain(channel, gain)
        else:
            self.help_adac250_adc_finegain()

    def help_adac250_adc_finegain(self):
        print "adac250_adc_finegain 'channel' 'gain' : Sets the ADC fine gain of the specified channel. 0 for channel A, 1 for channel B."
        print "Fine gain is a value from 0 to 128. Applied gain is 0 to 0.134 dB"

    def do_adac250_adc_pedestal(self, line):
        args = line.split()
        if len(args)>=2:
            channel = int(args[0], 0)
            pedestal = int(args[1], 0)
            if channel > 2:
                print "Invalid channel ID"
                self.help_adac250_adc_pedestal()
            elif pedestal > 32:
                print "Invalid offset value value"
                self.help_adac250_adc_pedestal()
            else:
                self.c.adac250_adc_setPedestal(channel, pedestal)
        else:
            self.help_adac250_adc_pedestal()

    def help_adac250_adc_pedestal(self):
        print "adac250_adc_pedestal 'channel' 'offset' : Sets the DC offset of the specified channel. 0 for channel A, 1 for channel B. Value is -32 to 31" 

    def do_adac250_adc_calibrate(self, line):
        if self.c.adac250_adc_calibrate():
            print "Could not calibrate ADC"

    def help_adac250_adc_calibrate(self):
        print "adac250_adc_calibrate : Calibrates the ADC data bus timings."

    def do_adac250_adc_settriggerdelay(self, line):
        args = line.split()
        if len(args)>=1:
            delay = int(args[0], 0)
            if delay > 32:
                print "Invalid delay value"
                self.help_adac250_adc_settriggerdelay()
            elif delay < 1:
                print "Invalid delay value"
                self.help_adac250_adc_settriggerdelay()
            else:
                self.c.adac250_adc_settriggerdelay(delay)
        else:
            self.help_adac250_adc_settriggerdelay()
            
    def help_adac250_adc_settriggerdelay(self):
        print "adac250_adc_settriggerdelay 'delay': Sets a delay for the trigger. Delay is in ADC clock cycles and is from 1 to 32 cycles."

    def do_adac250_adc_getstatus(self, line):
        args = line.split()
        if len(args)>=1:
            channel = int(args[0], 0)
            ret,status = eapi.ADAC250_ADC_getStatus_send(self.c.state, channel)
            if ret<0:
                raise bas_exception(ret)
            if channel == 0:
                if status == eapi.ADAC250_ADC_OVERRANGE:
                    print "Channel A is overrange"
                else: 
                    print "Channel A is normal"
            else:
                if status == eapi.ADAC250_ADC_OVERRANGE:
                    print "Channel B is overrange"
                else: 
                    print "Channel B is normal"
        
    def help_adac250_pll_getStatus(self):
        print "adac250_pll_getstatus 'channel' : Verifies and prints the ADC over-range status"


    def do_adac250_mux_configClockOutput(self, line):
        args = line.split()
        if len(args)>=2:
            outclk = int(args[0], 0)
            inclk = int(args[1], 0)
            if outclk > 4:
                print "Invalid output clock"
                self.help_adac250_mux_configClockOutput()
            elif inclk > 4:
                print "Invalid input clock"
                self.help_adac250_mux_configClockOutput()
            else:
                self.c.adac250_mux_configClockOutput(outclk, inclk)
        else:
            self.help_adac250_mux_configClockOutput()
            
    def help_adac250_mux_configClockOutput(self):
        print "adac250_mux_configClockOutput clock_outputted clock_inputted: Routes the specified input to the specified mux output."


