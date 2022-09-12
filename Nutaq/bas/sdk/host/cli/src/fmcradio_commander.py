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
# @file      fmcradio_commander.py
# 
# @brief     Commander for FMC Radio
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

class fmcradio_commander(bas_commander):
    def __init__(self):
        super(fmcradio_commander, self).__init__()
        self.band = eapi.FMC_RADIO_SDR_LOW_BAND
        self.board_revision = eapi.FMCRADIOREVSDR_B
        
    def fmcradio_powerup(self):
        ret = eapi.fmc_radio_powerup_send(self.state)
        if ret<0:
            raise bas_exception(ret)

    def fmcradio_select(self, nb):
        ret = eapi.fmc_radio_select_send(self.state, nb)
        if ret < 0:
            raise bas_exception(ret)

    def fmcradio_setrevision(self, rev):
        ret = eapi.fmc_radio_setrevision_send(self.state, rev)
        if ret < 0:
            raise bas_exception(ret)
        self.board_revision = rev

    def fmcradio_path_enable(self, dir):
        ret = eapi.fmc_radio_path_enable_send(self.state, dir)
        if ret<0:
            raise bas_exception(ret)

    def fmcradio_path_disable(self, dir):
        ret = eapi.fmc_radio_path_disable_send(self.state, dir)
        if ret<0:
            raise bas_exception(ret)

    # def fmcradio_rx_connect(self, band):
        # ret = eapi.fmc_radio_rx_connect_send(self.state, band)
        # if ret<0:
            # raise bas_exception(ret)

    # def fmcradio_rx_disconnect(self, band):
        # ret = eapi.fmc_radio_rx_disconnect_send(self.state, band)
        # if ret<0:
            # raise bas_exception(ret)
                
    def fmcradio_reset(self):
        ret = eapi.fmc_radio_sdr_reset_send(self.state)
        if ret<0:
            raise bas_exception(ret)
        self.fmcradio_lime_band(self.band)

    def fmcradio_spi_write(self, device, addr, data):
        ret = eapi.fmc_radio_spi_write_send(self.state, device, addr, data)
        if ret<0:
            raise bas_exception(ret)

    def fmcradio_spi_read(self, device, addr):
        ret, data = eapi.fmc_radio_spi_read_send(self.state, device, addr)
        if ret<0:
            raise bas_exception(ret)
        return data

    def fmcradio_pll(self, ref_freq, adac_freq, lime_freq, external):
        param = eapi.cdce62005_param()
        ret = eapi.fmc_radio_pll_calculate(param, ref_freq, adac_freq, lime_freq)
        if ret<0:
            raise bas_exception(ret)
        if external:
            param.secondary_clock = 1
        ret = eapi.fmc_radio_pll_setparam_send(self.state, param)
        if ret<0:
            raise bas_exception(ret)

    def fmcradio_pll_lock(self):
        ret, pllstatus = eapi.fmc_radio_pll_lockstatus_send(self.state)
        if ret<0:
            raise bas_exception(ret)
        return pllstatus

    def fmcradio_clkmux(self, dest, src):
        ret = eapi.fmc_radio_clockmux_set_send(self.state, dest, src)
        if ret<0:
            raise bas_exception(ret)

    def fmcradio_lime_pll(self, dir, ref_freq, carrier_freq):
        param = eapi.lms6002_pll_param()
        ret = eapi.fmc_radio_lime_calculate(param, ref_freq, carrier_freq)
        if ret<0:
            raise bas_exception(ret)
        ret = eapi.fmc_radio_lime_setpllparam_send(self.state, dir, param)

    def fmcradio_lime_pllcalibrate(self, pll):
        ret = eapi.fmc_radio_lime_calibratePLL_send(self.state, pll)
        if ret<0:
            raise bas_exception(ret)

    def fmcradio_lime_reset(self):
        ret = eapi.fmc_radio_lime_reset_send(self.state)
        if ret<0:
            raise bas_exception(ret)

    def fmcradio_lime_band(self, band):
        ret = eapi.fmc_radio_sdr_band_send(self.state, band)
        if ret<0:
            raise bas_exception(ret)
        self.band = band

    def fmcradio_rx_gain(self, lna_gain, rx_gain1, rx_gain2, rx_gain3):
        ret = eapi.fmc_radio_sdr_rx_gain_send(self.state, lna_gain, rx_gain1, rx_gain2, rx_gain3)
        if ret<0:
            raise bas_exception(ret)

    def fmcradio_rxvga_calibrate(self):
        ret = eapi.fmc_radio_rxvga_calibrate_send(self.state)
        if ret<0:
            raise bas_exception(ret)

    def fmcradio_tx_gain(self, tx_vga1_gain, tx_vga2_gain, tx_gain3):
        ret = eapi.fmc_radio_tx_gain_send(self.state, tx_vga1_gain, tx_vga2_gain, tx_gain3)
        if ret<0:
            raise bas_exception(ret)

    def fmcradio_lime_setrxfilter(self, bank):
        ret = eapi.fmc_radio_sdr_setrxfilter_send(self.state, bank)
        if ret<0:
            raise bas_exception(ret)

    def fmcradio_lpf(self, dir, bwt):
        ret = eapi.fmc_radio_lpf_set_send(self.state, dir, bwt)
        if ret<0:
            raise bas_exception(ret)

    def fmcradio_lpf_calibrate(self, direction, ref_freq):
        ret = eapi.fmc_radio_lpf_calibrate_send(self.state, direction, ref_freq)

    def fmcradio_rf_calibrate(self, carrier_freq, acq_freq):
        lo_calib = eapi.lms6002_lo_leakage_calib()
        ssb_calib = eapi.lms6002_ssb_calib()
        rx_dc = eapi.lms6002_rx_dc_calib()
        ret = eapi.fmc_radio_lo_leakage_calibration_send(self.state, carrier_freq, acq_freq, lo_calib);
        if ret<0:
            raise bas_exception(ret)
        ret = eapi.fmc_radio_ssb_calibration_send(self.state,  carrier_freq, acq_freq, ssb_calib);
        if ret<0:
            raise bas_exception(ret)
        ret = eapi.fmc_radio_rx_dc_offset_calibration_send(self.state, rx_dc)
        if ret<0:
            raise bas_exception(ret)
        
    def fmcradio_rxdcoffset_calibrate(self):
        rx_dc = eapi.lms6002_rx_dc_calib()
        ret = eapi.fmc_radio_rx_dc_offset_calibration_send(self.state, rx_dc)
        if ret<0:
            raise bas_exception(ret)

    def fmcradio_loleakage_calibrate(self, carrier_freq, acq_freq):
        lo_calib = eapi.lms6002_lo_leakage_calib()
        ret = eapi.fmc_radio_lo_leakage_calibration_send(self.state, carrier_freq, acq_freq, lo_calib);
        if ret<0:
            raise bas_exception(ret)

    def fmcradio_ssb_calibrate(self, carrier_freq, acq_freq):
        ssb_calib = eapi.lms6002_ssb_calib()
        ret = eapi.fmc_radio_ssb_calibration_send(self.state, carrier_freq, acq_freq, ssb_calib);
        if ret<0:
            raise bas_exception(ret)

    def fmcradio_spi_control(self, control, spi_controller):
        ret = eapi.fmc_radio_spi_control_set_send(self.state, control, spi_controller)
        if ret<0:
            raise bas_exception(ret)

    
