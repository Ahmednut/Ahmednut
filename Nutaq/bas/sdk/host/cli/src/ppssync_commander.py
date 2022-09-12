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
# @file      ppssync_commander.py
# 
# @brief     Commander for PPS Sync
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
import time

def to_int32(x):
    if(x & 0x80000000):
        x = x -0x100000000
    return x
            
class ppssync_commander(bas_commander):
    def __init__(self):
        super(ppssync_commander, self).__init__()

    def ppssync_presence(self):
        ret, ppsid, ppsver = eapi.PPSSync_Presence_send(self.state)
        if ret<0:
            raise bas_exception(ret)
        print "Detected PPS-SYNC core id: ", hex(ppsid), "version:", hex(ppsver)               

    def ppssync_ref_dac_init(self, DacVal):
        ret = eapi.PPSSync_DacInit_send(self.state, DacVal)
        if ret<0:
            raise bas_exception(ret)
        
    def ppssync_configure(self, DacMin, DacMax, PPSFreq):
        ret = eapi.PPSSync_Configure_send(self.state, DacMin, DacMax, PPSFreq)
        if ret<0:
            raise bas_exception(ret)

    def ppssync_start(self, freq, time, kp, ki):
        ret = eapi.PPSSync_Start_send(self.state, freq, time, kp, ki)
        if ret<0:
            raise bas_exception(ret)

    def ppssync_stop(self):
        ret = eapi.PPSSync_Stop_send(self.state)
        if ret<0:
            raise bas_exception(ret)

    def ppssync_save_ref_dac(self):
        ret = eapi.PPSSync_SaveRefDacVal_send(self.state)
        if ret<0:
            raise bas_exception(ret)

    def ppssync_read_ref_dac(self):
        ret, ref_dac_val = eapi.PPSSync_ReadRefDacVal_send(self.state)
        if ret<0:
            raise bas_exception(ret)
        print "Stored ref. DAC info:", ref_dac_val
        
    def ppssync_save_pi_profile(self):
        ret = eapi.PPSSync_SavePIProfile_send(self.state)
        if ret<0:
            raise bas_exception(ret)

    def ppssync_read_pi_profile(self):
        ret, kp, ki = eapi.PPSSync_ReadPIProfile_send(self.state)
        if ret<0:
            raise bas_exception(ret)
        print "Stored PI info:"        
        print "            kp: ", "{:3.4f}".format(kp)        
        print "            ki: ", "{:3.4f}".format(ki)        
        
    def ppssync_get_info(self):
        Info = eapi.PPSSync_Info_t()
        ret = eapi.PPSSync_GetInfo_send(self.state, Info)
        if ret<0:
            raise bas_exception(ret)
        #print "Raw:", ":".join("{0:x}".format(ord(c)) for c in pps_info)
        print "PPS-SYNC info:"
        print "  Target freq: ", Info.TargetFreq
        print "         Tick: ", Info.TickPerPPS
        print "          CTE: ", "{:3.3f}".format(Info.fCTE)
        print "          DAC: ", "{:1.6f}".format(Info.fDACValue)
        print "       Locked: ", Info.Lock
        print "          PPB: ", "{:3.3f}".format(Info.fPPB)
        
        
    def ppssync_wait_vcxo_locked(self):
        status = 0
        timeout = 150
        print "Waiting for VCXO locked to GPS"
        
        while((status == 0) and (timeout > 0)):
            time.sleep(2)
            ret, pps_info = eapi.PPSSync_GetInfo_send(self.state)
            if ret<0:
                raise bas_exception(ret)
            status = ((ord(pps_info[19])<<24) + (ord(pps_info[ 18])<<16) + (ord(pps_info[ 17])<<8) + ord(pps_info[ 16]))
            timeout -= 1

        if (timeout == 0):
            print "Waiting for VCXO lock timeout. Please check HW"
            raise bas_exception(ret)
        
        if status:
            print "VCXO LOCKED"
        else:
            print "VCXO UNLOCKED"
