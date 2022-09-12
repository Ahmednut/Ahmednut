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
# @file      fmcradio_help.py
# 
# @brief     Help information for FMC Radio
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
from cli_parser import OptionParser_cli

class fmcradio_help:
    def help_fmcradio_reset(self):
        print "fmcradio_reset : Reset Radio420X."

    def help_fmcradio_select(self):
        print "fmcradio_select 'nb' : Choose which Radio420 board will be controlled by the next commands. 'nb' starts at 1."

    def help_fmcradio_setrevision(self):
        print "fmcradio_setrevision 'revision' : Choose a board revision from the following list"
        attributes = dir(eapi)
        revisions = []
        for attribute in attributes:
            if attribute.find("FMCRADIOREV") == 0:
                attribute = attribute.replace("FMCRADIOREV","")
                revisions.append(attribute)
        print "REVISIONS"
        print "========="
        for revision in revisions:
            print revision

    def help_fmcradio_path_enable(self):
        print "fmcradio_path_enable 'path' : enable RF path on the card. You may disable RF path to reduce ambient noise. Path is tx or rx."

    def help_fmcradio_path_disable(self):
        print "fmcradio_path_disable 'path' : Disable RF path on the card. You may disable RF path to reduce ambient noise. Path is tx or rx."

    # def help_fmcradio_rx_connect(self):
        # print "fmcradio_rx_connect 'band' : Connect the RX signal from Antenna to the Radio with the specified bandwidth. Band is 'high' (1500 to 3000 MHz) or 'low' (300 to 1500 MHz)."

    # def help_fmcradio_rx_disconnect(self):
        # print "fmcradio_rx_disconnect 'band' : Disconnect the RX signal from Antenna to the Radio with the specified bandwidth. Band is 'high' (1500 to 3000 MHz) or 'low' (300 to 1500 MHz)."

    def help_fmcradio_pll(self):
        print "fmcradio_pll 'reference frequency' 'adac frequency' 'lime frequency' ['external'] : Configure the onboard PLL. The onboard crystal has a frequency of 10000000 Hz. adac frequency is the frequency at which the DAC and ADC operate. Lime frequency is the frequency that is used as a reference frequency for Lime Microsystem RF chip. Generally, this value should be 30000000 Hz. If 'external' equals true, a external reference (Ref In connector) will be used."

    def help_fmcradio_band(self):
        print "fmcradio_band 'band' : Switch RF band. Band is 'high' (1500 to 3000 MHz) or 'low' (300 to 1500 MHz)."

    def help_fmcradio_powerup(self):
        print "fmcradio_powerup : Power up Radio420X."

    def help_fmcradio_pll_lock(self):
        print "fmcradio_pll_lock : Print current lock status of CDCE62005 PLL"

    def help_fmcradio_clkmux(self):
        print "fmcradio_clkmux 'destination' 'source' : Configure clock mux so that the source clock is connected to the destination clock."
        print """
DESTINATION  #  SOURCE
#########################
PLLIN2       #  PLLCLKOUT
NC           #  EXTCLK
FMCCLK0      #  FMCCLK2
FMCCLK1      #  FMCCLK3
#########################
"""

    def help_fmcradio_lime_pll(self):
        print "fmcradio_lime_pll 'direction' 'reference frequency' 'carrier frequency' : Configure the PLL of the Lime Microsystem RF chip. Direction is rx or tx. Reference frequency is the frequency of the clock feeded by the CDCE62005 PLL. Generally this value should be 30000000 Hz. Carrier frequency is the radio mixing frequency. Hint: use a carrier frequency that is an integer multiple of the reference frequency to get better phase noise performance."

    def parser_fmcradio_rx(self):
        usage="%prog [options] 'rx_gain2' 'rx_gain3' : Configure Rx path band and gain. All gain are in dB. rx_gain2 need to be between 0 and 60dB. Using value above 30dB for rx_gain2 is not recommended. Value for rx_gain3 needs to be between -13 and 18 dB. LNA is not used for high band."
        parser = OptionParser_cli(usage, prog="fmcradio_rx")
        parser.add_option("--lh", "--lnamaxgain", action="store_true", dest="lnamaxgain", default=False, help="Set Low Noise Amplifier gain to maximum")
        parser.add_option("--lm", "--lnamidgain", action="store_true", dest="lnamidgain", default=False, help="Set Low Noise Amplifier gain to medium (default)")
        parser.add_option("--lb", "--lnabypass", action="store_true", dest="lnabypass", default=False, help="Bypass Low Noise Amplifier")
        parser.add_option("--vh", "--vgamaxgain", action="store_true", dest="vgamaxgain", default=False, help="Set VGA gain to 30 dB.")
        parser.add_option("--vm", "--vgamidgain", action="store_true", dest="vgamidgain", default=False, help="Set VGA gain to 19 dB (default).")
        parser.add_option("--vl", "--vgalowgain", action="store_true", dest="vgalowgain", default=False, help="Set VGA gain to 5 dB.")
        return parser

    def help_fmcradio_rx(self):
        parser = self.parser_fmcradio_rx()
        parser.print_help()

    def help_fmcradio_filter(self):
        print "fmcradio_filter 'filter name' : configure Rx path RF filter"
        print "==========================================================="
        print ""
        print "          Radio420X Revision C          "
        print "========================================"
        print "Low Band Filters       High Band Filters"
        print "################       #################"
        print "    BYPASS                  BYPASS      "
        print "    751MHZ                  1748MHZ     "
        print "    782MHZ                  1843MHZ     "
        print "    837MHZ                  1880MHZ     "
        print "    882MHZ                  1950MHZ     "
        print "    898MHZ                  1960MHZ     "
        print "    915MHZ                  2140MHZ     "
        print "    943MHZ                  2495MHZ     "
        print ""
        print "          Radio420X Revision B          "
        print "========================================"
        print "Low Band Filters       High Band Filters"
        print "################       #################"
        print "     NONE                    NONE       "
        print "    837MHZ                  1880MHZ     "
        print "    882MHZ                  1950MHZ     "
        print "    943MHZ                  1960MHZ     "
        print "   TESTPOINT                2140MHZ     "
        print "                           TESTPOINT    "
        
    def help_fmcradio_lpf(self):
        attributes = dir(eapi)
        filters = []
        for attribute in attributes:
            if attribute.find("LMS6002_LPF_") == 0:
                attribute = attribute.replace("LMS6002_LPF_","").replace("DOT", ".")
                filters.append(attribute)

        print "fmcradio_lpf 'direction' 'low pass filter name' : configure the low pass filter within the Lime Microsystem chip. Direction is 'tx' or 'rx'."
        print "FILTERS"
        print "#######"
        for f in filters:
            print f
        
    def help_fmcradio_lime_reset(self):
        print "fmcradio_lime_reset : Reset Lime Microsystem LMS6002 chip."

    def help_fmcradio_lpfcalibrate(self):
        print "fmcradio_lpfcalibrate 'direction (rx or tx)''reference frequency' : Calibrate LMS6002 Low Pass Filter. Reference frequency is the reference frequency feeded to the LMS6002 from the CDCE62005 PLL."

    def help_fmcradio_rxvga_calibrate(self):
        print "fmcradio_rxvga_calibrate : Calibrate DC offset of the RX amplifier"

    def help_fmcradio_tx(self):
        print "fmcradio_tx 'tx_vga1_gain' 'tx_vga2_gain' 'tx_gain3' : Configure the Tx path. All gain are in dB. tx_vga1_gain value need to be between -35 and -4. tx_vga2_gain need to be between 0 and 25. tx_gain3 need to be between -13 and 18."

    def help_fmcradio_limespi_write(self):
        print "fmcradio_limespi_write 'addr' 'data' : Write data to register addr of LMS6002"

    def help_fmcradio_limespi_read(self):
        print "fmcradio_limespi_read 'addr' : Read data from register addr of LMS6002"

    def help_fmcradio_spi_write(self):
        print "fmcradio_spi_write 'device' 'addr' 'data' : Write data to register addr"
        print ""
        print "   Device   #        Definition"
        print "#####################################"
        print "     0      #     LIMEMICRO RF device"
        print "     1      #     TX Gain device"
        print "     2      #     RX Gain device"
        print "     3      #     PLL device"
        print "     4      #     TCVXO device"
        print "#####################################"

    def help_fmcradio_spi_read(self):
        print "fmcradio_spi_read 'device' 'addr' : Read data from register addr"
        print ""
        print "   Device   #        Definition"
        print "#####################################"
        print "     0      #     LIMEMICRO RF device"
        print "     1      #     TX Gain device"
        print "     2      #     RX Gain device"
        print "     3      #     PLL device"
        print "     4      #     TCVXO device"
        print "#####################################"
        
    def help_fmcradio_ref(self):
        print "fmcradio_ref 'value' : Enter value between 0 and 65536 to tune the 10MHz reference crystal"

    def help_fmcradio_rf_calibrate(self):
        print "fmcradio_rf_calibrate 'carrier frequency' 'acquisition frequency' : Do IQ gain and IQ phase calibration. This command will reconfigure most of Radio420X, so you will need to reconfigure the board after running this command to suit your application."

    def help_fmcradio_ssb_calibrate(self):
        print "fmcradio_ssb_calibrate 'carrier frequency' 'acquisition frequency': Do IQ gain and IQ phase calibration. This command will reconfigure most of Radio420X, so you will need to reconfigure the board after running this command to suit your application."

    def help_fmcradio_loleakage_calibrate(self):
        print "fmcradio_loleakage_calibrate 'carrier frequency' : Do IQ gain and IQ phase calibration. This command will reconfigure most of Radio420X, so you will need to reconfigure the board after running this command to suit your application."

    def help_fmcradio_rxdcoffset_calibrate(self):
        print "fmcradio_rxdcoffset_calibrate : Calibrate the DC offset of the RX IQ paths"

    def help_fmcradio_spi_control(self):
        print "fmcradio_spi_control 'FPGA control' 'SPI controller' : Sets who controls the SPI"
        print ""
        print "   FPGA control   #        Definition"
        print "###########################################"
        print "     0            #     Reference Clock"
        print "     1            #     Radio Frequency"
        print "     2            #     RX Gain"
        print "     3            #     TX Gain"
        print "     4            #     PLL/CPLD"
        print "###########################################"
        print ""
        print " SPI controller   #        Definition"
        print "###########################################"
        print "     0            #     Host"
        print "     1            #     FPGA"
        print "###########################################"
        
