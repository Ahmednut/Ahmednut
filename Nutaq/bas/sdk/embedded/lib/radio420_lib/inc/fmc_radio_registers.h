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
 * @file    fmc_radio_registers.h 
 * @brief   FMC Radio core definitions
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
#ifndef __FMC_RADIO_REGISTERS__
#define __FMC_RADIO_REGISTERS__

typedef struct S_dac_data_control /**<  DAC data control register */ {
#ifdef _BIG_ENDIAN
    unsigned reserved/**< reserved */:5, dac_data_sel/**< DAC data selection */:3, dds_freq /**< Phase increment of the DDS signal */:24;
#else
    unsigned dds_freq /**< Phase increment of the DDS signal */:24, dac_data_sel/**< DAC data selection */:3,  reserved/**< reserved */:5;
#endif
} dac_data_control;

typedef struct S_rx_dc_offset /**< RX path DC offset values for I and Q */ {
#ifdef _BIG_ENDIAN
    signed int reserved0/**< reserved */:4, q_dc_offset/**< DC offset of Q channel*/:12 , reserved1/** reserved */:4, i_dc_offset/**< DC offset of I channel*/:12;
#else
	signed int i_dc_offset/**< DC offset of I channel*/:12, reserved1/** reserved */:4, q_dc_offset/**< DC offset of Q channel*/:12 , reserved0/**< reserved */:4;
#endif
} rx_dc_offset;

typedef struct S_radio_frequency_counter {
#ifdef _BIG_ENDIAN
    unsigned dac_dco_clock:16, fpga_clock:16;
#else
    unsigned fpga_clock:16, dac_dco_clock:16;
#endif
} radio_frequency_counter;

typedef struct S_radio_attenuator {
#ifdef _BIG_ENDIAN
    unsigned rsvd1:10, rx:6, rsvd2:10, tx:6;
#else
    unsigned tx:6, rsvd2:10, rx:6, rsvd1:10;
#endif
} radio_attenuator;

typedef struct S_radio_lo_lock_detect {
#ifdef _BIG_ENDIAN
    unsigned rsvd1:14, rx_lock_valid:1, rx_valid:1, rsvd2:14, tx_lock_valid:1, tx_valid:1;
#else
    unsigned tx_valid:1, tx_lock_valid:1, rsvd2:14, rx_valid:1, rx_lock_valid:1, rsvd1:14;
#endif
} radio_lo_lock_detect;

typedef struct S_err_fct_ctrl_t {
#ifdef _BIG_ENDIAN
    unsigned rsvd1:2, rx_errfct_nb_point:14, rsvd2:13, freq_wen:1, err_done:1, err_start:1;
#else
    unsigned err_start:1, err_done:1, freq_wen:1, rsvd2:13, rx_errfct_nb_point:14, rsvd1:2;
#endif
} err_fct_ctrl_t;

typedef struct s_adc_idelay_ctrl_t {
#ifdef _BIG_ENDIAN
    unsigned rsvd1:22, AdcClkIdelayValue:5, AdcIdelayValue:5;
#else
    unsigned AdcIdelayValue:5, AdcClkIdelayValue:5, rsvd1:22;
#endif
} adc_idelay_ctrl_t;

typedef struct S_fmc_radio_core /**< Radio42X FPGA core */ {
    unsigned coreid;                    /**< FPGA core id */
    unsigned board_control;             /**< Radio420X control signal */
    dac_data_control dac_data_control;  /**< DAC data control */
    unsigned lime_spi_data;             /**< LMS6002 SPI */
    unsigned tx_gain_spi_data;          /**< TX amplifier SPI */
    unsigned rx_gain_spi_data;          /**< RX amplifier SPI */
    unsigned pll_ctrl_spi_data;         /**< CDCE62005 SPI */
    unsigned ref_dac_spi_data;          /**< Reference tuning SPI */
    unsigned spi_ctrl;                  /**< SPI control */
    unsigned tx_gain_imbalance;         /**< IQ gain imbalance correction - 13-bit two's complement*/
    unsigned tx_phase_imbalance;        /**< IQ phase imbalance correction */
    unsigned tx_gain_imbalance_backoff; /**< IQ imbalance backoff - 13-bit two's complement */ 
    unsigned reserved0;                 /**< reserved */ 
    unsigned reserved1;                 /**< reserved */
    rx_dc_offset rx_dc_offset;          /**< RX DC offset value - high word is Q channel - low word is I channel */
    unsigned erf;                       /**< reserved */
    unsigned ext_dac_spi_data;			/**< External DAC SPI data */
    unsigned rf_lo_spi_data;			/**< Local Oscillators SPI data */
    unsigned fpga_version_id;           /**< FPGA build number */
    radio_frequency_counter freq_counter; /**< Clock frequencies */
    radio_attenuator attenuator;        /**< TX and RX RF attenuator */
    unsigned external_dac_reset;        /**< DAC reset signal */
    unsigned rf_lock_detect; 
    unsigned custom_io_ctrl;
    unsigned user_reg0;
    err_fct_ctrl_t err_fct_ctrl;
    unsigned err_fct_dds;
    unsigned err_fct_I;
    unsigned err_fct_Q;
    adc_idelay_ctrl_t adc_idelay_ctrl;
} fmc_radio_core;

typedef struct S_fmc_radio /**< Radio420X/Radio421X SDR handle */ {
    volatile fmc_radio_core * core;     /**< FPGA core */
    spi_mode spimode;                   /**< SPI mode */
    spi_core_t pll;                     /**< SPI controller for CDCE62005 */
    spi_core_t radio;                   /**< SPI controller for LMS6002 */
    spi_core_t txgain;                  /**< SPI controller for TX amplifier */
    spi_core_t rxgain;                  /**< SPI controller for RX amplifier */
    spi_core_t tcvcxo;                  /**< SPI controller for reference tuning */
    spi_core_t iox;                     /**< SPI controller for IO expander */
    lms6002_core lime_core;             /**< LMS6002 handle */
    unsigned cpld_reg_shadow;           /**< CPLD register shadow */
    unsigned rev;                       /**< board revision */
} fmc_radio;

typedef struct S_fmc_radio_param /**< Radio420X/Radio421X parameters */ {
    cdce62005_param pll_param;       /**< CDCE62005 parameters */
    lms6002_pll_param lime_rx_param; /**< LMS6002 parameters for RX PLL */
    lms6002_pll_param lime_tx_param; /**< LMS6002 parameters for TX PLL  */
} fmc_radio_param;

#endif

