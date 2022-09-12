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
 * @file       MI125_pca9535_regs.h
 * @brief      IO expander internal registers definitions
 *
 * $Date: 2015/05/15 17:25:28 $
 * $Revision: 1.2 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
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

#ifndef __MI125_PCA9535_REGS__
#define __MI125_PCA9535_REGS__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

// internal device registers definitions
typedef struct MI125_S_pca9535_Register_Map
{
    union
    {
        unsigned char RegisterArray[0x08];
        struct 
        {
            union U_pca9535_regs_00
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char adc_shdn_i:4,spi_bridge_resetn_i:1,adc_clk_src_i:3;
#else
                    unsigned char adc_clk_src_i:3,spi_bridge_resetn_i:1,adc_shdn_i:4;
#endif                    
                }Bit;
                unsigned char raw;
            }Register_00;
            
            union U_pca9535_regs_01
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char reserved:1,buf_clock_pwrn_i:1,trig_out_en_i:1,local_osc_sw_i:1,adc_clk_sout_i:2,adc_clk_load_i:1,adc_clk_config_i:1;
#else
                    unsigned char adc_clk_config_i:1,adc_clk_load_i:1,adc_clk_sout_i:2,local_osc_sw_i:1,trig_out_en_i:1,buf_clock_pwrn_i:1,reserved:1;
#endif                    
                }Bit;
                unsigned char raw;
            }Register_01;

            union U_pca9535_regs_02
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char adc_shdn_o:4,spi_bridge_resetn_o:1,adc_clk_src_o:3;
#else
                    unsigned char adc_clk_src_o:3,spi_bridge_resetn_o:1,adc_shdn_o:4;
#endif                    
                }Bit;
                unsigned char raw;
            }Register_02;
            
            union U_pca9535_regs_03
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char reserved:1,buf_clock_pwrn_o:1,trig_out_en_o:1,local_osc_sw_o:1,adc_clk_sout_o:2,adc_clk_load_o:1,adc_clk_config_o:1;
#else
                    unsigned char adc_clk_config_o:1,adc_clk_load_o:1,adc_clk_sout_o:2,local_osc_sw_o:1,trig_out_en_o:1,buf_clock_pwrn_o:1,reserved:1;
#endif                    
                }Bit;
                unsigned char raw;
            }Register_03;
            
            union U_pca9535_regs_04
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char adc_shdn_p:4,spi_bridge_resetn_p:1,adc_clk_src_p:3;
#else
                    unsigned char adc_clk_src_p:3,spi_bridge_resetn_p:1,adc_shdn_p:4;
#endif                    
                }Bit;
                unsigned char raw;
            }Register_04;
            
            union U_pca9535_regs_05
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char reserved:1,buf_clock_pwrn_p:1,trig_out_en_p:1,local_osc_sw_p:1,adc_clk_sout_p:2,adc_clk_load_p:1,adc_clk_config_p:1;
#else
                    unsigned char adc_clk_config_p:1,adc_clk_load_p:1,adc_clk_sout_p:2,local_osc_sw_p:1,trig_out_en_p:1,buf_clock_pwrn_p:1,reserved:1;
#endif                    
                }Bit;
                unsigned char raw;
            }Register_05;
            
            union U_pca9535_regs_06
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char adc_shdn_c:4,spi_bridge_resetn_c:1,adc_clk_src_c:3;
#else
                    unsigned char adc_clk_src_c:3,spi_bridge_resetn_c:1,adc_shdn_c:4;
#endif                    
                }Bit;
                unsigned char raw;
            }Register_06;
            
            union U_pca9535_regs_07
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char reserved:1,buf_clock_pwrn_c:1,trig_out_en_c:1,local_osc_sw_c:1,adc_clk_sout_c:2,adc_clk_load_c:1,adc_clk_config_c:1;
#else
                    unsigned char adc_clk_config_c:1,adc_clk_load_c:1,adc_clk_sout_c:2,local_osc_sw_c:1,trig_out_en_c:1,buf_clock_pwrn_c:1,reserved:1;
#endif                    
                }Bit;
                unsigned char raw;
            }Register_07;
        };
    };
} MI125_pca9535_Register_Map;

#ifdef __cplusplus
} 
#endif  // extern "C"                        

#endif

