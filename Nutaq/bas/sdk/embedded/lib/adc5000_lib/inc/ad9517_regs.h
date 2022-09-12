/****************************************************************************  
 *
 *    ****                              *                                    
 *   ******                            ***                                   
 *   *******                           ****                                  
 *   ********    ****  ****     **** *********    ******* ****    ***********
 *   *********   ****  ****     **** *********  **************  *************
 *   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
 *   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
 *  ****    *********  ****     ****   ****   ****       ****  ****      ****
 *  ****     ********  ****    *****  ****    *****     *****  ****      ****
 *  ****      ******   ***** ******   *****    ****** *******  ****** *******
 *  ****        ****   ************    ******   *************   *************
 *  ****         ***     ****  ****     ****      *****  ****     *****  ****
 *                                                                       ****
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***      
 *
 ************************************************************************//**
 *
 * @file      ad9517_regs.h
 * 
 * @brief     ad9517 registers
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


#ifndef __AD9517_REGS__
#define __AD9517_REGS__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

typedef struct S_ad9517_Register_Map
{
    union
    {
        unsigned char RegisterArray[0x232];
        struct 
        {
            union U_ad9517_regs_000
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char sdo_active_mirror:1, lsb_first_mirror:1, soft_reset_mirror:1, long_instruction_mirror:1, long_instruction:1, soft_reset:1, lsb_first:1, sdo_active:1;
#else   
                    unsigned char sdo_active:1,  lsb_first:1, soft_reset:1,long_instruction:1, long_instruction_mirror:1, soft_reset_mirror:1, lsb_first_mirror:1, sdo_active_mirror:1;

#endif
					}Bit;
                unsigned char raw;
            }Register_00;

            union U_ad9517_regs_001
            {
                struct
                {
                    unsigned char zeroes;
                }Bit;
                unsigned char raw;
            }Register_01;

            union U_ad9517_regs_002
            {
                struct
                {
                    unsigned char reserved;
                }Bit;
                unsigned char raw;
            }Register_002;

            union U_ad9517_regs_003
            {
                struct
                {
                    unsigned char part_id;
                }Bit;
                unsigned char raw;
            }Register_003;

            union U_ad9517_regs_004
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char blank:7, readback:1;
#else
                    unsigned char readback:1, blank:7;
#endif					
                }Bit;
                unsigned char raw;
            }Register_004;

            unsigned char reserved1[11];

            union U_ad9517_regs_010
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char pfd_polarity:1, cp_current:3, cp_mode:2, pll_pd:2;
#else
                    unsigned char pll_pd:2, cp_mode:2, cp_current:3, pfd_polarity:1;
#endif					
                }Bit;
                unsigned char raw;
            }Register_010;

            union U_ad9517_regs_011
            {
                struct
                {
                    unsigned char r_divider_lsb:8;
                }Bit;
                unsigned char raw;
            }Register_011;

            union U_ad9517_regs_012
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:2, r_divider_msb:6;
#else					
                    unsigned char r_divider_msb:6, reserved:2;
#endif					
                }Bit;
                unsigned char raw;
            }Register_012;

            union U_ad9517_regs_013
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:2, a_counter:6;
#else					
                    unsigned char a_counter:6, reserved:2;
#endif					
                }Bit;
                unsigned char raw;
            }Register_013;

            union U_ad9517_regs_014
            {
                struct
                {
                    unsigned char b_counter_lsb:8;
                }Bit;
                unsigned char raw;
            }Register_014;

            union U_ad9517_regs_015
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:3, b_counter_msb:5;
#else						
                    unsigned char b_counter_msb:5, reserved:3;
#endif					
                }Bit;
                unsigned char raw;
            }Register_015;

            union U_ad9517_regs_016
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char cp_half:1, reset_r_counter:1, reset_ab_counter:1, reset_all_counter:1, b_counter_bypass:1, prescaler:3;
#else						
                    unsigned char prescaler:3, b_counter_bypass:1, reset_all_counter:1, reset_ab_counter:1, reset_r_counter:1, cp_half:1;
#endif			
                }Bit;
                unsigned char raw;
            }Register_016;

            union U_ad9517_regs_017
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char status_pin_ctrl:6, antibacklash_pulse_width:2;
#else						
                    unsigned char antibacklash_pulse_width:2, status_pin_ctrl:6;
#endif			
                }Bit;
                unsigned char raw;
            }Register_017;

            union U_ad9517_regs_018
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:1, lock_detect_counter:2, digital_lock_detect_windows:1, digital_lock_detect_disable:1;
#else						
                    unsigned char digital_lock_detect_disable:1, digital_lock_detect_windows:1, lock_detect_counter:2, reserved:1;
#endif		
                }Bit;
                unsigned char raw;
            }Register_018;

            union U_ad9517_regs_019
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char sync_pin_mode:2, r_path_delay:3, n_path_delay:3;
#else						
                    unsigned char n_path_delay:3, r_path_delay:3, sync_pin_mode:2;
#endif	
                }Bit;
                unsigned char raw;
            }Register_019;

            union U_ad9517_regs_01A
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:1, ref_freq_monitor_thresh_BIG_ENDIAN :1, ld_pin_control:6;
#else						
                    unsigned char ld_pin_control:6, ref_freq_monitor_thresh_BIG_ENDIAN :1, reserved:1;
#endif	
                }Bit;
                unsigned char raw;
            }Register_01A;

            union U_ad9517_regs_01B
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char vco_freq_monitor:1, ref2_freq_monitor:1, ref1_freq_monitor:1, ref_mon_control:5;
#else						
                    unsigned char ref_mon_control:5, ref1_freq_monitor:1, ref2_freq_monitor:1, vco_freq_monitor:1;
#endif	
                }Bit;
                unsigned char raw;
            }Register_01B;

            union U_ad9517_regs_01C
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char deglitch_en:1, ref2_select:1, ref_sel_pin_en:1, reserved:2, ref2_en:1, ref1_en:1, differencial_ref:1;
#else						
                    unsigned char differencial_ref:1, ref1_en:1, ref2_en:1, reserved:2, ref_sel_pin_en:1, ref2_select:1, deglitch_en:1;
#endif	
                }Bit;
                unsigned char raw;
            }Register_01C;

            union U_ad9517_regs_01D
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:3, pll_status_readback:1, ld_pin_comparator_en:1, holdover_en1:1, external_holdover_en:1, holdover_en2:1;
#else						
                    unsigned char holdover_en2:1, external_holdoover_en:1, holdover_en1:1, ld_pin_comparator_en:1, pll_status_readback:1, reserved:3;
#endif	
                }Bit;
                unsigned char raw;
            }Register_01D;

            union U_ad9517_regs_01E
            {
                struct
                {
                    unsigned char reserved:8;
                }Bit;
                unsigned char raw;
            }Register_01E;

            union U_ad9517_regs_01F
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:1, vco_cal:1, holdover_state:1, ref2_selected:1, vco_freq_threshold:1, ref2_freq_threshold:1, ref1_freq_threshold:1, digital_lock:1;
#else						
                    unsigned char digital_lock:1, ref1_freq_threshold:1, ref2_freq_threshold:1, vco_freq_threshold:1, ref2_selected:1, holdover_state:1, vco_cal:1, reserved:1;
#endif	
                }Bit;
                unsigned char raw;
            }Register_01F;

            unsigned char reserved2[128];

            union U_ad9517_regs_0A0
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:7, out4_delay_bypass:1;
#else						
                    unsigned char out4_delay_bypass:1, reserved:7;
#endif	
                }Bit;
                unsigned char raw;
            }Register_0A0;

            union U_ad9517_regs_0A1
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:2, out4_ramp_caps:3, out4_ramp_current:2;
#else						
                    unsigned char out4_ramp_current:2, out4_ramp_caps:3, reserved:2;
#endif	
                }Bit;
                unsigned char raw;
            }Register_0A1;

            union U_ad9517_regs_0A2
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:2, out4_delay_fraction:6;
#else						
                    unsigned char out4_delay_fraction:6, reserved:2;
#endif	
                }Bit;
                unsigned char raw;
            }Register_0A2;

            union U_ad9517_regs_0A3
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:2, out5_ramp_caps:3, out5_ramp_current:2;
#else						
                    unsigned char out5_ramp_current:2, out5_ramp_caps:3, reserved:2;
#endif	
                }Bit;
                unsigned char raw;
            }Register_0A3;

            union U_ad9517_regs_0A4
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:2, out5_ramp_caps:3, out5_ramp_current:2;
#else						
                    unsigned char out5_ramp_current:2, out5_ramp_caps:3, reserved:2;
#endif	
                }Bit;
                unsigned char raw;
            }Register_0A4;

            union U_ad9517_regs_0A5
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:2, out5_delay_fraction:6;
#else						
                    unsigned char out5_delay_fraction:6, reserved:2;
#endif	
                }Bit;
                unsigned char raw;
            }Register_0A5;

            union U_ad9517_regs_0A6
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:2, out6_ramp_caps:3, out6_ramp_current:2;
#else						
                    unsigned char out6_ramp_current:2, out6_ramp_caps:3, reserved:2;
#endif	
                }Bit;
                unsigned char raw;
            }Register_0A6;

            union U_ad9517_regs_0A7
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:2, out6_ramp_caps:3, out6_ramp_current:2;
#else						
                    unsigned char out6_ramp_current:2, out6_ramp_caps:3, reserved:2;
#endif	
                }Bit;
                unsigned char raw;
            }Register_0A7;

            union U_ad9517_regs_0A8
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:2, out6_delay_fraction:6;
#else						
                    unsigned char reserved:6, out6_delay_fraction:2;
#endif	
                }Bit;
                unsigned char raw;
            }Register_0A8;

            union U_ad9517_regs_0A9
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:2, out7_ramp_caps:3, out7_ramp_current:2;
#else						
                    unsigned char out7_ramp_current:2, out7_ramp_caps:3, reserved:2;
#endif	
                }Bit;
                unsigned char raw;
            }Register_0A9;

            union U_ad9517_regs_0AA
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:2, out7_ramp_caps:3, out7_ramp_current:2;
#else						
                    unsigned char out7_ramp_current:2, out7_ramp_caps:3, reserved:2;
#endif	
                }Bit;
                unsigned char raw;
            }Register_0AA;

            union U_ad9517_regs_0AB
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:2, out7_delay_fraction:6;
#else						
                    unsigned char reserved:6, out7_delay_fraction:2;
#endif	
                }Bit;
                unsigned char raw;
            }Register_0AB;

            unsigned char reserved3[68];

            union U_ad9517_regs_0F0
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:3, out0_invert:1, out0_differencial_voltage:2, out0_pd:2;
#else						
                    unsigned char out0_pd:2, out0_differencial_voltage:2, out0_invert:1, reserved:3;
#endif	
                }Bit;
                unsigned char raw;
            }Register_0F0;

            union U_ad9517_regs_0F1
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:3, out1_invert:1, out1_differencial_voltage:2, out1_pd:2;
#else						
                    unsigned char out1_pd:2, out1_differencial_voltage:2, out1_invert:1, reserved:3;
#endif	
                }Bit;
                unsigned char raw;
            }Register_0F1;

            unsigned char reserved4[2];

            union U_ad9517_regs_0F4
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:3, out2_invert:1, out2_differencial_voltage:2, out2_pd:2;
#else						
                    unsigned char out2_pd:2, out2_differencial_voltage:2, out2_invert:1, reserved:3;
#endif	
                }Bit;
                unsigned char raw;
            }Register_0F4;

            union U_ad9517_regs_0F5
            {
                struct
                {
#ifdef 	_BIG_ENDIAN 
                    unsigned char reserved:3, out3_invert:1, out3_differencial_voltage:2, out3_pd:2;
#else						
                    unsigned char out3_pd:2, out3_differencial_voltage:2, out3_invert:1, reserved:3;
#endif	
                }Bit;
                unsigned char raw;
            }Register_0F5;

            unsigned char reserved5[74];

            union U_ad9517_regs_140
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char out4_polarity:3, out4_cmos_b_en:1, out4_lvds_or_cmos:1, out4_lvds_current:2, out4_pd:1;
#else						
                    unsigned char out4_pd:1, out4_lvds_current:2, out4_lvds_or_cmos:1, out4_cmos_b_en:1, out4_polarity:3;
#endif	
                }Bit;
                unsigned char raw;
            }Register_140;

            union U_ad9517_regs_141
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char out5_polarity:3, out5_cmos_b_en:1, out5_lvds_or_cmos:1, out5_lvds_current:2, out5_pd:1;
#else						
                    unsigned char out5_pd:1, out5_lvds_current:2, out5_lvds_or_cmos:1, out5_cmos_b_en:1, out5_polarity:3;
#endif	
                }Bit;
                unsigned char raw;
            }Register_141;

            union U_ad9517_regs_142
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char out6_polarity:3, out6_cmos_b_en:1, out6_lvds_or_cmos:1, out6_lvds_current:2, out6_pd:1;
#else						
                    unsigned char out6_pd:1, out6_lvds_current:2, out6_lvds_or_cmos:1, out6_cmos_b_en:1, out6_polarity:3;
#endif	
                }Bit;
                unsigned char raw;
            }Register_142;

            union U_ad9517_regs_143
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char out7_polarity:3, out7_cmos_b_en:1, out7_lvds_or_cmos:1, out7_lvds_current:2, out7_pd:1;
#else						
                    unsigned char out7_pd:1, out7_lvds_current:2, out7_lvds_or_cmos:1, out7_cmos_b_en:1, out7_polarity:3;
#endif	
                }Bit;
                unsigned char raw;
            }Register_143;

            unsigned char reserved6[76];

            union U_ad9517_regs_190
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char divider0_low:4, divider0_high:4;
#else						
                    unsigned char divider0_high:4, divider0_low:4;
#endif	
                }Bit;
                unsigned char raw;
            }Register_190;

            union U_ad9517_regs_191
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char divider0_bypass:1, divider0_nosync:1, divider0_force_high:1, divider0_start_high:1, divider0_phase_offset:4;
#else						
                    unsigned char divider0_phase_offset:4, divider0_start_high:1, divider0_force_high:1, divider0_nosync:1, divider0_bypass:1;
#endif	
                }Bit;
                unsigned char raw;
            }Register_191;

            union U_ad9517_regs_192
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char reserved:6, divider0_direct2output:1, divider0_dccoff_dis:1;
#else						
                    unsigned char divider0_dccoff_dis:1, divider0_direct2output:1, reserved:6;
#endif	
                }Bit;
                unsigned char raw;
            }Register_192;

            unsigned char reserved7[3];

            union U_ad9517_regs_196
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char divider1_low:4, divider1_high:4;
#else						
                    unsigned char divider1_high:4, divider1_low:4;
#endif	
                }Bit;
                unsigned char raw;
            }Register_196;

            union U_ad9517_regs_197
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char divider1_bypass:1, divider1_nosync:1, divider1_force_high:1, divider1_start_high:1, divider1_phase_offset:4;
#else						
                    unsigned char divider1_phase_offset:4, divider1_start_high:1, divider1_force_high:1, divider1_nosync:1, divider1_bypass:1;
#endif	
                }Bit;
                unsigned char raw;
            }Register_197;

            union U_ad9517_regs_198
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char reserved:6, divider1_direct2output:1, divider1_dccoff_dis:1;
#else						
                    unsigned char divider1_dccoff_dis:1, divider1_direct2output:1, reserved:6;
#endif	
                }Bit;
                unsigned char raw;
            }Register_198;

            union U_ad9517_regs_199
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char divider2DOT1_low:4, divider2DOT1_high:4;
#else						
                    unsigned char divider2DOT1_high:4, divider2DOT1_low:4;
#endif	
                }Bit;
                unsigned char raw;
            }Register_199;

            union U_ad9517_regs_19A
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char divider2DOT2_phase_offset:4, divider2DOT1_phase_offset:4;
#else						
                    unsigned char divider2DOT1_phase_offset:4, divider2DOT2_phase_offset:4;
#endif	
                }Bit;
                unsigned char raw;
            }Register_19A;

            union U_ad9517_regs_19B
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char divider2DOT2_low:4, divider2DOT2_high:4;
#else						
                    unsigned char divider2DOT2_high:4, divider2DOT2_low:4;
#endif	
                }Bit;
                unsigned char raw;
            }Register_19B;

            union U_ad9517_regs_19C
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char reserved:2, divider2DOT2_bypass:1, divider2DOT1_bypass:1, divider2_nosync:1, divider2_force_high:1, divider2DOT2_start_high:1, divider2DOT1_start_high:1;
#else						
                    unsigned char divider2DOT1_start_high:1, divider2DOT2_start_high:1, divider2_force_high:1, divider2_nosync:1, divider2DOT1_bypass:1, divider2DOT2_bypass:1, reserved:2;
#endif	
                }Bit;
                unsigned char raw;
            }Register_19C;

            union U_ad9517_regs_19D
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char reserved:7, divider2_dccoff_dis:1;
#else						
                    unsigned char divider2_dccoff_dis:1, reserved:7;
#endif	
                }Bit;
                unsigned char raw;
            }Register_19D;

            union U_ad9517_regs_19E
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char divider3DOT1_low:4, divider3DOT1_high:4;
#else						
                    unsigned char divider3DOT1_high:4, divider3DOT1_low:4;
#endif	
                }Bit;
                unsigned char raw;
            }Register_19E;

            union U_ad9517_regs_19F
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char divider3DOT2_phase_offset:4, divider3DOT1_phase_offset:4;
#else						
                    unsigned char divider3DOT1_phase_offset:4, divider3DOT2_phase_offset:4;
#endif	
                }Bit;
                unsigned char raw;
            }Register_19F;

            union U_ad9517_regs_1A0
            {
                struct
                {
#ifdef 	_BIG_ENDIAN      
                    unsigned char divider3DOT2_low:4, divider3DOT2_high:4;
#else						
                    unsigned char divider3DOT2_high:4, divider3DOT2_low:4;
#endif	
                }Bit;
                unsigned char raw;
            }Register_1A0;

            union U_ad9517_regs_1A1
            {
                struct
                {
#ifdef 	_BIG_ENDIAN                          
                    unsigned char reserved:2, divider3DOT2_bypass:1, divider3DOT1_bypass:1, divider3_nosync:1, divider3_force_high:1, divider3DOT2_start_high:1, divider3DOT1_start_high:1;
#else						             
                    unsigned char divider3DOT1_start_high:1, divider3DOT2_start_high:1, divider3_force_high:1, divider3_nosync:1, divider3DOT1_bypass:1, divider3DOT2_bypass:1, reserved:2;
#endif	
                }Bit;
                unsigned char raw;
            }Register_1A1;

            union U_ad9517_regs_1A2
            {
                struct
                {
#ifdef 	_BIG_ENDIAN                          
                    unsigned char reserved:7, divider3_dccoff:1;
#else						             
                    unsigned char divider3_dccoff:1, reserved:7;
#endif	
                }Bit;
                unsigned char raw;
            }Register_1A2;

            unsigned char reserved8[61];

            union U_ad9517_regs_1E0
            {
                struct
                {
#ifdef 	_BIG_ENDIAN                          
                    unsigned char reserved:5, vco_divider:3;
#else						             
                    unsigned char vco_divider:3, reserved:5;
#endif	
                }Bit;
                unsigned char raw;
            }Register_1E0;

            union U_ad9517_regs_1E1
            {
                struct
                {
#ifdef 	_BIG_ENDIAN                          
                    unsigned char reserved:3, clock_input_pd:1, vco_pd:1, vco_clock_input_pd:1, select_vco:1, vco_divider_bypass:1;
#else						             
                    unsigned char vco_divider_bypass:1, select_vco:1, vco_clock_input_pd:1, vco_pd:1, clock_input_pd:1, reserved:3;
#endif	
                }Bit;
                unsigned char raw;
            }Register_1E1;

            unsigned char reserved9[78];

            union U_ad9517_regs_230
            {
                struct
                {
#ifdef 	_BIG_ENDIAN                          
                    unsigned char reserved:5, sync_pd:1, ref_pd:1, soft_sync:1;
#else						             
                    unsigned char soft_sync:1, ref_pd:1, sync_pd:1, reserved:5;
#endif	
                }Bit;
                unsigned char raw;
            }Register_230;

            union U_ad9517_regs_231
            {
                struct
                {
                    unsigned char reserved:8;
                }Bit;
                unsigned char raw;
            }Register_231;

            union U_ad9517_regs_232
            {
                struct
                {
#ifdef 	_BIG_ENDIAN                          
                    unsigned char reserved:7, commit:1;
#else						             
                    unsigned char commit:1, reserved:7;
#endif	
                }Bit;
                unsigned char raw;
            }Register_232;

        };
    };
} ad9517_Register_Map;

#ifdef __cplusplus
} 
#endif  // extern "C"    

#endif

