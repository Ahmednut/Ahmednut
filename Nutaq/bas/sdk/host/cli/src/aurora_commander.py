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
# @file      aurora_commander.py
#
# @brief     Commander for Aurora
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

class aurora_commander(bas_commander):
    def __init__(self):
        super(aurora_commander, self).__init__()

    def aurora_resetcore(self,nb):
        ret = eapi.Aurora_ResetCore_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)

    def aurora_getchannelstatus(self,nb):
        ret, status = eapi.Aurora_GetChannelStatus_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)
        if status == 1:
            print "Up"
        else:
            print "Down"

    def aurora_resetrxfifo(self,nb):
        ret = eapi.Aurora_ResetRxFifo_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)

    def aurora_resettxfifo(self,nb):
        ret = eapi.Aurora_ResetTxFifo_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)

    def aurora_getrxdatacount(self,nb):
        ret, value = eapi.Aurora_GetRxDataCount_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)
        print value," Bytes"

    def aurora_gettxdatacount(self,nb):
        ret, value = eapi.Aurora_GetTxDataCount_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)
        print value," Bytes"

    def aurora_getrxdatarate(self,nb):
        ret, value = eapi.Aurora_GetRxDataRate_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)
        print value," Bytes/s"

    def aurora_gettxdatarate(self,nb):
        ret, value = eapi.Aurora_GetTxDataRate_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)
        print value," Bytes/s"

    def aurora_setgtxtxparam(self,nb,TxDiffCtrl,TxPostEmphasis,TxPreEmphasis):
        ret = eapi.Aurora_SetGtxTxParam_send(self.state,nb,TxDiffCtrl,TxPostEmphasis,TxPreEmphasis)
        if ret<0:
            raise bas_exception(ret)

    def aurora_getgtxtxparam(self,nb):
        ret,TxDiffCtrl,TxPostEmphasis,TxPreEmphasis = eapi.Aurora_GetGtxTxParam_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)
        print " TxDiffCtrl     : ",TxDiffCtrl
        print " TxPostEmphasis : ",TxPostEmphasis
        print " TxPreEmphasis  : ",TxPreEmphasis

    def aurora_setgtxrxparam(self,nb,RxEqMix,DfeTap1):
        ret = eapi.Aurora_SetGtxRxParam_send(self.state,nb,RxEqMix,DfeTap1)
        if ret<0:
            raise bas_exception(ret)

    def aurora_getgtxrxparam(self,nb):
        ret,RxEqMix,DfeTap1 = eapi.Aurora_GetGtxRxParam_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)
        print " RxEqMix        : ",RxEqMix
        print " DfeTap1        : ",DfeTap1

    def aurora_getdfeeyedacmon(self,nb):
        ret,pLane0,pLane1,pLane2,pLane3 = eapi.Aurora_GetDfeEyeDacMon_send(self.state,nb)
        if ret<0:
            raise bas_exception(ret)
        print " pLane0         : ",pLane0
        print " pLane1         : ",pLane1
        print " pLane2         : ",pLane2
        print " pLane3         : ",pLane3

