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
# @file      mo1000_commander.py
# 
# @brief     Commander for MO1000
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

class mo1000_commander(bas_commander):
    def __init__(self):
        super(mo1000_commander, self).__init__()
        self.Mo1000Pllparam = eapi.Mo1000_stPllParam()
        self.Mo1000PllparamUpd = 0
        
    def mo1000_powerup(self,nb):
        ret = eapi.Mo1000_PowerUp_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)

    def mo1000_reset(self,nb):
        ret = eapi.Mo1000_Reset_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)

    def mo1000_init(self,nb):
        ret = eapi.Mo1000_Init_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)

    def mo1000_writereg(self, nb, device, addr, value):
        ret = eapi.Mo1000_WriteReg_send(self.state, nb, device, addr, value)
        if ret<0:
            raise bas_exception(ret)       

    def mo1000_readreg(self, nb, device, addr):
        ret,value = eapi.Mo1000_ReadReg_send(self.state, nb, device, addr)
        if ret<0:
            raise bas_exception(ret)
        return value

    def mo1000_getstatus(self, nb):
        ret, status, compare = eapi.Mo1000_GetStatus_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return status, compare

    def mo1000_gettemperature(self, nb, mode):
        ret, temp, tdac1, tdac2 = eapi.Mo1000_GetTemperature_send(self.state, nb, mode)
        if ret<0:
            raise bas_exception(ret)
        return temp, tdac1, tdac2

    def mo1000_setclockconfig(self, nb, SrcClk, uFreqSrcClk, uFreqDacClock, MasterClkMode, uFreqMasterClock):
        self.Mo1000PllparamUpd = 0
        ret = eapi.Mo1000_SetClockConfig_send(self.state, nb, SrcClk, uFreqSrcClk, uFreqDacClock, MasterClkMode, uFreqMasterClock)
        if ret<0:
            raise bas_exception(ret)

    def mo1000_writeclockconfig(self, nb):
        ret = eapi.Mo1000_WriteClockConfig_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)

    def mo1000_getpllconfig(self, nb):
        ret  = eapi.Mo1000_GetPllConfig_send(self.state, nb, self.Mo1000Pllparam)
        if ret<0:
            raise bas_exception(ret)
        else:
            self.Mo1000PllparamUpd = 1
            print "Primary reference frequency:", self.Mo1000Pllparam.uPrimRefFreq
            print "Secondary reference frequency:", self.Mo1000Pllparam.uSecRefFreq
            print "Ouput #0 clock reference:", self.Mo1000Pllparam.OutputRef0
            print "Ouput #1 clock reference:", self.Mo1000Pllparam.OutputRef1
            print "Ouput #2 clock reference:", self.Mo1000Pllparam.OutputRef2
            print "Ouput #3 clock reference:", self.Mo1000Pllparam.OutputRef3
            print "Ouput #4 clock reference:", self.Mo1000Pllparam.OutputRef4
            print "Vco frequency:", self.Mo1000Pllparam.uVcoFreq
            print "Primary input divider:", self.Mo1000Pllparam.ucPrimaryInputDivider
            print "Reference divider:", self.Mo1000Pllparam.usReferenceDivider
            print "Input divider:", self.Mo1000Pllparam.usInputDivider
            print "Feedback divider:", self.Mo1000Pllparam.usFeedbackDivider
            print "Bypass divider:", self.Mo1000Pllparam.ucBypassDivider
            print "Prescale divider:", self.Mo1000Pllparam.ucPrescaleDivider
            print "Output #0 divider:", self.Mo1000Pllparam.aucOutDivider0
            print "Output #1 divider:", self.Mo1000Pllparam.aucOutDivider1
            print "Output #2 divider:", self.Mo1000Pllparam.aucOutDivider2
            print "Output #3 divider:", self.Mo1000Pllparam.aucOutDivider3
            print "Output #4 divider:" , self.Mo1000Pllparam.aucOutDivider4
            print "ChargePump:", self.Mo1000Pllparam.ChargePump
            print "C1:", self.Mo1000Pllparam.C1Value
            print "R2:", self.Mo1000Pllparam.R2Value
            print "C2:", self.Mo1000Pllparam.C2Value
            print "R3:", self.Mo1000Pllparam.R3Value
            print "C3:", self.Mo1000Pllparam.C3Value

    def mo1000_setpllconfig(self, nb, ChargePump, C1Value, R2Value, C2Value, R3Value, C3Value):
        if self.Mo1000PllparamUpd == 0:
            print "You must use 'mo1000_getpllconfig' first before using this function."
        else:
            self.Mo1000Pllparam.ChargePump = ChargePump
            self.Mo1000Pllparam.C1Value = C1Value
            self.Mo1000Pllparam.R2Value = R2Value
            self.Mo1000Pllparam.C2Value = C2Value
            self.Mo1000Pllparam.R3Value = R3Value
            self.Mo1000Pllparam.C3Value = C3Value
            ret = eapi.Mo1000_SetPllConfig_send(self.state, nb, self.Mo1000Pllparam)
            if ret<0:
                raise bas_exception(ret)

    def mo1000_dodaccalibration(self, nb):
        ret = eapi.Mo1000_DoDacCalibration_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)

    def mo1000_getchannelcalibstatus(self, nb):
        ret, uChannelLaneCalib, uChannelFrameCalib, uChannelSyncCalib, uCalibStatus = eapi.Mo1000_GetChannelCalibStatus_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return uChannelLaneCalib, uChannelFrameCalib, uChannelSyncCalib, uCalibStatus

    def mo1000_setdacoutctrl(self, nb, Channel, State):
        ret = eapi.Mo1000_SetDacOutCtrl_send(self.state, nb, Channel, State)
        if ret<0:
            raise bas_exception(ret)       

    def mo1000_setdacparinterpolation(self, nb, Mode):
        ret = eapi.Mo1000_SetDacParInterpolation_send(self.state, nb, Mode)
        if ret<0:
            raise bas_exception(ret)       

    def mo1000_setdacpardcoffset(self, nb, Channel, sOffset):
        ret = eapi.Mo1000_SetDacParDcOffset_send(self.state, nb, Channel, sOffset)
        if ret<0:
            raise bas_exception(ret)       

    def mo1000_setdacpargain(self, nb, Channel, ucGain):
        ret = eapi.Mo1000_SetDacParGain_send(self.state, nb, Channel, ucGain)
        if ret<0:
            raise bas_exception(ret)       

    def mo1000_dodacupdate(self,nb):
        ret = eapi.Mo1000_DoDacUpdate_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)

    def mo1000_setdacparactchannel(self, nb, ActChannel):
        ret = eapi.Mo1000_SetDacParActChannel_send(self.state, nb, ActChannel)
        if ret<0:
            raise bas_exception(ret)       

    def mo1000_getclkmaster(self, nb):
        ret, master = eapi.Mo1000_GetClkMaster_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return master

    def mo1000_setdacparisinc(self, nb, DacPair, C0Real, C1Real, C2Real, C3Real, C4Real, C0Img, C1Img, C2Img, C3Img, C4Img):
        ret = eapi.Mo1000_SetDacParISinc_send(self.state, nb, DacPair, C0Real, C1Real, C2Real, C3Real, C4Real, C0Img, C1Img, C2Img, C3Img, C4Img)
        if ret<0:
            raise bas_exception(ret)       

    def mo1000_setdacparisincctrl(self, nb, Group, State):
        ret = eapi.Mo1000_SetDacParISincCtrl_send(self.state, nb, Group, State)
        if ret<0:
            raise bas_exception(ret)       

    def mo1000_setdacparfinemod(self, nb, Group, Ftw, NcoPhaseOffset, SideBand):
        ret = eapi.Mo1000_SetDacParFineMod_send(self.state, nb, Group, Ftw, NcoPhaseOffset, SideBand)
        if ret<0:
            raise bas_exception(ret)       

    def mo1000_setdacparfinemodctrl(self, nb, Group, State):
        ret = eapi.Mo1000_SetDacParFineModCtrl_send(self.state, nb, Group, State)
        if ret<0:
            raise bas_exception(ret)       

    def mo1000_setdacpardataformat(self, nb, Group, Format):
        ret = eapi.Mo1000_SetDacParDataFormat_send(self.state, nb, Group, Format)
        if ret<0:
            raise bas_exception(ret)       

    def mo1000_setdacparquadphase(self, nb, DacPair, RealPhase, ImaginaryPhase):
        ret = eapi.Mo1000_SetDacParQuadPhase_send(self.state, nb, DacPair, RealPhase, ImaginaryPhase)
        if ret<0:
            raise bas_exception(ret)       

    def mo1000_getversions(self, nb):
        ret, CoreVersion, DriverVersion = eapi.Mo1000_GetVersions_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return CoreVersion, DriverVersion

    def mo1000_testlocaloscsw(self, nb):
        ret, Result = eapi.Mo1000_TestLocalOscSw_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return Result

    def mo1000_testbridgereset(self, nb):
        ret, Result = eapi.Mo1000_TestBridgeReset_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return Result

    def mo1000_testdacreset(self, nb):
        ret, Result = eapi.Mo1000_TestDacReset_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return Result

    def mo1000_testpllsel(self, nb):
        ret, Result = eapi.Mo1000_TestPllSel_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return Result

    def mo1000_testpllpowdown(self, nb):
        ret, Result = eapi.Mo1000_TestPllPowDown_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return Result

    def mo1000_testpllsync(self, nb):
        ret, Result = eapi.Mo1000_TestPllSync_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return Result

    def mo1000_testdacirq(self, nb):
        ret, Result = eapi.Mo1000_TestDacIrq_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return Result

    def mo1000_testdacclks(self, nb):
        ret, Result = eapi.Mo1000_TestDacClks_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return Result

    def mo1000_testdciframe(self, nb):
        ret, Result = eapi.Mo1000_TestDciFrame_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return Result

    def mo1000_checki2c(self, nb, ScanMax, ScanRaw, NumDevices):
        ret, ScanRes, DetectOk = eapi.Mo1000_CheckI2c_send(self.state, nb, ScanMax, ScanRaw, NumDevices)
        if ret<0:
            raise bas_exception(ret)
        return ScanRes, DetectOk

    def mo1000_getcorefrequency(self, nb):
        ret, CoreRefClock, DacRefClock = eapi.Mo1000_GetCoreFrequency_send(self.state, nb)
        if ret<0:
            raise bas_exception(ret)
        return CoreRefClock, DacRefClock

    def mo1000_settestmode(self, nb, Test, EvenPattern, OddPattern):
        ret = eapi.Mo1000_SetTestMode_send(self.state, nb, Test, EvenPattern, OddPattern)
        if ret<0:
            raise bas_exception(ret)
