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
# @file      fmcradio_cli.py
# 
# @brief     Command-line interface for FMC Radio
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
from fmcradio_help import *

class fmcradio_cli(fmcradio_help):
    def c_dir(self, l):
        if l.lower() == "tx":
            return eapi.LMS6002_TX
        else:
            return eapi.LMS6002_RX

    def c_band(self, l):
        if l.lower() == "high":
            return eapi.FMC_RADIO_SDR_HIGH_BAND
        else:
            return eapi.FMC_RADIO_SDR_LOW_BAND
        
    def do_fmcradio_reset(self, line):
        self.c.fmcradio_reset()

    def do_fmcradio_powerup(self, line):
        self.c.fmcradio_powerup()

    def do_fmcradio_select(self, line):
        args = line.split()
        if len(args) >= 1:
            nb = int(args[0], 0)
            self.c.fmcradio_select(nb)
        else:
            self.help_fmcradio_select()

    def do_fmcradio_setrevision(self, line):
        args = line.split()
        if len(args) >= 1:
            revision = args[0]
            try:
                rev_obj = getattr(eapi, "FMCRADIOREV" + revision)
                self.c.fmcradio_setrevision(rev_obj)
            except AttributeError:
                print "This revision is unknown."
                return
        else:
            self.help_fmcradio_setrevision()

    def do_fmcradio_path_enable(self, line):
        args = line.split()
        if len(args) >= 1:
            direc = self.c_dir(args[0])
            self.c.fmcradio_path_enable(direc)
        else:
            self.help_fmcradio_path_enable()

    def do_fmcradio_path_disable(self, line):
        args = line.split()
        if len(args) >= 1:
            direc = self.c_dir(args[0])
            self.c.fmcradio_path_disable(direc)
        else:
            self.help_fmcradio_path_disable()

    # def do_fmcradio_rx_connect(self, line):
        # args = line.split()
        # if len(args) >= 1:
            # band = self.c_band(args[0])            
            # self.c.fmcradio_rx_connect(band)
        # else:
            # self.help_fmcradio_rx_connect()

    # def do_fmcradio_rx_disconnect(self, line):
        # args = line.split()
        # if len(args) >= 1:
            # band = self.c_band(args[0])
            # self.c.fmcradio_rx_disconnect(band)
        # else:
            # self.help_fmcradio_rx_disconnect()

    def do_fmcradio_pll(self, line):
        args = line.split()
        if len(args) >= 3:
            ref_freq = int(args[0],0)
            adac_freq = int(args[1],0)
            lime_freq = int(args[2],0)
            try:
                external = args[3]
            except IndexError:
                external = False
            self.c.fmcradio_pll(ref_freq, adac_freq, lime_freq, external)
        else:
            self.help_fmcradio_pll()

    def do_fmcradio_pll_lock(self, line):
        lock = self.c.fmcradio_pll_lock()
        if lock == eapi.CDCE62005_PLL_LOCKED:
            print "PLL is locked."
        else:
            print "PLL is unlocked."

    def do_fmcradio_clkmux(self, line):
        args = line.split()
        if len(args) >= 2:
            destination = args[0]
            source = args[1]
            try:
                destination = getattr(eapi, "FMCRADIO_CLOCKOUT_" + destination)
            except AttributeError:
                print "invalid clock name %s" % destination
                return
            try:
                source = getattr(eapi, "FMCRADIO_CLOCKIN_" + source)
            except AttributeError:
                print "invalid clock name %s" % source
                return
            self.c.fmcradio_clkmux(destination, source)
        else:
            self.help_fmcradio_clkmux()

    def do_fmcradio_lime_pll(self, line):
        args = line.split()
        if len(args) >= 3:
            direction = self.c_dir(args[0])
            ref_freq = int(args[1], 0)
            carrier_freq = int(args[2], 0)
            self.c.fmcradio_lime_pll(direction, ref_freq, carrier_freq)
            self.c.fmcradio_lime_pllcalibrate(direction)
        else:
            self.help_fmcradio_lime_pll()

    def do_fmcradio_lime_reset(self, line):
        self.c.fmcradio_lime_reset()

    def do_fmcradio_rx(self, line):
        parser = self.parser_fmcradio_rx()
        argv = line.split()

        if len(argv) < 2:
            parser.print_help()
            print "Not enough argument"
            return

        gains=[]

        gains.append(argv.pop())
        gains.append(argv.pop())
        gains.reverse()

        try:
            rx_gain2 = int(gains[0], 0)
            rx_gain3 = int(gains[1], 0)
        except ValueError:
            parser.print_help()
            print "gains need to be numeric"
            return

        (options, args) = parser.parse_args(argv)

        lnagain = eapi.LMS6002_LNA_MIDGAIN
        if options.lnamaxgain:
            lnagain = eapi.LMS6002_LNA_MAXGAIN
        if options.lnamidgain:
            lnagain = eapi.LMS6002_LNA_MIDGAIN
        if options.lnabypass:
            lnagain = eapi.LMS6002_LNA_BYPASS

        vgagain = eapi.LMS_VGA1_GAIN19DB
        if options.vgamaxgain:
            vgagain = eapi.LMS_VGA1_GAIN30DB
        if options.vgamidgain:
            vgagain = eapi.LMS_VGA1_GAIN19DB
        if options.vgalowgain:
            vgagain = eapi.LMS_VGA1_GAIN5DB

        self.c.fmcradio_rx_gain(lnagain, vgagain, rx_gain2, rx_gain3)

    def do_fmcradio_band(self, line):
        args = line.split()
        if len(args) >=1:
            band = self.c_band(args[0])
            self.c.fmcradio_lime_band(band)
        else:
            self.help_fmcradio_band()

    def do_fmcradio_rxvga_calibrate(self, line):
        self.c.fmcradio_rxvga_calibrate()

    def do_fmcradio_tx(self, line):
        args = line.split()
        if len(args) >= 3:
            tx_vga1_gain = int(args[0],0)
            tx_vga2_gain = int(args[1],0)
            tx_gain3 = int(args[2],0)

            self.c.fmcradio_tx_gain(tx_vga1_gain, tx_vga2_gain, tx_gain3)
        else:
            self.help_fmcradio_tx()
        
    def do_fmcradio_filter(self, line):
        args = line.split()
        if len(args) >= 1:
            rxfilter = args[0]
            if self.c.board_revision == eapi.FMCRADIOREVSDR_B or self.c.board_revision == eapi.FMCRADIOREVSDR_A:
                filter_prefix = "FMCRADIOSDR_REVB_FILTER_"
            elif self.c.board_revision == eapi.FMCRADIOREVSDR_C  or self.c.board_revision == eapi.FMCRADIOREVSDR_D: 
                filter_prefix = "FMCRADIOSDR_REVC_FILTER_"
            else:
                print "This board revision is not supported. Check if you have set the correct board revision using fmcradio_setrevision command"
                return
            try:
                rxfilter = getattr(eapi, filter_prefix + rxfilter)
                self.c.fmcradio_lime_setrxfilter(rxfilter)
            except AttributeError:
                self.help_fmcradio_filter()
                print "Bad filter name %s" % rxfilter
        else:
            self.help_fmcradio_filter()

    def do_fmcradio_lpf(self, line):
        args = line.split()
        if len(args) >= 2:
            direction = self.c_dir(args[0])
            bwt = args[1]
            try:
                bwt = getattr(eapi, "LMS6002_LPF_" + bwt.replace(".","DOT"))
                self.c.fmcradio_lpf(direction, bwt)
            except AttributeError:
                self.help_fmcradio_lpf()
        else:
            self.help_fmcradio_lpf()

    def do_fmcradio_lpfcalibrate(self, line):
        args = line.split()
        if len(args) >= 2:
            direction = self.c_dir(args[0])
            ref_freq = int(args[1], 0)
            self.c.fmcradio_lpf_calibrate(direction, ref_freq)
        else:
            self.help_fmcradio_lpfcalibrate()

    def do_fmcradio_limespi_write(self, line):
        args = line.split()
        if len(args) >= 2:
            addr = int(args[0], 0)
            data = int(args[1], 0)
            self.c.fmcradio_spi_write(eapi.FMCRADIO_RF, addr, data)
        else:
            self.help_fmcradio_limespi_write()

    def do_fmcradio_limespi_read(self, line):
        args = line.split()
        if len(args) >= 1:
            addr = int(args[0], 0)
            data = self.c.fmcradio_spi_read(eapi.FMCRADIO_RF, addr)
            print hex(data)
        else:
            self.help_fmcradio_limespi_read()

    def do_fmcradio_spi_write(self, line):
        args = line.split()
        if len(args) >= 3:
            dev  = int(args[0], 0)
            addr = int(args[1], 0)
            data = int(args[2], 0)
            self.c.fmcradio_spi_write(dev, addr, data)
        else:
            self.help_fmcradio_spi_write()

    def do_fmcradio_spi_read(self, line):
        args = line.split()
        if len(args) >= 2:
            dev = int(args[0], 0)
            addr = int(args[1], 0)
            data = self.c.fmcradio_spi_read(dev, addr)
            print hex(data)
        else:
            self.help_fmcradio_spi_read()
            
    def do_fmcradio_ref(self, line):
        args = line.split()
        if len(args) >= 1:
            value = int(args[0], 0)
            self.c.fmcradio_spi_write(eapi.FMCRADIO_TCVCXODAC, 0, value)
        else:
            self.help_fmcradio_ref()

    def do_fmcradio_rf_calibrate(self, line):
        args = line.split()
        if len(args) >= 2:
            carrier_freq = int(args[0], 0)
            acq_freq = int(args[1], 0)
            self.c.fmcradio_rf_calibrate(carrier_freq, acq_freq)
        else:
            self.help_fmcradio_rf_calibrate()

    def do_fmcradio_rxdcoffset_calibrate(self, line):
        self.c.fmcradio_rxdcoffset_calibrate()

    def do_fmcradio_loleakage_calibrate(self, line):
        args = line.split()
        if len(args) >= 2:
            carrier_freq = int(args[0], 0)
            acq_freq = int(args[1], 0)
            self.c.fmcradio_loleakage_calibrate(carrier_freq, acq_freq)
        else:
            self.help_fmcradio_loleakage_calibrate()

    def do_fmcradio_ssb_calibrate(self, line):
        args = line.split()
        if len(args) >= 2:
            carrier_freq = int(args[0], 0)
            acq_freq = int(args[1], 0)
            self.c.fmcradio_ssb_calibrate(carrier_freq, acq_freq)
        else:
            self.help_fmcradio_ssb_calibrate()

    def do_fmcradio_spi_control(self, line):
        args = line.split()
        if len(args) >= 2:
            control = int(args[0], 0)
            spi_controller = int(args[1], 0)
            self.c.fmcradio_spi_control(control, spi_controller)
        else:
            self.help_fmcradio_spi_control()
