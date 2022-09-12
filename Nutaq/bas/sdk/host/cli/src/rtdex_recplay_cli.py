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
# @file      rtdex_recplay_cli.py
# 
# @brief     Command-line interface for RTDEx Record Playback
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
from bas_exception import *

class rtdex_recplay_cli:
        
    def do_PlaybackData(self, line):
        args = line.split()
        if len(args) == 4:
            #Get parameters
            tmp_trigger_source = args[0]           
            tmp_playback_mode = args[1]
            playback_size = int(args[2], 0)
            start_address = int(args[3], 0)
            
            #Validate trigger_source
            if (tmp_trigger_source.upper() == "SOFT"):
                trigger_source = 1
            elif (tmp_trigger_source.upper() == "EXT"):
                trigger_source = 0
            else:
                print "Invalid trigger_source %s" % tmp_trigger_source
                self.help_PlaybackData()
                return
            
            #Validate playback_mode
            if (tmp_playback_mode.upper() == "SINGLE"):
                playback_mode = 0
            elif (tmp_playback_mode.upper() == "CONTINUOUS"):
                playback_mode = 1
            else:
                print "Invalid playback_mode %s" % tmp_playback_mode
                self.help_PlaybackData()
                return            
            
            #Call function
            ret = eapi.PlaybackData(self.c.state, trigger_source, playback_mode, playback_size, start_address)
            if ret<0:
                raise bas_exception(ret)
        else:
            self.help_PlaybackData()
            
    def help_PlaybackData(self):
        print "PlaybackData 'trigger_source' 'playback_mode' 'playback_size' 'start_address': Playback data from RAM with specified parameters : "
        print "  -'trigger_source'     : 'SOFT' or 'EXT'"
        print "  -'playback_mode'      : 'SINGLE' or 'CONTINUOUS'"
        print "  -'playback_size'      : Size of playback in bytes"
        print "  -'start_address'      : Address to start playback in RAM in bytes"
        
    def do_PlaybackStop(self, line):            
        #Call function
        ret = eapi.PlaybackStop(self.c.state)
        if ret<0:
            raise bas_exception(ret)
            
    def help_PlaybackStop(self):
        print "help_PlaybackStop : Stop the current playback from RAM"
        
    def do_RecordData(self, line):
        args = line.split()
        if len(args) == 4:
            #Get parameters
            tmp_trigger_source = args[0]           
            record_size = int(args[1], 0)
            start_address = int(args[2], 0)
            trigger_delay = int(args[3], 0)
            
            #Validate trigger_source
            if (tmp_trigger_source.upper() == "SOFT"):
                trigger_source = 1
            elif (tmp_trigger_source.upper() == "EXT"):
                trigger_source = 0
            else:
                print "Invalid trigger_source %s" % tmp_trigger_source
                self.help_record_to_ram()
                return    
            
            #Call function
            ret = eapi.RecordData(self.c.state, trigger_source, record_size, start_address, trigger_delay )
            if ret<0:
                raise bas_exception(ret)
        else:
            self.help_RecordData()
            
    def help_RecordData(self):
        print "RecordData 'trigger_source' 'record_size' 'start_address' 'trigger_delay' : Record data to RAM with specified parameters : "
        print "  -'trigger_source'     : 'SOFT' or 'EXT'"
        print "  -'record_size'        : Size of record in bytes"
        print "  -'start_address'      : Address to start record in RAM in bytes"
        print "  -'trigger_delay'      : Trigger delay in chunk of 64 bytes"
        
    def do_LoadDataToPlaybackFromFile(self, line):
        args = line.split()
        if len(args) == 4:
            #Get parameters
            rtdex_channel = int(args[0], 0)         
            frame_size = int(args[1], 0)
            start_address = int(args[2], 0)
            filename = args[3]
                        
            #Call function
            ret = eapi.LoadDataToPlaybackFromFile(self.c.state, rtdex_channel, frame_size, start_address, filename )
            if ret<0:
                raise bas_exception(ret)
        else:
            self.help_LoadDataToPlaybackFromFile()
            
    def help_LoadDataToPlaybackFromFile(self):
        print "LoadDataToPlaybackFromFile 'rtdex_channel' 'frame_size' 'start_address' 'filename' : Transfer a file into the RAM with specified parameters : "
        print "  -'rtdex_channel'     : RTDEx channel used in FPGA for the transfer"
        print "  -'frame_size'        : Frame size for each RTDEx transfer in bytes."
        print "    Suggestion :"
        print "       - 1024  : Jumbo frames are not available ; Ethernet"
        print "       - 8192  : Jumbo frames are available     ; Gigabit Ethernet"
        print "       - 49152 : Linux only                     ; PCI Express"
        print "  -'start_address'     : Address to start record in the RAM in bytes"
        print "  -'filename'          : Name of the file to transfer in the RAM"
        
    def do_RetrieveRecordedDataToFile(self, line):
        args = line.split()
        if len(args) == 6:
            #Get parameters
            rtdex_channel = int(args[0], 0)         
            frame_size = int(args[1], 0)
            record_size = int(args[2], 0)
            start_address = args[3]
            record_timeout = int(args[4], 0)
            filename = args[5]
                        
            #Call function
            if (start_address == 'trigger'):
                ret = eapi.RetrieveRecordedDataFromTriggerToFile(self.c.state, rtdex_channel, frame_size, record_size, record_timeout, filename )
            else:
                ret = eapi.RetrieveRecordedDataToFile(self.c.state, rtdex_channel, frame_size, record_size, int(start_address,0), record_timeout, filename )

            if ret<0:
                raise bas_exception(ret)
        else:
            self.help_RetrieveRecordedDataToFile()
            
    def help_RetrieveRecordedDataToFile(self):
        print "RetrieveRecordedDataToFile 'rtdex_channel' 'frame_size' 'record_size' 'start_address' 'record_timeout' 'filename' : Transfer from RAM into a file with specified parameters : "
        print "  -'rtdex_channel'     : RTDEx channel used in FPGA for the transfer"
        print "  -'frame_size'        : Frame size for each RTDEx transfer in bytes. "
        print "    Suggestion :"
        print "       - 1024  : Jumbo frames are not available ; Ethernet"
        print "       - 8192  : Jumbo frames are available     ; Gigabit Ethernet"
        print "       - 49152 : Linux only                     ; PCI Express"
        print "  -'record_size'       : Size of record in bytes"
        print "  -'start_address'     : Address to start to get the data from the RAM in bytes"
        print "  -'record_timeout'    : Maximum Record time waiting for transfer in milliseconds"
        print "  -'filename'          : Name of the file to get the data from the RAM"
        

    