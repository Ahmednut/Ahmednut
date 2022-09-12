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
# @file      ppssync_cli.py
# 
# @brief     Command-line interface for PPS Sync
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

class ppssync_cli:

    def do_ppssync_presence(self, line):        
        self.c.ppssync_presence()        
        
    def help_ppssync_presence(self):
        print "ppssync_presence : Verifies the presence of the PPSYNC core and FMC daughter card"

    def do_ppssync_ref_dac_init(self, line):
        args = line.split()
        if len(args) >= 1:
            try:
                DacVal = float(args[0])
                self.c.ppssync_ref_dac_init(DacVal)
            except ValueError:
                print "invalid value"
        else:
            self.help_ppssync_ref_dac_init()

    def help_ppssync_ref_dac_init(self):
        print "ppssync_ref_dac_init 'init DAC value'"
        
    def do_ppssync_configure(self, line):
        args = line.split()
        if len(args) >= 2:
            try:
                DacMin = float(args[0])
                DacMax = float(args[1])
                PPSFreq = int(args[2])
            
                self.c.ppssync_configure(DacMin, DacMax, PPSFreq)
            except ValueError:
                print "invalid value"
        else:
            self.help_ppssync_ref_dac_limit()            

    def help_ppssync_configure(self):
        print "ppssync_configure 'Minimum DAC' 'Maximum DAC' 'PPS Freq'"        

    def do_ppssync_start(self, line):
        args = line.split()
        if len(args) >= 4:
            try:
                freq = int(args[0], 0)
                time = int(args[1], 0)
                kp = float(args[2])
                ki = float(args[3])

                if(freq < 0):
                    print 'ERROR: Must be a positive frequency value!'
                    raise ValueError
                if(time < 1):
                    print 'ERROR: Must be a positive integration time value!'
                    raise ValueError                                  
                self.c.ppssync_start(freq, time, kp, ki)
            except ValueError:
                print "invalid value"
        else:
            self.help_ppssync_start()
            
    def help_ppssync_start(self):
        print "ppssync_start 'frequency' 'integration time' 'proportional gain' 'integral gain'"

    def do_ppssync_stop(self, line):
         self.c.ppssync_stop()
         
    def help_ppssync_stop(self):
        print "ppssync_stop : Stop PPS-SYNC routine"

    def do_ppssync_save_ref_dac(self, line):
        self.c.ppssync_save_ref_dac()
        
    def help_ppssync_save_ref_dac(self):
        print "ppssync_save_ref_dac : Save current DAC value to carrier memory"

    def do_ppssync_read_ref_dac(self, line):
        self.c.ppssync_read_ref_dac()
        
    def help_ppssync_read_ref_dac(self):
        print "ppssync_read_ref_dac : Read ref. DAC value from carrier memory"        

    def do_ppssync_save_pi_profile(self, line):
        self.c.ppssync_save_pi_profile()
        
    def help_ppssync_save_pi_profile(self):
        print "ppssync_save_pi_profile : Save current PI profile to carrier memory"

    def do_ppssync_read_pi_profile(self, line):
        self.c.ppssync_read_pi_profile()
        
    def help_ppssync_read_pi_profile(self):
        print "ppssync_read_pi_profile : Read PI profile from carrier memory"

    def do_ppssync_get_info(self, line):
        self.c.ppssync_get_info()
        
    def help_ppssync_get_info(self):
        print "ppssync_get_info : Get status of PPS-SYNC"

    def do_ppssync_wait_vcxo_locked(self, line):
        self.c.ppssync_wait_vcxo_locked()
        
    def help_ppssync_wait_vcxo_locked(self):
        print "ppssync_wait_vcxo_locked : Wait for VCXO locked to GPS"
