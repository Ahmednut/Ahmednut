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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       **** 
 *                                                                        ***       
 * 
 ************************************************************************//**  
 * 
 * @file    lms6002.h 
 * @brief   Definitions for the lms6002 registers
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * This file is part of Nutaq’s BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq’s BAS Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq’s BAS Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq’s BAS Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq’s BAS Software Suite under the GPL requires that your work based on
 * Nutaq’s BAS Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq’s BAS Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq’s BAS Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/
#ifndef __ADP_LMS6002_REGS__
#define __ADP_LMS6002_REGS__

typedef struct S_lms6002_Register_Map
{
    union
    {
        unsigned char RegisterArray[0x7c];
        struct 
        {
            union U_lms6002_regs_00
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, dc_regval:6;
#else
                    unsigned char dc_regval:6, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_00;

            union U_lms6002_regs_01
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char rccal_lpfcal:3, dc_lock:3, dc_clbr_done:1, dc_ud:1;
#else
                    unsigned char dc_ud:1, dc_clbr_done:1, dc_lock:3, rccal_lpfcal:3;
#endif
                }Bit;
                unsigned char raw;
            }Register_01;

            union U_lms6002_regs_02
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, dc_cntval:6;
#else
                    unsigned char dc_cntval:6, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_02;

            union U_lms6002_regs_03
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, dc_start_clbr:1, dc_load:1, dc_sreset:1, dc_addr:3;
#else
                    unsigned char dc_addr:3, dc_sreset:1, dc_load:1, dc_start_clbr:1, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_03;

            union U_lms6002_regs_04
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char ver:4, rev:4;
#else
                    unsigned char rev:4, ver:4;
#endif
                }Bit;
                unsigned char raw;
            }Register_04;

            union U_lms6002_regs_05
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char decode:1, not_used:1, sreset:1, en:1, stxen:1, srxen:1, tfwmode:1, not_used2:1;
#else
                    unsigned char not_used2:1, tfwmode:1, srxen:1, stxen:1, en:1, sreset:1, not_used:1, decode:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_05;

            union U_lms6002_regs_06
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:4, clksel_lpfcal:1, pd_clklpfcal:1, enf_en_cal_lpfcal:1, rst_cal_lpfcal:1;
#else
                    unsigned char rst_cal_lpfcal:1, enf_en_cal_lpfcal:1, pd_clklpfcal:1, clksel_lpfcal:1, not_used:4;
#endif
                }Bit;
                unsigned char raw;
            }Register_06;

            union U_lms6002_regs_07
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char en_cal_lpfcal:1, force_code_cal_lpfcal:3, bwc_lpfcal:4;
#else
                    unsigned char bwc_lpfcal:4, force_code_cal_lpfcal:3, en_cal_lpfcal:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_07;

            union U_lms6002_regs_08
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char reserved:1, lben_lpfin:1, lben_vga2in:1, lben_opin:1, lbrfen:4;
#else
                    unsigned char lbrfen:4, lben_opin:1, lben_vga2in:1, lben_lpfin:1, reserved:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_08;

            union U_lms6002_regs_09
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char rxoutsw:1, clk_en_pll:1, clk_en_lpf:1, clk_en_vga2_dccal:1, clk_en_rx_lpf_dccal:1, clk_en_rx_dsm_spi:1, clk_en_tx_lpf_dccal:1, clk_en_tx_dsm_spi:1;
#else
                    unsigned char clk_en_tx_dsm_spi:1, clk_en_tx_lpf_dccal:1, clk_en_rx_dsm_spi:1, clk_en_rx_lpf_dccal:1, clk_en_vga2_dccal:1, clk_en_lpf:1, clk_en_pll:1, rxoutsw:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_09;

            union U_lms6002_regs_0A
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:6, fddtdd:1, tddmod:1;
#else
                    unsigned char tddmod:1, fddtdd:1, not_used:6;
#endif
                }Bit;
                unsigned char raw;
            }Register_0A;

            union U_lms6002_regs_0B
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:3, pdxcobuf:1, clfbxobuf:1, bypxcobuf:1, pd_dcoref_lpfcal:1, pd_rf_loopback:1;
#else
                    unsigned char pd_rf_loopback:1, pd_dcoref_lpfcal:1, bypxcobuf:1, clfbxobuf:1, pdxcobuf:1, not_used:3;
#endif
                }Bit;
                unsigned char raw;
            }Register_0B;
            
            union U_lms6002_regs_0C
            {
                struct
                {
                    unsigned char reserved;
                }Bit;
                unsigned char raw;
            }Register_0C;
            
            union U_lms6002_regs_0D
            {
                struct
                {
                    unsigned char reserved;
                }Bit;
                unsigned char raw;
            }Register_0D;

            union U_lms6002_regs_0E
            {
                struct
                {
                    unsigned char spare0;
                }Bit;
                unsigned char raw;
            }Register_0E;
       
            union U_lms6002_regs_0F
            {
                struct
                {
                    unsigned char spare1;
                }Bit;
                unsigned char raw;
            }Register_0F;

            union U_lms6002_regs_10
            {
                struct
                {
                    unsigned char nint_msb:8;
                }Bit;
                unsigned char raw;
            }Register_10;

            union U_lms6002_regs_11
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char nint_lsb:1, nfrac_msb:7;
#else
                    unsigned char nfrac_msb:7, nint_lsb:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_11;

            union U_lms6002_regs_12
            {
                struct
                {
                    unsigned char nfrac_csb:8;
                }Bit;
                unsigned char raw;
            }Register_12;

            union U_lms6002_regs_13
            {
                struct
                {
                    unsigned char nfrac_lsb:8;
                }Bit;
                unsigned char raw;
            }Register_13;

            union U_lms6002_regs_14
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char dithen:1, dithn:3, en:1, autobyp:1, decode:1, reserved:1;
#else
                    unsigned char reserved:1, decode:1, autobyp:1, en:1, dithn:3, dithen:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_14;

            union U_lms6002_regs_15
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char freqsel:6, selout:2;
#else
                    unsigned char selout:2, freqsel:6;
#endif
                }Bit;
                unsigned char raw;
            }Register_15;

            union U_lms6002_regs_16
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char en_pfd_up:1, oen_tstd_sx:1, passen_tstod_sd:1, ichp:5;
#else
                    unsigned char ichp:5, passen_tstod_sd:1, oen_tstd_sx:1, en_pfd_up:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_16;

            union U_lms6002_regs_17
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char bypvcoreg:1, pdvcoreg:1, fstvcobg:1, offup:5;
#else
                    unsigned char offup:5, fstvcobg:1, pdvcoreg:1, bypvcoreg:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_17;

            union U_lms6002_regs_18
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char vovcoreg_msb:3, offdown:5;
#else
                    unsigned char offdown:5, vovcoreg_msb:3;
#endif
                }Bit;
                unsigned char raw;
            }Register_18;

            union U_lms6002_regs_19
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char vovcoreg_lsb:1, not_used:1, vcocap:6;
#else
                    unsigned char vcocap:6, not_used:1, vovcoreg_lsb:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_19;

            union U_lms6002_regs_1A
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char vtune_h:1, vtune_l:1, reserved:6;
#else
                    unsigned char reserved:6, vtune_l:1, vtune_h:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_1A;

            union U_lms6002_regs_1B
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char reserved1:4, pd_vcocomp_sx:1, reserved2:1, reserved3:1, reserved4:1;
#else
                    unsigned char reserved4:1, reserved3:1, reserved2:1, pd_vcocomp_sx:1, reserved1:4;
#endif
                }Bit;
                unsigned char raw;
            }Register_1B;

            union U_lms6002_regs_1C
            {
                struct
                {
                    unsigned char reserved:8;
                }Bit;
                unsigned char raw;
            }Register_1C;

            union U_lms6002_regs_1D
            {
                struct
                {
                    unsigned char reserved:8;
                }Bit;
                unsigned char raw;
            }Register_1D;

            union U_lms6002_regs_1E
            {
                struct
                {
                    unsigned char reserved:8;
                }Bit;
                unsigned char raw;
            }Register_1E;

            union U_lms6002_regs_1F
            {
                struct
                {
                    unsigned char reserved:8;
                }Bit;
                unsigned char raw;
            }Register_1F;

            union U_lms6002_regs_20
            {
                struct
                {
                    unsigned char nint_msb:8;
                }Bit;
                unsigned char raw;
            }Register_20;

            union U_lms6002_regs_21
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char nint_lsb:1, nfrac_msb:7;
#else
                    unsigned char nfrac_msb:7, nint_lsb:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_21;

            union U_lms6002_regs_22
            {
                struct
                {
                    unsigned char nfrac_csb:8;
                }Bit;
                unsigned char raw;
            }Register_22;

            union U_lms6002_regs_23
            {
                struct
                {
                    unsigned char nfrac_lsb:8;
                }Bit;
                unsigned char raw;
            }Register_23;

            union U_lms6002_regs_24
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char dithen:1, dithn:3, en:1, autobyp:1, decode:1, reserved:1;
#else
                    unsigned char reserved:1, decode:1, autobyp:1, en:1, dithn:3, dithen:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_24;

            union U_lms6002_regs_25
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char freqsel:6, selout:2;
#else
                    unsigned char selout:2, freqsel:6;
#endif
                }Bit;
                unsigned char raw;
            }Register_25;

            union U_lms6002_regs_26
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char en_pfd_up:1, oen_tstd_sx:1, passen_tstod_sd:1, ichp:5;
#else
                    unsigned char ichp:5, passen_tstod_sd:1, oen_tstd_sx:1, en_pfd_up:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_26;

            union U_lms6002_regs_27
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char bypvcoreg:1, pdvcoreg:1, fstvcobg:1, offup:5;
#else
                    unsigned char offup:5, fstvcobg:1, pdvcoreg:1, bypvcoreg:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_27;

            union U_lms6002_regs_28
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char vovcoreg_msb:3, offdown:5;
#else
                    unsigned char offdown:5, vovcoreg_msb:3;
#endif
                }Bit;
                unsigned char raw;
            }Register_28;

            union U_lms6002_regs_29
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char vovcoreg_lsb:1, not_used:1, vcocap:6;
#else
                    unsigned char vcocap:6, not_used:1, vovcoreg_lsb:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_29;

            union U_lms6002_regs_2A
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char vtune_h:1, vtune_l:1, reserved:6;
#else
                    unsigned char reserved:6, vtune_l:1, vtune_h:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_2A;

            union U_lms6002_regs_2B
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char reserved1:4, pd_vcocomp_sx:1, reserved2:1, reserved3:1, reserved4:1;
#else
                    unsigned char reserved4:1, reserved3:1, reserved2:1, pd_vcocomp_sx:1, reserved1:4;
#endif
                }Bit;
                unsigned char raw;
            }Register_2B;

            union U_lms6002_regs_2C
            {
                struct
                {
                    unsigned char reserved:8;
                }Bit;
                unsigned char raw;
            }Register_2C;

            union U_lms6002_regs_2D
            {
                struct
                {
                    unsigned char reserved:8;
                }Bit;
                unsigned char raw;
            }Register_2D;

            union U_lms6002_regs_2E
            {
                struct
                {
                    unsigned char reserved:8;
                }Bit;
                unsigned char raw;
            }Register_2E;

            union U_lms6002_regs_2F
            {
                struct
                {
                    unsigned char reserved:8;
                }Bit;
                unsigned char raw;
            }Register_2F;

            union U_lms6002_regs_30
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, dc_regval:6;
#else
                    unsigned char dc_regval:6, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_30;

            union U_lms6002_regs_31
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:3, dc_lock:3, dc_clbr_done:1, dc_ud:1;
#else
                    unsigned char dc_ud:1, dc_clbr_done:1, dc_lock:3, not_used:3;
#endif
                }Bit;
                unsigned char raw;
            }Register_31;

            union U_lms6002_regs_32
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, dc_cntval:6;
#else
                    unsigned char dc_cntval:6, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_32;

            union U_lms6002_regs_33
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, dc_start_clbr:1, dc_load:1, dc_reset:1, dc_addr:3;
#else
                    unsigned char dc_addr:3, dc_reset:1, dc_load:1, dc_start_clbr:1, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_33;

            union U_lms6002_regs_34
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, bwc_lpf:4, en:1, decode:1;
#else
                    unsigned char decode:1, en:1, bwc_lpf:4, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_34;

            union U_lms6002_regs_35
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:1, byp_en_lpf:1, dco_daccal:6;
#else
                    unsigned char dco_daccal:6, byp_en_lpf:1, not_used:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_35;

            union U_lms6002_regs_36
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char tx_dacbuf_pd:1, rccal_lpf:3, pd_dcocmp_lpf:1, pd_dcodac_lpf:1, pd_dcoref_lpf:1, pd_fil_lpf:1;
#else
                    unsigned char pd_fil_lpf:1, pd_dcoref_lpf:1, pd_dcodac_lpf:1, pd_dcocmp_lpf:1, rccal_lpf:3, tx_dacbuf_pd:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_36;

            union U_lms6002_regs_37
            {
                struct
                {
                    unsigned char reserved:8;
                }Bit;
                unsigned char raw;
            }Register_37;

            union U_lms6002_regs_38
            {
                struct
                {
                    unsigned char reserved:8;
                }Bit;
                unsigned char raw;
            }Register_38;

            union U_lms6002_regs_39
            {
                struct
                {
                    unsigned char reserved:8;
                }Bit;
                unsigned char raw;
            }Register_39;

            union U_lms6002_regs_3A
            {
                struct
                {
                    unsigned char reserved:8;
                }Bit;
                unsigned char raw;
            }Register_3A;

            union U_lms6002_regs_3B
            {
                struct
                {
                    unsigned char reserved:8;
                }Bit;
                unsigned char raw;
            }Register_3B;

            union U_lms6002_regs_3C
            {
                struct
                {
                    unsigned char reserved:8;
                }Bit;
                unsigned char raw;
            }Register_3C;

            union U_lms6002_regs_3D
            {
                struct
                {
                    unsigned char reserved:8;
                }Bit;
                unsigned char raw;
            }Register_3D;

            union U_lms6002_regs_3E
            {
                struct
                {
                    unsigned char spare0:8;
                }Bit;
                unsigned char raw;
            }Register_3E;

            union U_lms6002_regs_3F
            {
                struct
                {
                    unsigned char spare1:8;
                }Bit;
                unsigned char raw;
            }Register_3F;

            
////////////////////////////////////////////////////////////////

            union U_lms6002_regs_40
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:6, txfe_en:1, decode:1;
#else
                    unsigned char decode:1, txfe_en:1, not_used:6;
#endif
                }Bit;
                unsigned char raw;
            }Register_40;

            union U_lms6002_regs_41
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:3, txvga1_gain:5;
#else
                    unsigned char txvga1_gain:5, not_used:3;
#endif
                }Bit;
                unsigned char raw;
            }Register_41;

            union U_lms6002_regs_42
            {
                struct
                {
                    unsigned char txvga1_dc_leakage_cancel_i:8;
                }Bit;
                unsigned char raw;
            }Register_42;

            union U_lms6002_regs_43
            {
                struct
                {
                    unsigned char txvga1_dc_leakage_cancel_q:8;
                }Bit;
                unsigned char raw;
            }Register_43;

            union U_lms6002_regs_44
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:3, pa_en:3, aux_pa_pd:1, detector_pd:1;
#else
                    unsigned char detector_pd:1, aux_pa_pd:1, pa_en:3, not_used:3;
#endif
                }Bit;
                unsigned char raw;
            }Register_44;

            union U_lms6002_regs_45
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char txvga2_gain:5, detector_mux:3;
#else
                    unsigned char detector_mux:3, txvga2_gain:5;
#endif
                }Bit;
                unsigned char raw;
            }Register_45;

            union U_lms6002_regs_46
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char detector_bandwidth:4, baseband_loopback:2, fst_pkdet:1, fst_txhfbias:1;
#else
                    unsigned char fst_txhfbias:1, fst_pkdet:1, baseband_loopback:2, detector_bandwidth:4;
#endif
                }Bit;
                unsigned char raw;
            }Register_46;

            union U_lms6002_regs_47
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char lo_bias_current:4, vbcas_txdrv:4;
#else
                    unsigned char vbcas_txdrv:4, lo_bias_current:4;
#endif
                }Bit;
                unsigned char raw;
            }Register_47;

            union U_lms6002_regs_48
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:3, mixer_bias:5;
#else
                    unsigned char mixer_bias:5, not_used:3;
#endif
                }Bit;
                unsigned char raw;
            }Register_48;

            union U_lms6002_regs_49
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:3, pa_bias:5;
#else
                    unsigned char pa_bias:5, not_used:3;
#endif
                }Bit;
                unsigned char raw;
            }Register_49;

            union U_lms6002_regs_4A
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:3, i_power:1, q_power:1, pd_pa:1, pd_tx:1, pd_txmix:1;
#else
                    unsigned char pd_txmix:1, pd_tx:1, pd_pa:1, q_power:1, i_power:1, not_used:3;
#endif
                }Bit;
                unsigned char raw;
            }Register_4A;

            union U_lms6002_regs_4B
            {
                struct
                {
                    unsigned char tx_vga1_gain:8;
                }Bit;
                unsigned char raw;
            }Register_4B;

            union U_lms6002_regs_4C
            {
                struct
                {
                    unsigned char tx_vga2_gain_msb:8;
                }Bit;
                unsigned char raw;
            }Register_4C;

            union U_lms6002_regs_4D
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char tx_vga2_gain_lsb:1, not_used:7;
#else
                    unsigned char not_used:7, tx_vga2_gain_lsb:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_4D;

            union U_lms6002_regs_4E
            {
                struct
                {
                    unsigned char spare0:8;
                }Bit;
                unsigned char raw;
            }Register_4E;

            union U_lms6002_regs_4F
            {
                struct
                {
                    unsigned char spare1:8;
                }Bit;
                unsigned char raw;
            }Register_4F;

////////////////////////////////////////////////////////////////

            union U_lms6002_regs_50
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, dc_regval:6;
#else
                    unsigned char dc_regval:6, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_50;

            union U_lms6002_regs_51
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:3, dc_lock:3, dc_clbr_done:1, dc_ud:1;
#else
                    unsigned char dc_ud:1, dc_clbr_done:1, dc_lock:3, not_used:3;
#endif
                }Bit;
                unsigned char raw;
            }Register_51;

            union U_lms6002_regs_52
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, dc_cntval:6;
#else
                    unsigned char dc_cntval:6, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_52;

            union U_lms6002_regs_53
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, dc_start_clbr:1, dc_load:1, dc_sreset:1, dc_addr:3;
#else
                    unsigned char dc_addr:3, dc_sreset:1, dc_load:1, dc_start_clbr:1, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_53;

            union U_lms6002_regs_54
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, bwc_lpf:4, en_lpf:1, decode:1;
#else
                    unsigned char decode:1, en_lpf:1, bwc_lpf:4, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_54;

            union U_lms6002_regs_55
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:1, byp_en_lpf:1, dco_daccal:6;
#else
                    unsigned char dco_daccal:6, byp_en_lpf:1, not_used:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_55;
    
            union U_lms6002_regs_56
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:1, rccal_lpf:3, pd_dcocmp_lpf:1, pd_dcodac_lpf:1, pd_dcoref_lpf:1, pd_fil_lpf:1;
#else
                    unsigned char pd_fil_lpf:1, pd_dcoref_lpf:1, pd_dcodac_lpf:1, pd_dcocmp_lpf:1, rccal_lpf:3, not_used:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_56;

            union U_lms6002_regs_57
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char en_adc_dac:1, decode:1, tx_ctrl_load_resistor:3, tx_ctrl_resitor_internal:1, tx_ctrl_dac_current:2;
#else
                    unsigned char tx_ctrl_dac_current:2, tx_ctrl_resitor_internal:1, tx_ctrl_load_resistor:3, decode:1, en_adc_dac:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_57;

            union U_lms6002_regs_58
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char bias_adjust:2, bias_up:2, bias_down:4;
#else
                    unsigned char bias_down:4, bias_up:2, bias_adjust:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_58;

            union U_lms6002_regs_59
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:1, reference_gain_adjust:2, common_mode_adjust:2, reference_buffer_boost:2, adc_input_disable:1;
#else
                    unsigned char adc_input_disable:1, reference_buffer_boost:2, common_mode_adjust:2, reference_gain_adjust:2, not_used:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_59;

            union U_lms6002_regs_5A
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char rx_fsync_polarity:1, rx_interleave:1, dac_clk_polarity:1, tx_fsync_polarity:1, tx_interleave_mode:1, adc_phase_select:1, clock_adjust:2;
#else
                    unsigned char clock_adjust:2, adc_phase_select:1, tx_interleave_mode:1, tx_fsync_polarity:1, dac_clk_polarity:1, rx_interleave:1, rx_fsync_polarity:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_5A;

            union U_lms6002_regs_5B
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char adc_bias_adjust:2, main_bias_down:2, adc_amp1_bias:2, adc_amp2_bias:2;
#else
                    unsigned char adc_amp2_bias:2, adc_amp1_bias:2, main_bias_down:2, adc_bias_adjust:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_5B;

            union U_lms6002_regs_5C
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char adc_amp1_stage2_bias:2, adc_amp2_stage2_bias:2, quantizer_bias:2, input_buffer_bias:2;
#else
                    unsigned char input_buffer_bias:2, quantizer_bias:2, adc_amp2_stage2_bias:2, adc_amp1_stage2_bias:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_5C;

            union U_lms6002_regs_5D
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char bandgap_temp_coefficient_control:4, bandgap_gain_control:4;
#else
                    unsigned char bandgap_gain_control:4, bandgap_temp_coefficient_control:4;
#endif
                }Bit;
                unsigned char raw;
            }Register_5D;

            union U_lms6002_regs_5E
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char reference_amps_bias_adjust:2, reference_amps_bias_up:2, reference_amps_bias_down:4;
#else
                    unsigned char reference_amps_bias_down:4, reference_amps_bias_up:2, reference_amps_bias_adjust:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_5E;

            union U_lms6002_regs_5F
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char spare0:3, enable_dac:1, enable_adc1:1, enable_adc2:1, enable_adc_ref:1, enable_master_ref:1;
#else
                    unsigned char enable_master_ref:1, enable_adc_ref:1, enable_adc2:1, enable_adc1:1, enable_dac:1, spare0:3;
#endif
                }Bit;
                unsigned char raw;
            }Register_5F;

            union U_lms6002_regs_60
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, dc_regval:6;
#else
                    unsigned char dc_regval:6, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_60;

            union U_lms6002_regs_61
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:3, dc_lock:3, dc_calbr_done:1, dc_ud:1;
#else
                    unsigned char dc_ud:1, dc_calbr_done:1, dc_lock:3, not_used:3;
#endif
                }Bit;
                unsigned char raw;
            }Register_61;

            union U_lms6002_regs_62
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, dc_cntval:6;
#else
                    unsigned char dc_cntval:6, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_62;

            union U_lms6002_regs_63
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, dc_start_clbr:1, dc_load:1, dc_sreset:1, dc_addr:3;
#else
                    unsigned char dc_addr:3, dc_sreset:1, dc_load:1, dc_start_clbr:1, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_63;

            union U_lms6002_regs_64
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, rx_vga2_voltage:4, rx_vga2_en:1, decode:1;
#else
                    unsigned char decode:1, rx_vga2_en:1, rx_vga2_voltage:4, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_64;

            union U_lms6002_regs_65
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:3, rx_vga2_gain:5;
#else
                    unsigned char rx_vga2_gain:5, not_used:3;
#endif
                }Bit;
                unsigned char raw;
            }Register_65;

            union U_lms6002_regs_66
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, pd_current_regulator:1, pd_calibration_dac_vga2b:1, pd_calibration_comparator_vga2b:1, pd_calibration_dac_vga2a:1, pd_calibration_comparator_vga2a:1, pd_bandgap:1;
#else
                    unsigned char pd_bandgap:1, pd_calibration_comparator_vga2a:1, pd_calibration_dac_vga2a:1, pd_calibration_comparator_vga2b:1, pd_calibration_dac_vga2b:1, pd_current_regulator:1, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_66;

            union U_lms6002_regs_67
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:4, pd_buffer_vga:1, pd_rx_vga2b:1, pd_rx_vga2a:1, pd_current_reference:1;
#else
                    unsigned char pd_current_reference:1, pd_rx_vga2a:1, pd_rx_vga2b:1, pd_buffer_vga:1, not_used:4;
#endif
                }Bit;
                unsigned char raw;
            }Register_67;

            union U_lms6002_regs_68
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char vga_gain_b:4, vga_gain_a:4;
#else
                    unsigned char vga_gain_a:4, vga_gain_b:4;
#endif
                }Bit;
                unsigned char raw;
            }Register_68;

            unsigned char reserved2[5];

            union U_lms6002_regs_6e
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char pd_vga2_comparator:2, rsvd:6;
#else
                    unsigned char rsvd:6, pd_vga2_comparator:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_6E;

            union U_lms6002_regs_6f
            {
                struct
                {
                    unsigned char rsvd:8;
                }Bit;
                unsigned char raw;
            }Register_6F;


            union U_lms6002_regs_70
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:6, decode:1, rxfe_en:1; //Fixed with LMS6002Dr2 Programming and Calibration Guide
#else
                    unsigned char rxfe_en:1, decode:1, not_used:6; //Fixed with LMS6002Dr2 Programming and Calibration Guide
#endif
                }Bit;
                unsigned char raw;
            }Register_70;

            union U_lms6002_regs_71
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char input_mixer:1, dc_offset_cancel_i:7;
#else
                    unsigned char dc_offset_cancel_i:7, input_mixer:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_71;

            union U_lms6002_regs_72
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char lna_internal_load:1, dc_offset_cancel_q:7;
#else
                    unsigned char dc_offset_cancel_q:7, lna_internal_load:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_72;

            union U_lms6002_regs_73
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char lna_external_load:1, ip2_cancel_i:7;
#else
                    unsigned char ip2_cancel_i:7, lna_external_load:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_73;

            union U_lms6002_regs_74
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:1, ip2_cancel_q:7;
#else
                    unsigned char ip2_cancel_q:7, not_used:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_74;

            union U_lms6002_regs_75
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char lna_gain_mode:2, active_lna:2, cbe_cap:4;
#else
                    unsigned char cbe_cap:4, active_lna:2, lna_gain_mode:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_75;

            union U_lms6002_regs_76
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:1, feedback_resistor:7;
#else
                    unsigned char feedback_resistor:7, not_used:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_76;

            union U_lms6002_regs_77
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:1, feedback_cap:7;
#else
                    unsigned char feedback_cap:7, not_used:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_77;

            union U_lms6002_regs_78
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, lna_load_resistor_ext:6;
#else
                    unsigned char lna_load_resistor_ext:6, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_78;

            union U_lms6002_regs_79
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:2, lna_load_resistor_int:6;
#else
                    unsigned char lna_load_resistor_int:6, not_used:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_79;

            union U_lms6002_regs_7A
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char bias_current_mixer:4, bias_current_lna:4;
#else
                    unsigned char bias_current_lna:4, bias_current_mixer:4;
#endif
                }Bit;
                unsigned char raw;
            }Register_7A;

            union U_lms6002_regs_7B
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char bias_current_rxvga1:4, bias_current_lo:4;
#else
                    unsigned char bias_current_lo:4, bias_current_rxvga1:4;
#endif
                }Bit;
                unsigned char raw;
            }Register_7B;

            union U_lms6002_regs_7C
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char not_used:1, lo_mixer_bias:4, termination_resistor:1, lna3_fine_gain:2;
#else
                    unsigned char lna3_fine_gain:2, termination_resistor:1, lo_mixer_bias:4, not_used:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_7C;


        };
    };
} lms6002_Register_Map;

#endif

