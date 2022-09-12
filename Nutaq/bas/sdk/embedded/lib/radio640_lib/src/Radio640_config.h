/****************************************************************************
 *
 *    ****                              I
 *   ******                            ***
 *   *******                           ****
 *   ********    ****  ****     **** *********    ******* ****    ***********
 *   *********   ****  ****     **** *********  **************  *************
 *   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
 *   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
 *  ****    *********  ****     ****   ****   ****       ****  ****      ****
 *  ****     ********  ****    ****I  ****    *****     *****  ****      ****
 *  ****      ******   ***** ******   *****    ****** *******  ****** *******
 *  ****        ****   ************    ******   *************   *************
 *  ****         ***     ****  ****     ****      *****  ****     *****  ****
 *                                                                       ****
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       Radio640_regs.h
 * @brief      radio640 module related internal registers definitions
 *
 * $Date: 2015/12/07 15:01:22 $
 * $Revision: 1.4 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup RADIO640
 *
 * This file is part of Nutaq's BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq's BAS Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq's BAS Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq's BAS Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq's BAS Software Suite under the GPL requires that your work based on
 * Nutaq's BAS Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq's BAS Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq's BAS Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/

#ifndef RADIO640_CONFIG_H__
#define RADIO640_CONFIG_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "bas_types.h"

const Radio640DevCom_stI2cAddr RADIO640_I2C_ADDR_MAIN_GA00  = {0x50, 0x48, 0x40, 0x41}; ///< GA0=0 GA1=0 main board (fmc bottom)
const Radio640DevCom_stI2cAddr RADIO640_I2C_ADDR_SEC_GA00   = {0x54, 0x4C, 0x45, 0x44}; ///< GA0=0 GA1=0 secondary board (fmc top)
const Radio640DevCom_stI2cAddr RADIO640_I2C_ADDR_MAIN_GA01  = {0x51, 0x49, 0x40, 0x41}; ///< GA0=0 GA1=1 main board (fmc bottom)
const Radio640DevCom_stI2cAddr RADIO640_I2C_ADDR_SEC_GA01   = {0x55, 0x4D, 0x45, 0x44}; ///< GA0=0 GA1=1 secondary board (fmc top)
const Radio640DevCom_stI2cAddr RADIO640_I2C_ADDR_MAIN_GA10  = {0x52, 0x4A, 0x40, 0x41}; ///< GA0=1 GA1=0 main board (fmc bottom)
const Radio640DevCom_stI2cAddr RADIO640_I2C_ADDR_SEC_GA10   = {0x56, 0x4E, 0x45, 0x44}; ///< GA0=1 GA1=0 secondary board (fmc top)
const Radio640DevCom_stI2cAddr RADIO640_I2C_ADDR_MAIN_GA11  = {0x53, 0x4B, 0x40, 0x41}; ///< GA0=1 GA1=1 main board (fmc bottom)
const Radio640DevCom_stI2cAddr RADIO640_I2C_ADDR_SEC_GA11   = {0x57, 0x4F, 0x45, 0x44}; ///< GA0=1 GA1=1 secondary board (fmc top)


const uint16_t RADIO640_HMC7043_INIT_REGS[][2] = 
    //glbl_cfg1_swrst[0:0] = 0x0
    {{0x0, 0x0},

    //glbl_cfg1_sleep[0:0] = 0x0
    //glbl_cfg1_restart[1:1] = 0x0
    //sysr_cfg1_pulsor_req[2:2] = 0x0
    //grpx_cfg1_mute[3:3] = 0x0
    //dist_cfg1_perf_floor[6:6] = 0x1
    //sysr_cfg1_reseed_req[7:7] = 0x0
    {0x1, 0x40},

    //sysr_cfg1_altrat_req[0:0] = 0x0
    //sysr_cfg1_slipN_req[1:1] = 0x0
    {0x2, 0x0},

    //glbl_cfg1_ena_sysr[2:2] = 0x0
    //glbl_cfg1_ena_sysri[5:5] = 0x0
    {0x3, 0x0},

    //glbl_cfg7_ena_clkgr[6:0] = 0x29
    {0x4, 0x29},

    //glbl_cfg4_ena_rpath[3:0] = 0xF
    //dist_cfg1_refbuf0_as_rfsync[4:4] = 0x0
    //dist_cfg1_refbuf1_as_extvco[5:5] = 0x0
    {0x5, 0xF},

    //glbl_cfg1_clear_alarms[0:0] = 0x0
    {0x6, 0x0},

    //glbl_cfg1_7043_sdata_hs_en[1:1] = 0x0
    //glbl_cfg2_7043_gpio1[3:2] = 0x0
    //glbl_cfg4_spare[7:4] = 0x0
    {0x7, 0x0},

    //glbl_cfg8_spare2[7:0] = 0x0
    {0x8, 0x0},

    //glbl_cfg7_digspare[7:1] = 0x0
    {0x9, 0x0},

    //glbl_cfg5_ibuf0[4:0] = 0x0
    {0xA, 0x0},

    //glbl_cfg5_ibuf1[4:0] = 0x7
    {0xB, 0x7},

    //glbl_cfg5_gpi1[4:0] = 0x0
    {0x46, 0x0},

    //glbl_cfg8_gpo1[7:0] = 0x10
    {0x50, 0x10},

    //glbl_cfg2_sdio[1:0] = 0x3
    {0x54, 0x3},

    //sysr_cfg3_pulsor_mode[2:0] = 0x4
    {0x5A, 0x4},

    //sysr_cfg1_synci_invpol[0:0] = 0x0
    //sysr_cfg1_ext_sync_retimemode[2:2] = 0x0
    {0x5B, 0x0},

    //sysr_cfg16_divrat_lsb[7:0] = 0xA
    {0x5C, 0xA},

    //sysr_cfg16_divrat_msb[7:0] = 0x0
    {0x5D, 0x0},

    //dist_cfg1_extvco_islowfreq_sel[0:0] = 0x1
    //dist_cfg1_extvco_div2_sel[1:1] = 0x0
    {0x64, 0x1},

    //clkgrpx_cfg1_alg_dly_lowpwr_sel[0:0] = 0x0
    {0x65, 0x0},

    //alrm_cfg1_sysr_unsyncd_allow[1:1] = 0x0
    //alrm_cfg1_clkgrpx_validph_allow[2:2] = 0x0
    //alrm_cfg1_sync_req_allow[4:4] = 0x0
    {0x71, 0x0},

    //dist_cfg8_eng_ena_force[7:0] = 0x0
    {0x98, 0x0},

    //sysr_cfg8_eng_ena_force[7:0] = 0x0
    {0x99, 0x0},

    //glbl_cfg8_eng_bia_distfloor_lo[7:0] = 0xAA
    {0x9D, 0xAA},

    //glbl_cfg8_eng_bia_distfloor_hi[7:0] = 0xAA
    {0x9E, 0xAA},

    //glbl_cfg8_eng_bia_grpx_lo[7:0] = 0x55
    {0x9F, 0x55},

    //glbl_cfg8_eng_bia_grpx_hi[7:0] = 0x56
    {0xA0, 0x56},

    //glbl_cfg1_eng_disable_gpo_export[0:0] = 0x0
    //glbl_cfg1_eng_smartclkgate[1:1] = 0x1
    //glbl_cfg4_eng_gpivals_when_dis[5:2] = 0x0
    //glbl_cfg1_eng_sleepregs_on_sleep[6:6] = 0x0
    //glbl_cfg1_swrst_clkgr[7:7] = 0x0
    {0xA2, 0x2},

    //glbl_cfg1_tempdiode_txgate_en[0:0] = 0x0
    //glbl_cfg2_proctst_thickpnfet_en[2:1] = 0x0
    //glbl_cfg2_proctst_thinpnfet_en[4:3] = 0x0
    {0xA3, 0x0},

    //glbl_cfg8_eng_spare1[7:0] = 0x0
    {0xA4, 0x0},

    //ldo_cfg1_eng_ldo_trims_en[0:0] = 0x0
    //ldo_cfg1_eng_ldo_xtrim_2p8[1:1] = 0x0
    //ldo_cfg2_eng_ldo_xtrim_1p8[3:2] = 0x0
    {0xAD, 0x0},

    //sysr_cfg4_eng[3:0] = 0x0
    {0xB5, 0x0},

    //dist_cfg1_bia_as_extvcobuf[0:0] = 0x0
    //dist_cfg1_bia_as_rfsyncbuf[1:1] = 0x0
    {0xB6, 0x0},

    //grpx_cfg2_fsm_clkdiv_sel[1:0] = 0x0
    {0xB7, 0x0},

    //glbl_cfg8_gpo_eng_sel[7:0] = 0x0
    {0xB8, 0x0},

    //clkgrp1_div1_cfg1_en[0:0] = 0x1
    //clkgrp1_div1_cfg1_phdelta_mslip[1:1] = 0x1
    //clkgrp1_div1_cfg2_startmode[3:2] = 0x0
    //clkgrp1_div1_cfg1_altdivratmask[4:4] = 0x1
    //clkgrp1_div1_cfg1_slipmask[5:5] = 0x1
    //clkgrp1_div1_cfg1_reseedmask[6:6] = 0x1
    //clkgrp1_div1_cfg1_hi_perf[7:7] = 0x1
    {0xC8, 0xF3},

    //clkgrp1_div1_cfg12_divrat_lsb[7:0] = 0x0
    {0xC9, 0x0},

    //clkgrp1_div1_cfg12_divrat_msb[3:0] = 0x0
    {0xCA, 0x0},

    //clkgrp1_div1_cfg5_fine_delay[4:0] = 0x0
    {0xCB, 0x0},

    //clkgrp1_div1_cfg5_sel_coarse_delay[4:0] = 0x0
    {0xCC, 0x0},

    //clkgrp1_div1_cfg12_mslip_lsb[7:0] = 0x0
    //clkgrp1_div1_cfg12_phdelta_lsb[7:0] = 0x0
    {0xCD, 0x0},

    //clkgrp1_div1_cfg12_mslip_msb[3:0] = 0x0
    //clkgrp1_div1_cfg12_phdelta_msb[3:0] = 0x0
    {0xCE, 0x0},

    //clkgrp1_div1_cfg2_sel_outmux[1:0] = 0x3
    //clkgrp1_div1_cfg1_drvr_sel_testclk[2:2] = 0x0
    //clkgrp1_div1_cfg5_spares[7:3] = 0x0
    {0xCF, 0x3},

    //clkgrp1_div1_cfg5_drvr[4:0] = 0x1B
    //clkgrp1_div1_cfg_outbuf_dyn[5:5] = 0x0
    //clkgrp1_div1_cfg2_mutesel[7:6] = 0x0
    {0xD0, 0x1B},

    //clkgrp1_div2_cfg1_en[0:0] = 0x0
    //clkgrp1_div2_cfg1_phdelta_mslip[1:1] = 0x0
    //clkgrp1_div2_cfg2_startmode[3:2] = 0x3
    //clkgrp1_div2_cfg1_altdivratmask[4:4] = 0x1
    //clkgrp1_div2_cfg1_slipmask[5:5] = 0x1
    //clkgrp1_div2_cfg1_reseedmask[6:6] = 0x1
    //clkgrp1_div2_cfg1_hi_perf[7:7] = 0x0
    {0xD2, 0x7C},

    //clkgrp1_div2_cfg12_divrat_lsb[7:0] = 0x0
    {0xD3, 0x0},

    //clkgrp1_div2_cfg12_divrat_msb[3:0] = 0x1
    {0xD4, 0x1},

    //clkgrp1_div2_cfg5_fine_delay[4:0] = 0x0
    {0xD5, 0x0},

    //clkgrp1_div2_cfg5_sel_coarse_delay[4:0] = 0x0
    {0xD6, 0x0},

    //clkgrp1_div2_cfg12_mslip_lsb[7:0] = 0x0
    //clkgrp1_div2_cfg12_phdelta_lsb[7:0] = 0x0
    {0xD7, 0x0},

    //clkgrp1_div2_cfg12_mslip_msb[3:0] = 0x0
    //clkgrp1_div2_cfg12_phdelta_msb[3:0] = 0x0
    {0xD8, 0x0},

    //clkgrp1_div2_cfg2_sel_outmux[1:0] = 0x3
    //clkgrp1_div2_cfg1_drvr_sel_testclk[2:2] = 0x0
    //clkgrp1_div2_cfg5_spares[7:3] = 0x0
    {0xD9, 0x3},

    //clkgrp1_div2_cfg5_drvr[4:0] = 0x18
    //clkgrp1_div2_cfg_outbuf_dyn[5:5] = 0x1
    //clkgrp1_div2_cfg2_mutesel[7:6] = 0x0
    {0xDA, 0x38},

    //clkgrp2_div1_cfg1_en[0:0] = 0x0
    //clkgrp2_div1_cfg1_phdelta_mslip[1:1] = 0x1
    //clkgrp2_div1_cfg2_startmode[3:2] = 0x0
    //clkgrp2_div1_cfg1_altdivratmask[4:4] = 0x1
    //clkgrp2_div1_cfg1_slipmask[5:5] = 0x1
    //clkgrp2_div1_cfg1_reseedmask[6:6] = 0x1
    //clkgrp2_div1_cfg1_hi_perf[7:7] = 0x0
    {0xDC, 0x72},

    //clkgrp2_div1_cfg12_divrat_lsb[7:0] = 0x8
    {0xDD, 0x8},

    //clkgrp2_div1_cfg12_divrat_msb[3:0] = 0x0
    {0xDE, 0x0},

    //clkgrp2_div1_cfg5_fine_delay[4:0] = 0x0
    {0xDF, 0x0},

    //clkgrp2_div1_cfg5_sel_coarse_delay[4:0] = 0x0
    {0xE0, 0x0},

    //clkgrp2_div1_cfg12_mslip_lsb[7:0] = 0x0
    //clkgrp2_div1_cfg12_phdelta_lsb[7:0] = 0x0
    {0xE1, 0x0},

    //clkgrp2_div1_cfg12_phdelta_msb[3:0] = 0x0
    //clkgrp2_div1_cfg12_mslip_msb[3:0] = 0x0
    {0xE2, 0x0},

    //clkgrp2_div1_cfg2_sel_outmux[1:0] = 0x3
    //clkgrp2_div1_cfg1_drvr_sel_testclk[2:2] = 0x0
    //clkgrp2_div1_cfg5_spares[7:3] = 0x0
    {0xE3, 0x3},

    //clkgrp2_div1_cfg5_drvr[4:0] = 0x18
    //clkgrp2_div1_cfg_outbuf_dyn[5:5] = 0x0
    //clkgrp2_div1_cfg2_mutesel[7:6] = 0x0
    {0xE4, 0x18},

    //clkgrp2_div2_cfg1_en[0:0] = 0x0
    //clkgrp2_div2_cfg1_phdelta_mslip[1:1] = 0x0
    //clkgrp2_div2_cfg2_startmode[3:2] = 0x3
    //clkgrp2_div2_cfg1_altdivratmask[4:4] = 0x0
    //clkgrp2_div2_cfg1_slipmask[5:5] = 0x0
    //clkgrp2_div2_cfg1_reseedmask[6:6] = 0x1
    //clkgrp2_div2_cfg1_hi_perf[7:7] = 0x0
    {0xE6, 0x4C},

    //clkgrp2_div2_cfg12_divrat_lsb[7:0] = 0xA
    {0xE7, 0xA},

    //clkgrp2_div2_cfg12_divrat_msb[3:0] = 0x0
    {0xE8, 0x0},

    //clkgrp2_div2_cfg5_fine_delay[4:0] = 0x0
    {0xE9, 0x0},

    //clkgrp2_div2_cfg5_sel_coarse_delay[4:0] = 0x0
    {0xEA, 0x0},

    //clkgrp2_div2_cfg12_phdelta_lsb[7:0] = 0x0
    //clkgrp2_div2_cfg12_mslip_lsb[7:0] = 0x0
    {0xEB, 0x0},

    //clkgrp2_div2_cfg12_phdelta_msb[3:0] = 0x0
    //clkgrp2_div2_cfg12_mslip_msb[3:0] = 0x0
    {0xEC, 0x0},

    //clkgrp2_div2_cfg2_sel_outmux[1:0] = 0x3
    //clkgrp2_div2_cfg1_drvr_sel_testclk[2:2] = 0x0
    //clkgrp2_div2_cfg5_spares[7:3] = 0x0
    {0xED, 0x3},

    //clkgrp2_div2_cfg5_drvr[4:0] = 0x18
    //clkgrp2_div2_cfg_outbuf_dyn[5:5] = 0x1
    //clkgrp2_div2_cfg2_mutesel[7:6] = 0x0
    {0xEE, 0x38},

    //clkgrp3_div1_cfg1_en[0:0] = 0x0
    //clkgrp3_div1_cfg1_phdelta_mslip[1:1] = 0x1
    //clkgrp3_div1_cfg2_startmode[3:2] = 0x0
    //clkgrp3_div1_cfg1_altdivratmask[4:4] = 0x1
    //clkgrp3_div1_cfg1_slipmask[5:5] = 0x1
    //clkgrp3_div1_cfg1_reseedmask[6:6] = 0x1
    //clkgrp3_div1_cfg1_hi_perf[7:7] = 0x0
    {0xF0, 0x72},

    //clkgrp3_div1_cfg12_divrat_lsb[7:0] = 0x2
    {0xF1, 0x2},

    //clkgrp3_div1_cfg12_divrat_msb[3:0] = 0x0
    {0xF2, 0x0},

    //clkgrp3_div1_cfg5_fine_delay[4:0] = 0x0
    {0xF3, 0x0},

    //clkgrp3_div1_cfg5_sel_coarse_delay[4:0] = 0x0
    {0xF4, 0x0},

    //clkgrp3_div1_cfg12_phdelta_lsb[7:0] = 0x0
    //clkgrp3_div1_cfg12_mslip_lsb[7:0] = 0x0
    {0xF5, 0x0},

    //clkgrp3_div1_cfg12_phdelta_msb[3:0] = 0x0
    //clkgrp3_div1_cfg12_mslip_msb[3:0] = 0x0
    {0xF6, 0x0},

    //clkgrp3_div1_cfg2_sel_outmux[1:0] = 0x3
    //clkgrp3_div1_cfg1_drvr_sel_testclk[2:2] = 0x0
    //clkgrp3_div1_cfg5_spares[7:3] = 0x0
    {0xF7, 0x3},

    //clkgrp3_div1_cfg5_drvr[4:0] = 0x18
    //clkgrp3_div1_cfg_outbuf_dyn[5:5] = 0x0
    //clkgrp3_div1_cfg2_mutesel[7:6] = 0x0
    {0xF8, 0x18},

    //clkgrp3_div2_cfg1_en[0:0] = 0x0
    //clkgrp3_div2_cfg1_phdelta_mslip[1:1] = 0x0
    //clkgrp3_div2_cfg2_startmode[3:2] = 0x3
    //clkgrp3_div2_cfg1_altdivratmask[4:4] = 0x1
    //clkgrp3_div2_cfg1_slipmask[5:5] = 0x1
    //clkgrp3_div2_cfg1_reseedmask[6:6] = 0x1
    //clkgrp3_div2_cfg1_hi_perf[7:7] = 0x0
    {0xFA, 0x7C},

    //clkgrp3_div2_cfg12_divrat_lsb[7:0] = 0x0
    {0xFB, 0x0},

    //clkgrp3_div2_cfg12_divrat_msb[3:0] = 0x1
    {0xFC, 0x1},

    //clkgrp3_div2_cfg5_fine_delay[4:0] = 0x0
    {0xFD, 0x0},

    //clkgrp3_div2_cfg5_sel_coarse_delay[4:0] = 0x0
    {0xFE, 0x0},

    //clkgrp3_div2_cfg12_phdelta_lsb[7:0] = 0x0
    //clkgrp3_div2_cfg12_mslip_lsb[7:0] = 0x0
    {0xFF, 0x0},

    //clkgrp3_div2_cfg12_phdelta_msb[3:0] = 0x0
    //clkgrp3_div2_cfg12_mslip_msb[3:0] = 0x0
    {0x100, 0x0},

    //clkgrp3_div2_cfg2_sel_outmux[1:0] = 0x3
    //clkgrp3_div2_cfg1_drvr_sel_testclk[2:2] = 0x0
    //clkgrp3_div2_cfg5_spares[7:3] = 0x0
    {0x101, 0x3},

    //clkgrp3_div2_cfg5_drvr[4:0] = 0x18
    //clkgrp3_div2_cfg_outbuf_dyn[5:5] = 0x1
    //clkgrp3_div2_cfg2_mutesel[7:6] = 0x0
    {0x102, 0x38},

    //clkgrp4_div1_cfg1_en[0:0] = 0x1
    //clkgrp4_div1_cfg1_phdelta_mslip[1:1] = 0x1
    //clkgrp4_div1_cfg2_startmode[3:2] = 0x0
    //clkgrp4_div1_cfg1_altdivratmask[4:4] = 0x1
    //clkgrp4_div1_cfg1_slipmask[5:5] = 0x1
    //clkgrp4_div1_cfg1_reseedmask[6:6] = 0x1
    //clkgrp4_div1_cfg1_hi_perf[7:7] = 0x1
    {0x104, 0xF3},

    //clkgrp4_div1_cfg12_divrat_lsb[7:0] = 0x0
    {0x105, 0x0},

    //clkgrp4_div1_cfg12_divrat_msb[3:0] = 0x0
    {0x106, 0x0},

    //clkgrp4_div1_cfg5_fine_delay[4:0] = 0x0
    {0x107, 0x0},

    //clkgrp4_div1_cfg5_sel_coarse_delay[4:0] = 0x0
    {0x108, 0x0},

    //clkgrp4_div1_cfg12_phdelta_lsb[7:0] = 0x0
    //clkgrp4_div1_cfg12_mslip_lsb[7:0] = 0x0
    {0x109, 0x0},

    //clkgrp4_div1_cfg12_phdelta_msb[3:0] = 0x0
    //clkgrp4_div1_cfg12_mslip_msb[3:0] = 0x0
    {0x10A, 0x0},

    //clkgrp4_div1_cfg2_sel_outmux[1:0] = 0x3
    //clkgrp4_div1_cfg1_drvr_sel_testclk[2:2] = 0x0
    //clkgrp4_div1_cfg5_spares[7:3] = 0x0
    {0x10B, 0x3},

    //clkgrp4_div1_cfg5_drvr[4:0] = 0x1A
    //clkgrp4_div1_cfg_outbuf_dyn[5:5] = 0x0
    //clkgrp4_div1_cfg2_mutesel[7:6] = 0x0
    {0x10C, 0x1A},

    //clkgrp4_div2_cfg1_en[0:0] = 0x0
    //clkgrp4_div2_cfg1_phdelta_mslip[1:1] = 0x0
    //clkgrp4_div2_cfg2_startmode[3:2] = 0x3
    //clkgrp4_div2_cfg1_altdivratmask[4:4] = 0x1
    //clkgrp4_div2_cfg1_slipmask[5:5] = 0x1
    //clkgrp4_div2_cfg1_reseedmask[6:6] = 0x1
    //clkgrp4_div2_cfg1_hi_perf[7:7] = 0x0
    {0x10E, 0x7C},

    //clkgrp4_div2_cfg12_divrat_lsb[7:0] = 0x0
    {0x10F, 0x0},

    //clkgrp4_div2_cfg12_divrat_msb[3:0] = 0x1
    {0x110, 0x1},

    //clkgrp4_div2_cfg5_fine_delay[4:0] = 0x0
    {0x111, 0x0},

    //clkgrp4_div2_cfg5_sel_coarse_delay[4:0] = 0x0
    {0x112, 0x0},

    //clkgrp4_div2_cfg12_phdelta_lsb[7:0] = 0x0
    //clkgrp4_div2_cfg12_mslip_lsb[7:0] = 0x0
    {0x113, 0x0},

    //clkgrp4_div2_cfg12_mslip_msb[3:0] = 0x0
    //clkgrp4_div2_cfg12_phdelta_msb[3:0] = 0x0
    {0x114, 0x0},

    //clkgrp4_div2_cfg2_sel_outmux[1:0] = 0x3
    //clkgrp4_div2_cfg1_drvr_sel_testclk[2:2] = 0x0
    //clkgrp4_div2_cfg5_spares[7:3] = 0x0
    {0x115, 0x3},

    //clkgrp4_div2_cfg5_drvr[4:0] = 0x18
    //clkgrp4_div2_cfg_outbuf_dyn[5:5] = 0x1
    //clkgrp4_div2_cfg2_mutesel[7:6] = 0x0
    {0x116, 0x38},

    //clkgrp5_div1_cfg1_en[0:0] = 0x0
    //clkgrp5_div1_cfg1_phdelta_mslip[1:1] = 0x1
    //clkgrp5_div1_cfg2_startmode[3:2] = 0x0
    //clkgrp5_div1_cfg1_altdivratmask[4:4] = 0x1
    //clkgrp5_div1_cfg1_slipmask[5:5] = 0x1
    //clkgrp5_div1_cfg1_reseedmask[6:6] = 0x1
    //clkgrp5_div1_cfg1_hi_perf[7:7] = 0x0
    {0x118, 0x72},

    //clkgrp5_div1_cfg12_divrat_lsb[7:0] = 0x2
    {0x119, 0x2},

    //clkgrp5_div1_cfg12_divrat_msb[3:0] = 0x0
    {0x11A, 0x0},

    //clkgrp5_div1_cfg5_fine_delay[4:0] = 0x0
    {0x11B, 0x0},

    //clkgrp5_div1_cfg5_sel_coarse_delay[4:0] = 0x0
    {0x11C, 0x0},

    //clkgrp5_div1_cfg12_phdelta_lsb[7:0] = 0x0
    //clkgrp5_div1_cfg12_mslip_lsb[7:0] = 0x0
    {0x11D, 0x0},

    //clkgrp5_div1_cfg12_mslip_msb[3:0] = 0x0
    //clkgrp5_div1_cfg12_phdelta_msb[3:0] = 0x0
    {0x11E, 0x0},

    //clkgrp5_div1_cfg2_sel_outmux[1:0] = 0x3
    //clkgrp5_div1_cfg1_drvr_sel_testclk[2:2] = 0x0
    //clkgrp5_div1_cfg5_spares[7:3] = 0x0
    {0x11F, 0x3},

    //clkgrp5_div1_cfg5_drvr[4:0] = 0x18
    //clkgrp5_div1_cfg_outbuf_dyn[5:5] = 0x0
    //clkgrp5_div1_cfg2_mutesel[7:6] = 0x0
    {0x120, 0x18},

    //clkgrp5_div2_cfg1_en[0:0] = 0x0
    //clkgrp5_div2_cfg1_phdelta_mslip[1:1] = 0x0
    //clkgrp5_div2_cfg2_startmode[3:2] = 0x3
    //clkgrp5_div2_cfg1_altdivratmask[4:4] = 0x1
    //clkgrp5_div2_cfg1_slipmask[5:5] = 0x1
    //clkgrp5_div2_cfg1_reseedmask[6:6] = 0x1
    //clkgrp5_div2_cfg1_hi_perf[7:7] = 0x0
    {0x122, 0x7C},

    //clkgrp5_div2_cfg12_divrat_lsb[7:0] = 0x0
    {0x123, 0x0},

    //clkgrp5_div2_cfg12_divrat_msb[3:0] = 0x1
    {0x124, 0x1},

    //clkgrp5_div2_cfg5_fine_delay[4:0] = 0x0
    {0x125, 0x0},

    //clkgrp5_div2_cfg5_sel_coarse_delay[4:0] = 0x0
    {0x126, 0x0},

    //clkgrp5_div2_cfg12_phdelta_lsb[7:0] = 0x0
    //clkgrp5_div2_cfg12_mslip_lsb[7:0] = 0x0
    {0x127, 0x0},

    //clkgrp5_div2_cfg12_phdelta_msb[3:0] = 0x0
    //clkgrp5_div2_cfg12_mslip_msb[3:0] = 0x0
    {0x128, 0x0},

    //clkgrp5_div2_cfg2_sel_outmux[1:0] = 0x3
    //clkgrp5_div2_cfg1_drvr_sel_testclk[2:2] = 0x0
    //clkgrp5_div2_cfg5_spares[7:3] = 0x0
    {0x129, 0x3},

    //clkgrp5_div2_cfg5_drvr[4:0] = 0x18
    //clkgrp5_div2_cfg_outbuf_dyn[5:5] = 0x1
    //clkgrp5_div2_cfg2_mutesel[7:6] = 0x0
    {0x12A, 0x38},

    //clkgrp6_div1_cfg1_en[0:0] = 0x1
    //clkgrp6_div1_cfg1_phdelta_mslip[1:1] = 0x1
    //clkgrp6_div1_cfg2_startmode[3:2] = 0x0
    //clkgrp6_div1_cfg1_altdivratmask[4:4] = 0x1
    //clkgrp6_div1_cfg1_slipmask[5:5] = 0x1
    //clkgrp6_div1_cfg1_reseedmask[6:6] = 0x1
    //clkgrp6_div1_cfg1_hi_perf[7:7] = 0x1
    {0x12C, 0xF3},

    //clkgrp6_div1_cfg12_divrat_lsb[7:0] = 0x0
    {0x12D, 0x0},

    //clkgrp6_div1_cfg12_divrat_msb[3:0] = 0x0
    {0x12E, 0x0},

    //clkgrp6_div1_cfg5_fine_delay[4:0] = 0x0
    {0x12F, 0x0},

    //clkgrp6_div1_cfg5_sel_coarse_delay[4:0] = 0x0
    {0x130, 0x0},

    //clkgrp6_div1_cfg12_phdelta_lsb[7:0] = 0x0
    //clkgrp6_div1_cfg12_mslip_lsb[7:0] = 0x0
    {0x131, 0x0},

    //clkgrp6_div1_cfg12_mslip_msb[3:0] = 0x0
    //clkgrp6_div1_cfg12_phdelta_msb[3:0] = 0x0
    {0x132, 0x0},

    //clkgrp6_div1_cfg2_sel_outmux[1:0] = 0x3
    //clkgrp6_div1_cfg1_drvr_sel_testclk[2:2] = 0x0
    //clkgrp6_div1_cfg5_spares[7:3] = 0x0
    {0x133, 0x3},

    //clkgrp6_div1_cfg5_drvr[4:0] = 0x1A
    //clkgrp6_div1_cfg_outbuf_dyn[5:5] = 0x0
    //clkgrp6_div1_cfg2_mutesel[7:6] = 0x0
    {0x134, 0x10},

    //clkgrp6_div2_cfg1_en[0:0] = 0x0
    //clkgrp6_div2_cfg1_phdelta_mslip[1:1] = 0x0
    //clkgrp6_div2_cfg2_startmode[3:2] = 0x3
    //clkgrp6_div2_cfg1_altdivratmask[4:4] = 0x1
    //clkgrp6_div2_cfg1_slipmask[5:5] = 0x1
    //clkgrp6_div2_cfg1_reseedmask[6:6] = 0x1
    //clkgrp6_div2_cfg1_hi_perf[7:7] = 0x0
    {0x136, 0x7C},

    //clkgrp6_div2_cfg12_divrat_lsb[7:0] = 0x0
    {0x137, 0x0},

    //clkgrp6_div2_cfg12_divrat_msb[3:0] = 0x1
    {0x138, 0x1},

    //clkgrp6_div2_cfg5_fine_delay[4:0] = 0x0
    {0x139, 0x0},

    //clkgrp6_div2_cfg5_sel_coarse_delay[4:0] = 0x0
    {0x13A, 0x0},

    //clkgrp6_div2_cfg12_mslip_lsb[7:0] = 0x0
    //clkgrp6_div2_cfg12_phdelta_lsb[7:0] = 0x0
    {0x13B, 0x0},

    //clkgrp6_div2_cfg12_phdelta_msb[3:0] = 0x0
    //clkgrp6_div2_cfg12_mslip_msb[3:0] = 0x0
    {0x13C, 0x0},

    //clkgrp6_div2_cfg2_sel_outmux[1:0] = 0x3
    //clkgrp6_div2_cfg1_drvr_sel_testclk[2:2] = 0x0
    //clkgrp6_div2_cfg5_spares[7:3] = 0x0
    {0x13D, 0x3},

    //clkgrp6_div2_cfg5_drvr[4:0] = 0x18
    //clkgrp6_div2_cfg_outbuf_dyn[5:5] = 0x1
    //clkgrp6_div2_cfg2_mutesel[7:6] = 0x0
    {0x13E, 0x38},

    //clkgrp7_div1_cfg1_en[0:0] = 0x0
    //clkgrp7_div1_cfg1_phdelta_mslip[1:1] = 0x1
    //clkgrp7_div1_cfg2_startmode[3:2] = 0x0
    //clkgrp7_div1_cfg1_altdivratmask[4:4] = 0x1
    //clkgrp7_div1_cfg1_slipmask[5:5] = 0x1
    //clkgrp7_div1_cfg1_reseedmask[6:6] = 0x1
    //clkgrp7_div1_cfg1_hi_perf[7:7] = 0x0
    {0x140, 0x72},

    //clkgrp7_div1_cfg12_divrat_lsb[7:0] = 0x10
    {0x141, 0x10},

    //clkgrp7_div1_cfg12_divrat_msb[3:0] = 0x0
    {0x142, 0x0},

    //clkgrp7_div1_cfg5_fine_delay[4:0] = 0x0
    {0x143, 0x0},

    //clkgrp7_div1_cfg5_sel_coarse_delay[4:0] = 0x0
    {0x144, 0x0},

    //clkgrp7_div1_cfg12_phdelta_lsb[7:0] = 0x0
    //clkgrp7_div1_cfg12_mslip_lsb[7:0] = 0x0
    {0x145, 0x0},

    //clkgrp7_div1_cfg12_phdelta_msb[3:0] = 0x0
    //clkgrp7_div1_cfg12_mslip_msb[3:0] = 0x0
    {0x146, 0x0},

    //clkgrp7_div1_cfg2_sel_outmux[1:0] = 0x3
    //clkgrp7_div1_cfg1_drvr_sel_testclk[2:2] = 0x0
    //clkgrp7_div1_cfg5_spares[7:3] = 0x0
    {0x147, 0x3},

    //clkgrp7_div1_cfg5_drvr[4:0] = 0x18
    //clkgrp7_div1_cfg_outbuf_dyn[5:5] = 0x0
    //clkgrp7_div1_cfg2_mutesel[7:6] = 0x0
    {0x148, 0x18},

    //clkgrp7_div2_cfg1_en[0:0] = 0x0
    //clkgrp7_div2_cfg1_phdelta_mslip[1:1] = 0x0
    //clkgrp7_div2_cfg2_startmode[3:2] = 0x3
    //clkgrp7_div2_cfg1_altdivratmask[4:4] = 0x0
    //clkgrp7_div2_cfg1_slipmask[5:5] = 0x0
    //clkgrp7_div2_cfg1_reseedmask[6:6] = 0x1
    //clkgrp7_div2_cfg1_hi_perf[7:7] = 0x0
    {0x14A, 0x4C},

    //clkgrp7_div2_cfg12_divrat_lsb[7:0] = 0xA
    {0x14B, 0xA},

    //clkgrp7_div2_cfg12_divrat_msb[3:0] = 0x0
    {0x14C, 0x0},

    //clkgrp7_div2_cfg5_fine_delay[4:0] = 0x0
    {0x14D, 0x0},

    //clkgrp7_div2_cfg5_sel_coarse_delay[4:0] = 0x0
    {0x14E, 0x0},

    //clkgrp7_div2_cfg12_phdelta_lsb[7:0] = 0x0
    //clkgrp7_div2_cfg12_mslip_lsb[7:0] = 0x0
    {0x14F, 0x0},

    //clkgrp7_div2_cfg12_phdelta_msb[3:0] = 0x0
    //clkgrp7_div2_cfg12_mslip_msb[3:0] = 0x0
    {0x150, 0x0},

    //clkgrp7_div2_cfg2_sel_outmux[1:0] = 0x3
    //clkgrp7_div2_cfg1_drvr_sel_testclk[2:2] = 0x0
    //clkgrp7_div2_cfg5_spares[7:3] = 0x0
    {0x151, 0x3},

    //clkgrp7_div2_cfg5_drvr[4:0] = 0x18
    //clkgrp7_div2_cfg_outbuf_dyn[5:5] = 0x1
    //clkgrp7_div2_cfg2_mutesel[7:6] = 0x0
    {0x152, 0x38}};


const Ad9361Api_stInitParam DefaultAd9361Param = {
	/* Identification number */
	0,		//id_no;
	/* Reference Clock */
	30720000UL,	//reference_clk_rate
	/* Base Configuration */
	1,		//two_rx_two_tx_mode_enable *** adi,2rx-2tx-mode-enable
	1,		//one_rx_one_tx_mode_use_rx_num *** adi,1rx-1tx-mode-use-rx-num
	1,		//one_rx_one_tx_mode_use_tx_num *** adi,1rx-1tx-mode-use-tx-num
	1,		//frequency_division_duplex_mode_enable *** adi,frequency-division-duplex-mode-enable
	0,		//frequency_division_duplex_independent_mode_enable *** adi,frequency-division-duplex-independent-mode-enable
// TODO : JROY Verify the right value for TDD operation.
	1,		//tdd_use_dual_synth_mode_enable *** adi,tdd-use-dual-synth-mode-enable
	0,		//tdd_skip_vco_cal_enable *** adi,tdd-skip-vco-cal-enable
	0,		//tx_fastlock_delay_ns *** adi,tx-fastlock-delay-ns
	0,		//rx_fastlock_delay_ns *** adi,rx-fastlock-delay-ns
	0,		//rx_fastlock_pincontrol_enable *** adi,rx-fastlock-pincontrol-enable
	0,		//tx_fastlock_pincontrol_enable *** adi,tx-fastlock-pincontrol-enable
	0,		//external_rx_lo_enable *** adi,external-rx-lo-enable
	0,		//external_tx_lo_enable *** adi,external-tx-lo-enable
	5,		//dc_offset_tracking_update_event_mask *** adi,dc-offset-tracking-update-event-mask
	6,		//dc_offset_attenuation_high_range *** adi,dc-offset-attenuation-high-range
	5,		//dc_offset_attenuation_low_range *** adi,dc-offset-attenuation-low-range
	0x28,	//dc_offset_count_high_range *** adi,dc-offset-count-high-range
	0x32,	//dc_offset_count_low_range *** adi,dc-offset-count-low-range
	0,		//tdd_use_fdd_vco_tables_enable *** adi,tdd-use-fdd-vco-tables-enable
	0,		//split_gain_table_mode_enable *** adi,split-gain-table-mode-enable
	MAX_SYNTH_FREF,	//trx_synthesizer_target_fref_overwrite_hz *** adi,trx-synthesizer-target-fref-overwrite-hz
	0,		// qec_tracking_slow_mode_enable *** adi,qec-tracking-slow-mode-enable
	/* ENSM Control */
	0,		//ensm_enable_pin_pulse_mode_enable *** adi,ensm-enable-pin-pulse-mode-enable
	0,		//ensm_enable_txnrx_control_enable *** adi,ensm-enable-txnrx-control-enable
	/* LO Control */
	2400000000UL,	//rx_synthesizer_frequency_hz *** adi,rx-synthesizer-frequency-hz
	2400000000UL,	//tx_synthesizer_frequency_hz *** adi,tx-synthesizer-frequency-hz
	/* Rate & BW Control */
	{983040000, 491520000, 245760000, 122880000, 61440000, 30720000},//uint32_t	rx_path_clock_frequencies[6] *** adi,rx-path-clock-frequencies
	{983040000, 245760000, 245760000, 122880000, 61440000, 30720000},//uint32_t	tx_path_clock_frequencies[6] *** adi,tx-path-clock-frequencies
	18000000,//rf_rx_bandwidth_hz *** adi,rf-rx-bandwidth-hz
	18000000,//rf_tx_bandwidth_hz *** adi,rf-tx-bandwidth-hz
	/* RF Port Control */
	0,		//rx_rf_port_input_select *** adi,rx-rf-port-input-select
	0,		//tx_rf_port_input_select *** adi,tx-rf-port-input-select
	/* TX Attenuation Control */
	10000,	//tx_attenuation_mdB *** adi,tx-attenuation-mdB
	0,		//update_tx_gain_in_alert_enable *** adi,update-tx-gain-in-alert-enable
	/* Reference Clock Control */
	1,		//xo_disable_use_ext_refclk_enable *** adi,xo-disable-use-ext-refclk-enable
	{8, 5920},	//dcxo_coarse_and_fine_tune[2] *** adi,dcxo-coarse-and-fine-tune
	1,		//clk_output_mode_select *** adi,clk-output-mode-select
	/* Gain Control */
	2,		//gc_rx1_mode *** adi,gc-rx1-mode
	2,		//gc_rx2_mode *** adi,gc-rx2-mode
	58,		//gc_adc_large_overload_thresh *** adi,gc-adc-large-overload-thresh
	4,		//gc_adc_ovr_sample_size *** adi,gc-adc-ovr-sample-size
	47,		//gc_adc_small_overload_thresh *** adi,gc-adc-small-overload-thresh
	8192,	//gc_dec_pow_measurement_duration *** adi,gc-dec-pow-measurement-duration
	0,		//gc_dig_gain_enable *** adi,gc-dig-gain-enable
	800,	//gc_lmt_overload_high_thresh *** adi,gc-lmt-overload-high-thresh
	704,	//gc_lmt_overload_low_thresh *** adi,gc-lmt-overload-low-thresh
	24,		//gc_low_power_thresh *** adi,gc-low-power-thresh
	15,		//gc_max_dig_gain *** adi,gc-max-dig-gain
	/* Gain MGC Control */
	2,		//mgc_dec_gain_step *** adi,mgc-dec-gain-step
	2,		//mgc_inc_gain_step *** adi,mgc-inc-gain-step
	0,		//mgc_rx1_ctrl_inp_enable *** adi,mgc-rx1-ctrl-inp-enable
	0,		//mgc_rx2_ctrl_inp_enable *** adi,mgc-rx2-ctrl-inp-enable
	0,		//mgc_split_table_ctrl_inp_gain_mode *** adi,mgc-split-table-ctrl-inp-gain-mode
	/* Gain AGC Control */
	10,		//agc_adc_large_overload_exceed_counter *** adi,agc-adc-large-overload-exceed-counter
	2,		//agc_adc_large_overload_inc_steps *** adi,agc-adc-large-overload-inc-steps
	0,		//agc_adc_lmt_small_overload_prevent_gain_inc_enable *** adi,agc-adc-lmt-small-overload-prevent-gain-inc-enable
	10,		//agc_adc_small_overload_exceed_counter *** adi,agc-adc-small-overload-exceed-counter
	4,		//agc_dig_gain_step_size *** adi,agc-dig-gain-step-size
	3,		//agc_dig_saturation_exceed_counter *** adi,agc-dig-saturation-exceed-counter
	1000,	// agc_gain_update_interval_us *** adi,agc-gain-update-interval-us
	0,		//agc_immed_gain_change_if_large_adc_overload_enable *** adi,agc-immed-gain-change-if-large-adc-overload-enable
	0,		//agc_immed_gain_change_if_large_lmt_overload_enable *** adi,agc-immed-gain-change-if-large-lmt-overload-enable
	10,		//agc_inner_thresh_high *** adi,agc-inner-thresh-high
	1,		//agc_inner_thresh_high_dec_steps *** adi,agc-inner-thresh-high-dec-steps
	12,		//agc_inner_thresh_low *** adi,agc-inner-thresh-low
	1,		//agc_inner_thresh_low_inc_steps *** adi,agc-inner-thresh-low-inc-steps
	10,		//agc_lmt_overload_large_exceed_counter *** adi,agc-lmt-overload-large-exceed-counter
	2,		//agc_lmt_overload_large_inc_steps *** adi,agc-lmt-overload-large-inc-steps
	10,		//agc_lmt_overload_small_exceed_counter *** adi,agc-lmt-overload-small-exceed-counter
	5,		//agc_outer_thresh_high *** adi,agc-outer-thresh-high
	2,		//agc_outer_thresh_high_dec_steps *** adi,agc-outer-thresh-high-dec-steps
	18,		//agc_outer_thresh_low *** adi,agc-outer-thresh-low
	2,		//agc_outer_thresh_low_inc_steps *** adi,agc-outer-thresh-low-inc-steps
	1,		//agc_attack_delay_extra_margin_us; *** adi,agc-attack-delay-extra-margin-us
	0,		//agc_sync_for_gain_counter_enable *** adi,agc-sync-for-gain-counter-enable
	/* Fast AGC */
	64,		//fagc_dec_pow_measuremnt_duration ***  adi,fagc-dec-pow-measurement-duration
	260,	//fagc_state_wait_time_ns ***  adi,fagc-state-wait-time-ns
		/* Fast AGC - Low Power */
	0,		//fagc_allow_agc_gain_increase ***  adi,fagc-allow-agc-gain-increase-enable
	5,		//fagc_lp_thresh_increment_time ***  adi,fagc-lp-thresh-increment-time
	1,		//fagc_lp_thresh_increment_steps ***  adi,fagc-lp-thresh-increment-steps
		/* Fast AGC - Lock Level */
	10,		//fagc_lock_level ***  adi,fagc-lock-level */
	1,		//fagc_lock_level_lmt_gain_increase_en ***  adi,fagc-lock-level-lmt-gain-increase-enable
	5,		//fagc_lock_level_gain_increase_upper_limit ***  adi,fagc-lock-level-gain-increase-upper-limit
		/* Fast AGC - Peak Detectors and Final Settling */
	1,		//fagc_lpf_final_settling_steps ***  adi,fagc-lpf-final-settling-steps
	1,		//fagc_lmt_final_settling_steps ***  adi,fagc-lmt-final-settling-steps
	3,		//fagc_final_overrange_count ***  adi,fagc-final-overrange-count
		/* Fast AGC - Final Power Test */
	0,		//fagc_gain_increase_after_gain_lock_en ***  adi,fagc-gain-increase-after-gain-lock-enable
		/* Fast AGC - Unlocking the Gain */
	0,		//fagc_gain_index_type_after_exit_rx_mode ***  adi,fagc-gain-index-type-after-exit-rx-mode
	1,		//fagc_use_last_lock_level_for_set_gain_en ***  adi,fagc-use-last-lock-level-for-set-gain-enable
	1,		//fagc_rst_gla_stronger_sig_thresh_exceeded_en ***  adi,fagc-rst-gla-stronger-sig-thresh-exceeded-enable
	5,		//fagc_optimized_gain_offset ***  adi,fagc-optimized-gain-offset
	10,		//fagc_rst_gla_stronger_sig_thresh_above_ll ***  adi,fagc-rst-gla-stronger-sig-thresh-above-ll
	1,		//fagc_rst_gla_engergy_lost_sig_thresh_exceeded_en ***  adi,fagc-rst-gla-engergy-lost-sig-thresh-exceeded-enable
	1,		//fagc_rst_gla_engergy_lost_goto_optim_gain_en ***  adi,fagc-rst-gla-engergy-lost-goto-optim-gain-enable
	10,		//fagc_rst_gla_engergy_lost_sig_thresh_below_ll ***  adi,fagc-rst-gla-engergy-lost-sig-thresh-below-ll
	8,		//fagc_energy_lost_stronger_sig_gain_lock_exit_cnt ***  adi,fagc-energy-lost-stronger-sig-gain-lock-exit-cnt
	1,		//fagc_rst_gla_large_adc_overload_en ***  adi,fagc-rst-gla-large-adc-overload-enable
	1,		//fagc_rst_gla_large_lmt_overload_en ***  adi,fagc-rst-gla-large-lmt-overload-enable
	0,		//fagc_rst_gla_en_agc_pulled_high_en ***  adi,fagc-rst-gla-en-agc-pulled-high-enable
	0,		//fagc_rst_gla_if_en_agc_pulled_high_mode ***  adi,fagc-rst-gla-if-en-agc-pulled-high-mode
	64,		//fagc_power_measurement_duration_in_state5 ***  adi,fagc-power-measurement-duration-in-state5
	/* RSSI Control */
	1,		//rssi_delay *** adi,rssi-delay
	1000,	//rssi_duration *** adi,rssi-duration
	3,		//rssi_restart_mode *** adi,rssi-restart-mode
	0,		//rssi_unit_is_rx_samples_enable *** adi,rssi-unit-is-rx-samples-enable
	1,		//rssi_wait *** adi,rssi-wait
	/* Aux ADC Control */
	256,	//aux_adc_decimation *** adi,aux-adc-decimation
	40000000UL,	//aux_adc_rate *** adi,aux-adc-rate
	/* AuxDAC Control */
	1,		//aux_dac_manual_mode_enable ***  adi,aux-dac-manual-mode-enable
	0,		//aux_dac1_default_value_mV ***  adi,aux-dac1-default-value-mV
	0,		//aux_dac1_active_in_rx_enable ***  adi,aux-dac1-active-in-rx-enable
	0,		//aux_dac1_active_in_tx_enable ***  adi,aux-dac1-active-in-tx-enable
	0,		//aux_dac1_active_in_alert_enable ***  adi,aux-dac1-active-in-alert-enable
	0,		//aux_dac1_rx_delay_us ***  adi,aux-dac1-rx-delay-us
	0,		//aux_dac1_tx_delay_us ***  adi,aux-dac1-tx-delay-us
	0,		//aux_dac2_default_value_mV ***  adi,aux-dac2-default-value-mV
	0,		//aux_dac2_active_in_rx_enable ***  adi,aux-dac2-active-in-rx-enable
	0,		//aux_dac2_active_in_tx_enable ***  adi,aux-dac2-active-in-tx-enable
	0,		//aux_dac2_active_in_alert_enable ***  adi,aux-dac2-active-in-alert-enable
	0,		//aux_dac2_rx_delay_us ***  adi,aux-dac2-rx-delay-us
	0,		//aux_dac2_tx_delay_us ***  adi,aux-dac2-tx-delay-us
	/* Temperature Sensor Control */
	256,	//temp_sense_decimation *** adi,temp-sense-decimation
	1000,	//temp_sense_measurement_interval_ms *** adi,temp-sense-measurement-interval-ms
	0xCE,	//temp_sense_offset_signed *** adi,temp-sense-offset-signed
	1,		//temp_sense_periodic_measurement_enable *** adi,temp-sense-periodic-measurement-enable
	/* Control Out Setup */
	0xFF,	//ctrl_outs_enable_mask *** adi,ctrl-outs-enable-mask
	0,		//ctrl_outs_index *** adi,ctrl-outs-index
	/* External LNA Control */
	0,		//elna_settling_delay_ns *** adi,elna-settling-delay-ns
	0,		//elna_gain_mdB *** adi,elna-gain-mdB
	0,		//elna_bypass_loss_mdB *** adi,elna-bypass-loss-mdB
	0,		//elna_rx1_gpo0_control_enable *** adi,elna-rx1-gpo0-control-enable
	0,		//elna_rx2_gpo1_control_enable *** adi,elna-rx2-gpo1-control-enable
	0,		//elna_gaintable_all_index_enable *** adi,elna-gaintable-all-index-enable
	/* Digital Interface Control */
	0,		//digital_interface_tune_skip_mode *** adi,digital-interface-tune-skip-mode
	0,		//digital_interface_tune_fir_disable *** adi,digital-interface-tune-fir-disable
	1,		//pp_tx_swap_enable *** adi,pp-tx-swap-enable
	1,		//pp_rx_swap_enable *** adi,pp-rx-swap-enable
	0,		//tx_channel_swap_enable *** adi,tx-channel-swap-enable
	0,		//rx_channel_swap_enable *** adi,rx-channel-swap-enable
	1,		//rx_frame_pulse_mode_enable *** adi,rx-frame-pulse-mode-enable
	1,		//two_t_two_r_timing_enable *** adi,2t2r-timing-enable
	0,		//invert_data_bus_enable *** adi,invert-data-bus-enable
	0,		//invert_data_clk_enable *** adi,invert-data-clk-enable
	0,		//fdd_alt_word_order_enable *** adi,fdd-alt-word-order-enable
	0,		//invert_rx_frame_enable *** adi,invert-rx-frame-enable
	0,		//fdd_rx_rate_2tx_enable *** adi,fdd-rx-rate-2tx-enable
	0,		//swap_ports_enable *** adi,swap-ports-enable
	0,		//single_data_rate_enable *** adi,single-data-rate-enable
	1,		//lvds_mode_enable *** adi,lvds-mode-enable
	0,		//half_duplex_mode_enable *** adi,half-duplex-mode-enable
	0,		//single_port_mode_enable *** adi,single-port-mode-enable
	0,		//full_port_enable *** adi,full-port-enable
	0,		//full_duplex_swap_bits_enable *** adi,full-duplex-swap-bits-enable
	0,		//delay_rx_data *** adi,delay-rx-data
	0,		//rx_data_clock_delay *** adi,rx-data-clock-delay
	4,		//rx_data_delay *** adi,rx-data-delay
	7,		//tx_fb_clock_delay *** adi,tx-fb-clock-delay
	0,		//tx_data_delay *** adi,tx-data-delay
	150,	//lvds_bias_mV *** adi,lvds-bias-mV
	1,		//lvds_rx_onchip_termination_enable *** adi,lvds-rx-onchip-termination-enable
	0,		//rx1rx2_phase_inversion_en *** adi,rx1-rx2-phase-inversion-enable
	0xFF,	//lvds_invert1_control *** adi,lvds-invert1-control
	0x0F,	//lvds_invert2_control *** adi,lvds-invert2-control
	/* GPO Control */
	0,		//gpo0_inactive_state_high_enable *** adi,gpo0-inactive-state-high-enable
	0,		//gpo1_inactive_state_high_enable *** adi,gpo1-inactive-state-high-enable
	0,		//gpo2_inactive_state_high_enable *** adi,gpo2-inactive-state-high-enable
	0,		//gpo3_inactive_state_high_enable *** adi,gpo3-inactive-state-high-enable
	0,		//gpo0_slave_rx_enable *** adi,gpo0-slave-rx-enable
	0,		//gpo0_slave_tx_enable *** adi,gpo0-slave-tx-enable
	0,		//gpo1_slave_rx_enable *** adi,gpo1-slave-rx-enable
	0,		//gpo1_slave_tx_enable *** adi,gpo1-slave-tx-enable
	0,		//gpo2_slave_rx_enable *** adi,gpo2-slave-rx-enable
	0,		//gpo2_slave_tx_enable *** adi,gpo2-slave-tx-enable
	0,		//gpo3_slave_rx_enable *** adi,gpo3-slave-rx-enable
	0,		//gpo3_slave_tx_enable *** adi,gpo3-slave-tx-enable
	0,		//gpo0_rx_delay_us *** adi,gpo0-rx-delay-us
	0,		//gpo0_tx_delay_us *** adi,gpo0-tx-delay-us
	0,		//gpo1_rx_delay_us *** adi,gpo1-rx-delay-us
	0,		//gpo1_tx_delay_us *** adi,gpo1-tx-delay-us
	0,		//gpo2_rx_delay_us *** adi,gpo2-rx-delay-us
	0,		//gpo2_tx_delay_us *** adi,gpo2-tx-delay-us
	0,		//gpo3_rx_delay_us *** adi,gpo3-rx-delay-us
	0,		//gpo3_tx_delay_us *** adi,gpo3-tx-delay-us
	/* Tx Monitor Control */
	37000,	//low_high_gain_threshold_mdB *** adi,txmon-low-high-thresh
	0,		//low_gain_dB *** adi,txmon-low-gain
	24,		//high_gain_dB *** adi,txmon-high-gain
	0,		//tx_mon_track_en *** adi,txmon-dc-tracking-enable
	0,		//one_shot_mode_en *** adi,txmon-one-shot-mode-enable
	511,	//tx_mon_delay *** adi,txmon-delay
	8192,	//tx_mon_duration *** adi,txmon-duration
	2,		//tx1_mon_front_end_gain *** adi,txmon-1-front-end-gain
	2,		//tx2_mon_front_end_gain *** adi,txmon-2-front-end-gain
	48,		//tx1_mon_lo_cm *** adi,txmon-1-lo-cm
	48,		//tx2_mon_lo_cm *** adi,txmon-2-lo-cm
	/* GPIO definitions */
	-1,		//gpio_resetb *** reset-gpios
	/* MCS Sync */
	-1,		//gpio_sync *** sync-gpios
	-1,		//gpio_cal_sw1 *** cal-sw1-gpios
	-1,		//gpio_cal_sw2 *** cal-sw2-gpios
	/* External LO clocks */
	NULL,	//(*ad9361_rfpll_ext_recalc_rate)()
	NULL,	//(*ad9361_rfpll_ext_round_rate)()
	NULL	//(*ad9361_rfpll_ext_set_rate)()
};

#ifdef __cplusplus
}
#endif  // extern "C"

#endif

