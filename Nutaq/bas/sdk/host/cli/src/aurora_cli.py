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
# @file      aurora_cli.py
# 
# @brief     Command-line interface for Aurora
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

class aurora_cli:

    def do_aurora_resetcore(self, line):
        args = line.split()
        if len(args) >= 1:
            nb = int(args[0], 0)
            self.c.aurora_resetcore(nb)
        else:
            self.help_aurora_resetcore()
    def help_aurora_resetcore(self):
        print "aurora_resetcore [nb] : Reset Aurora FPGA core. 'nb' is the core ID {1,2,...}."

    def do_aurora_getchannelstatus(self, line):
        args = line.split()
        if len(args) >= 1:
            nb = int(args[0], 0)
            self.c.aurora_getchannelstatus(nb)
        else:
            self.help_aurora_getchannelstatus()
    def help_aurora_getchannelstatus(self):
        print "aurora_getchannelstatus [nb] : Get channel status (up or down). 'nb' is the core ID {1,2,...}."
    
    def do_aurora_resetrxfifo(self, line):
        args = line.split()
        if len(args) >= 1:
            nb = int(args[0], 0)
            self.c.aurora_resetrxfifo(nb)
        else:
            self.help_aurora_resetrxfifo()
    def help_aurora_resetrxfifo(self):
        print "aurora_resetrxfifo [nb] : Reset Aurora RX FIFO content. 'nb' is the core ID {1,2,...}."

    def do_aurora_resettxfifo(self, line):
        args = line.split()
        if len(args) >= 1:
            nb = int(args[0], 0)
            self.c.aurora_resettxfifo(nb)
        else:
            self.help_aurora_resettxfifo()
    def help_aurora_resettxfifo(self):
        print "aurora_resettxfifo [nb] : Reset Aurora TX FIFO content. 'nb' is the core ID {1,2,...}."

    def do_aurora_getrxdatacount(self, line):
        args = line.split()
        if len(args) >= 1:
            nb = int(args[0], 0)
            self.c.aurora_getrxdatacount(nb)
        else:
            self.help_aurora_getrxdatacount()
    def help_aurora_getrxdatacount(self):
        print "aurora_getrxdatacount [nb] : Display the number of bytes received by the RX channel in bytes. 'nb' is the core ID {1,2,...}."

    def do_aurora_gettxdatacount(self, line):
        args = line.split()
        if len(args) >= 1:
            nb = int(args[0], 0)
            self.c.aurora_gettxdatacount(nb)
        else:
            self.help_aurora_gettxdatacount()
    def help_aurora_gettxdatacount(self):
        print "aurora_gettxdatacount [nb] : Display the number of bytes sent by the TX channel in bytes. 'nb' is the core ID {1,2,...}."

    def do_aurora_getrxdatarate(self, line):
        args = line.split()
        if len(args) >= 1:
            nb = int(args[0], 0)
            self.c.aurora_getrxdatarate(nb)
        else:
            self.help_aurora_getrxdatarate()
    def help_aurora_getrxdatarate(self):
        print "aurora_getrxdatarate [nb] : Display the number of bytes received by the RX channel in bytes in the last seconds. 'nb' is the core ID {1,2,...}."

    def do_aurora_gettxdatarate(self, line):
        args = line.split()
        if len(args) >= 1:
            nb = int(args[0], 0)
            self.c.aurora_gettxdatarate(nb)
        else:
            self.help_aurora_gettxdatarate()
    def help_aurora_gettxdatarate(self):
        print "aurora_gettxdatarate [nb] : Display the number of bytes sent by the TX channel in bytes in the last seconds. 'nb' is the core ID {1,2,...}."

    def do_aurora_setgtxtxparam(self, line):
        args = line.split()
        if len(args) >= 4:
            nb              = int(args[0], 0)
            TxDiffCtrl      = int(args[1], 0)
            TxPostEmphasis  = int(args[2], 0)
            TxPreEmphasis   = int(args[3], 0)
            self.c.aurora_setgtxtxparam(nb,TxDiffCtrl,TxPostEmphasis,TxPreEmphasis)
        else:
            self.help_aurora_setgtxtxparam()
    def help_aurora_setgtxtxparam(self):
        print "aurora_setgtxtxparam [nb TxDiffCtrl TxPostEmphasis TxPreEmphasis] : Apply the given TX parameters to the Multi-Gigabit Transceiver (MGT) components. 'nb' is the core ID {1,2,...}. 'TxDiffCtrl', 'TxPostEmphasis' and 'TxPreEmphasis' are the values directly applied to the MGT components."

    def do_aurora_getgtxtxparam(self, line):
        args = line.split()
        if len(args) >= 1:
            nb              = int(args[0], 0)
            self.c.aurora_getgtxtxparam(nb)
        else:
            self.help_aurora_getgtxtxparam()
    def help_aurora_getgtxtxparam(self):
        print "aurora_getgtxtxparam [nb] : Display the current Multi-Gigabit Transceiver (MGT) component TX parameters. 'nb' is the core ID {1,2,...}."

    def do_aurora_setgtxrxparam(self, line):
        args = line.split()
        if len(args) >= 3:
            nb              = int(args[0], 0)
            RxEqMix         = int(args[1], 0)
            DfeTap1         = int(args[2], 0)
            self.c.aurora_setgtxrxparam(nb,RxEqMix,DfeTap1)
        else:
            self.help_aurora_setgtxrxparam()
    def help_aurora_setgtxrxparam(self):
        print "aurora_setgtxrxparam [nb RxEqMix DfeTap1] : Apply the given RX parameters to the Multi-Gigabit Transceiver (MGT) components. 'nb' is the core ID {1,2,...}. 'RxEqMix' and 'DfeTap1' are the values directly applied to the MGT components."

    def do_aurora_getgtxrxparam(self, line):
        args = line.split()
        if len(args) >= 1:
            nb              = int(args[0], 0)
            self.c.aurora_getgtxrxparam(nb)
        else:
            self.help_aurora_getgtxrxparam()
    def help_aurora_getgtxrxparam(self):
        print "aurora_getgtxrxparam [nb] : Display the current Multi-Gigabit Transceiver (MGT) component RX parameters. 'nb' is the core ID {1,2,...}."
    
    def do_aurora_getdfeeyedacmon(self, line):
        args = line.split()
        if len(args) >= 1:
            nb              = int(args[0], 0)
            self.c.aurora_getdfeeyedacmon(nb)
        else:
            self.help_aurora_getdfeeyedacmon()
    def help_aurora_getdfeeyedacmon(self):
        print "aurora_getdfeeyedacmon [nb] : Display the 'Averaged Vertical Eye Height' of the Multi-Gigabit Transceiver (MGT) components. 'nb' is the core ID {1,2,...}."
