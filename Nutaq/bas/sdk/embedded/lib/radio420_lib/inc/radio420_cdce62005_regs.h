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
 * @file    radio420_cdce62005_regs.h
 * @brief   Ccdce62005 PLL registers definitions
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
#ifndef __CDCE62005_REGS__
#define __CDCE62005_REGS__

#define CDCE62005_OUTPUT_MODE_LVCMOS_ACTIVE 0x0
#define CDCE62005_OUTPUT_MODE_LVCMOS_INVERTED 0x5
#define CDCE62005_OUTPUT_MODE_LVCMOS_LOW 0xf
#define CDCE62005_OUTPUT_MODE_LVCMOS_TRISTATE 0xa
#define CDCE62005_OUTPUT_MODE_LVCMOS_DISABLED 0xb
#define CDCE62005_OUTPUT_MODE_LVPECL 0x20
#define CDCE62005_OUTPUT_MODE_LVDS 0x1A

#define CDCE62005_CLOCK_SEL_FROM_PIN 0x0
#define CDCE62005_CLOCK_SEL_PRI 0x1
#define CDCE62005_CLOCK_SEL_SEC 0x2
#define CDCE62005_CLOCK_SEL_AUTO_PRI_SEC 0x3
#define CDCE62005_CLOCK_SEL_AUX 0x4
#define CDCE62005_CLOCK_SEL_AUTO_PRI_SEC_AUX 0x7
#define CDCE62005_CLOCK_SEL_AUTO_PRI_SEC 0x3

#define CDCE62005_BUFFER_SEL_LVDS 0x0
#define CDCE62005_BUFFER_SEL_LVPECL 0x2
#define CDCE62005_BUFFER_SEL_LVCMOS 0x3

// bitfield are written in GCC order.
typedef struct S_cdce62005_Register_Map
{
    union
    {
        unsigned RegisterArray[9];
        struct 
        {
            union U_cdce62005_regs_0
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned padding:4, output_mode:6, hi_swing:1, divider_enable:1, divider:7, phase_adjust:7, output_mux:2, reserved:2, primary_predivider:2;
#else
                    unsigned primary_predivider:2, reserved:2, output_mux:2, phase_adjust:7, divider:7, divider_enable:1, hi_swing:1, output_mode:6, padding:4;
#endif
                }Bit;
                unsigned raw;
            }Register_0;

            union U_cdce62005_regs_1
            {
                struct
                {
#ifdef _BIG_ENDIAN    
                    unsigned padding:4, output_mode:6, hi_swing:1, divider_enable:1, divider:7, phase_adjust:7, output_mux:2, reserved:2, secondary_predivider:2;
#else
                    unsigned secondary_predivider:2, reserved:2, output_mux:2, phase_adjust:7, divider:7, divider_enable:1, hi_swing:1, output_mode:6, padding:4;
#endif
                }Bit;
                unsigned raw;
            }Register_1;

            union U_cdce62005_regs_2
            {
                struct
                {
#ifdef _BIG_ENDIAN            
                    unsigned padding:4, output_mode:6, hi_swing:1, divider_enable:1, divider:7, phase_adjust:7, output_mux:2, reserved:2, reference_divider:2;
#else
					unsigned reference_divider:2, reserved:2, output_mux:2, phase_adjust:7, divider:7, divider_enable:1, hi_swing:1, output_mode:6, padding:4;
#endif
                }Bit;
                unsigned raw;
            }Register_2;

            union U_cdce62005_regs_03
            {
                struct
                {
#ifdef _BIG_ENDIAN                    
                    unsigned padding:4, output_mode:6, hi_swing:1, divider_enable:1, divider:7, phase_adjust:7, output_mux:2, reserved:3, reference_divider:1;
#else
					unsigned reference_divider:1, reserved:3, output_mux:2, phase_adjust:7, divider:7, divider_enable:1, hi_swing:1, output_mode:6, padding:4;
#endif
                }Bit;
                unsigned raw;
            }Register_3;

            union U_cdce62005_regs_04
            {
                struct
                {
#ifdef _BIG_ENDIAN                    
                    unsigned padding:4, output_mode:6, hi_swing:1, divider_enable:1, divider:7, phase_adjust:7, output_mux:2, reserved2:2, ti_test:1, reserved1:1;
#else
					unsigned reserved1:1, ti_test:1,reserved2:2, output_mux:2, phase_adjust:7, divider:7, divider_enable:1, hi_swing:1, output_mode:6, padding:4;
#endif
                }Bit;
                unsigned raw;
            }Register_4;

            union U_cdce62005_regs_05
            {
                struct
                {
#ifdef _BIG_ENDIAN              
                    unsigned padding:4, digital_lock_en:1, long_lock:1, lock_windows:4, input_divider:8, reserved:2, failsafe:1, secondary_input_biased:1, primary_input_biased:1, primary_input_termination_dis:1, buffer_hysteresis:1, dc_termination_en:1, soft_input_sel:1, clock_select:3, buf_select:2;
#else
					unsigned buf_select:2, clock_select:3, soft_input_sel:1, dc_termination_en:1, buffer_hysteresis:1, primary_input_termination_dis:1, primary_input_biased:1, secondary_input_biased:1, failsafe:1, reserved:2, input_divider:8, lock_windows:4, long_lock:1, digital_lock_en:1, padding:4;
#endif
                }Bit;
                unsigned raw;
            }Register_5;

            union U_cdce62005_regs_06
            {
                struct
                {
#ifdef _BIG_ENDIAN                    
                    unsigned padding:4, pll_calibration:1, external_loop_filter:1, aux_sel:1, aux_en:1, reserved3:1, vco_calibration:1, charge_pump_pulse_width:1, reserved2:1, charge_pump_current:4, divider_bypass:3, secondary_input_termination_dis:1, reserved1:1, feedback_divider:8, prescaler:2, vco_range:1;
#else
					unsigned vco_range:1, prescaler:2, feedback_divider:8, reserved1:1, secondary_input_termination_dis:1, divider_bypass:3, charge_pump_current:4, reserved2:1, charge_pump_pulse_width:1, vco_calibration:1, reserved3:1, aux_en:1, aux_sel:1, external_loop_filter:1, pll_calibration:1, padding:4;
#endif       	
                }Bit;
                unsigned raw;
            }Register_6;

            union U_cdce62005_regs_07
            {
                struct
                {
#ifdef _BIG_ENDIAN                    
                    unsigned padding:4, reserved1:1, eeprom_lock:1, sel_del1:1, textmux2:1, sel_del2:1, textmux1:1, reserved2:1, loopfilter_r3:2, loopfilter_c3:4, loopfilter_c1:5, loopfilter_r2:5, loopfilter_c2:5;
#else                  
					unsigned loopfilter_c2:5, loopfilter_r2:5, loopfilter_c1:5, loopfilter_c3:4, loopfilter_r3:2, reserved2:1, textmux1:1, sel_del2:1, textmux2:1, sel_del1:1, eeprom_lock:1, reserved1:1, padding:4;    
#endif          
                }Bit;
                unsigned raw;
            }Register_7;

            union U_cdce62005_regs_08
            {
                struct
                {
#ifdef _BIG_ENDIAN                                   
                    unsigned padding:4, aux_activity:1, sec_activity:1, pri_activity:1, test_config:4, reserved1:1, calibration_word:6, reserved2:1, version:3, reserved3:1, sync:1, sleep:1, pll_lock:1, cal_word:6;
#else                  
					unsigned cal_word:6, pll_lock:1, sleep:1, sync:1, reserved3:1, version:3, reserved2:1, calibration_word:6, reserved1:1, test_config:4, pri_activity:1, sec_activity:1, aux_activity:1, padding:4;    
#endif          
                }Bit;
                unsigned raw;
            }Register_8;
         };
     };
} cdce62005_Register_Map;

#endif

