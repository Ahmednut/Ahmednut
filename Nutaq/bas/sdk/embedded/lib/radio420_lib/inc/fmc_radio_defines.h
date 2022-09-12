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
 * @file    fmc_radio_defines.h 
 * @brief   Definitions, type and structure for the Radio420X API and for Radio421X.
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup FMCRADIO
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
#ifndef __FMC_RADIO_DEFINES__
#define __FMC_RADIO_DEFINES__


#include "fmc_spi.h"
#include "lms6002_defines.h"
#include "radio420_cdce62005_defines.h"


#define FMC_RADIO_SDR_CORE 0xFCC1                              /**< FPGA core ID for Radio420X */
#define FMC_RADIO_SDR_REVB_REFERENCE_CLOCK ((unsigned)10e6)    /**< Onboard crystal frequency */
#define FMC_RADIO_SDR_REVB_LIME_CLOCK ((unsigned)30e6)         /**< Recommended LMS6002 frequency */
#define FMC_RADIO_SDR_REVC_REFERENCE_CLOCK ((unsigned)30.72e6) /**< Onboard crystal frequency */
#define FMC_RADIO_SDR_REVC_LIME_CLOCK ((unsigned)30.72e6)      /**< Recommended LMS6002 frequency */

#define FMC_RADIO_LTE_COREID 0xFCC10421                   /**< FPGA core ID for Radio421X */
#define FMC_RADIO_LTE_REFERENCE_CLOCK ((unsigned)30.72e6) /**< Onboard crystal frequency */
#define FMC_RADIO_LTE_LIME_CLOCK ((unsigned)30.72e6)      /**< Recommended LMS6002 frequency */

#define FMC_RADIO_SPA_COREID 0xFCC10421                   /**< FPGA core ID for Radio422X */
#define FMC_RADIO_SPA_REFERENCE_CLOCK ((unsigned)30.72e6) /**< Onboard crystal frequency */
#define FMC_RADIO_SPA_LIME_CLOCK ((unsigned)30.72e6)      /**< Recommended LMS6002 frequency */

/****************************************************************************
*
* IOExpander bit definition;
*
****************************************************************************/

#define FMCRADIO_CPLD_PLLSYNC  0x01      /**< Reset Radio420X/Radio421X control */
#define FMCRADIO_CPLD_PLL_EN    0x02     /**< Enable PLL */
#define FMCRADIO_CPLD_PLLREFSEL  0x04    /**< Select between primary and secondary clock */
#define FMCRADIO_CPLD_RXHIBANDSEL 0x08   /**< Select Rx band */
#define FMCRADIO_CPLD_TXLOWBANDSEL 0x10  /**< Select Tx band */
#define FMCRADIO_CPLD_FILTERBANLSEL 0xE0 /**< Bit mask controlling Rx filter banks */

typedef enum E_FMC_RADIO_SDR_BAND /**< FMC Radio420X band */ {
    FMC_RADIO_SDR_LOW_BAND,       /**< 300 to 1500 MHz */
    FMC_RADIO_SDR_HIGH_BAND,      /**< 1500 to 3000 MHz */
} FMC_RADIO_SDR_BAND /**< FMC Radio420X band */;

typedef enum E_FMC_RADIO_LTE_BAND /**< FMC Radio421X band */ {
    FMC_RADIO_LTE_LOW_BAND,  
    FMC_RADIO_LTE_HIGH_BAND, 
	FMC_RADIO_LTE_WIDE_BAND,
} FMC_RADIO_LTE_BAND /**< FMC Radio421X band */;

typedef enum E_FMC_RADIO_DIR /**< FMC Radio42X signal direction */ {
    FMC_RADIO_DIR_RX,
    FMC_RADIO_DIR_TX,
    FMC_RADIO_DIR_RXTX,
} FMC_RADIO_DIR; /**< FMC Radio42X signal direction */

typedef enum E_FMC_RADIO_DATA /**< Data select */ {
    FMC_RADIO_DATA_USER,             /**< User data - bypass the IQ compensation core */
    FMC_RADIO_DATA_USER_CALIBRATED,  /**< User data compensated for IQ gain imbalance and IQ phase imbalance */
    FMC_RADIO_DATA_DDS,              /**< IQ compensation core internal DDS signal - full scale */
    FMC_RADIO_DATA_DDS_HALF,         /**< IQ compensation core internal DDS signal - half scale */
    FMC_RADIO_DATA_NULL,             /**< Null vector - data always equal to zero */
    FMC_RADIO_DATA_DDS_CALIBRATED,   /**< Internal DDS signal compensated for IQ gain imbalance and IQ phase imbalance */
} FMC_RADIO_DATA /**< Data select */;

//BOARD CONTROL
#define FMC_RADIO_LIME_TX_EN (1 << 0)                  /**< Enable FPGA TX section    */
#define FMC_RADIO_LIME_RX_EN (1 << 1)                  /**< Enable FPGA RX section    */
#define FMC_RADIO_LIME_ENABLE (1 << 2)                 /**< Enable Lime Micro LMS6002 */
#define FMC_RADIO_REF_CLK     (1 << 3)                 /**< FPGA control for reference clock */
#define FMC_RADIO_RF          (1 << 4)                 /**< FPGA control for RF       */
#define FMC_RADIO_RX_GAIN     (1 << 5)                 /**< FPGA control for RX gain  */
#define FMC_RADIO_TX_GAIN     (1 << 6)                 /**< FPGA control for TX gain  */
#define FMC_RADIO_PLL_CPLD    (1 << 7)                 /**< FPGA control for PLL/CPLD */
#define FMC_RADIO_2CLKMUXSIN(x) ((x & 0x3) << 8)       /**< Clock switch input port   */
#define FMC_RADIO_2CLKMUXSOUT(x) ((x & 0x3) << 10)     /**< Clock switch output port  */
#define FMC_RADIO_CLK_MUX_LOAD (1 << 12)               /**< Load config to switch     */
#define FMC_RADIO_CLK_MUX_CONFIG (1 << 13)             /**< Latch config to internal register */
#define FMC_RADIO_PLL_LOCK (1 << 14)                   /**< CDCE62005 PLL is locked   */
#define FMC_RADIO_ADC_I_OVR (1 << 15)                  /**< ADC I channel is overange */
#define FMC_RADIO_ADC_Q_OVR (1 << 16)                  /**< ADC Q channel is overange */
#define FMC_RADIO_DAC_I_OVR (1 << 17)                  /**< DAC I channel is overange */
#define FMC_RADIO_DAC_Q_OVR (1 << 18)                  /**< DAC Q channel is overange */
#define FMC_RADIO_ALL_OVR (FMC_RADIO_ADC_I_OVR | FMC_RADIO_ADC_Q_OVR | FMC_RADIO_DAC_I_OVR | FMC_RADIO_DAC_Q_OVR)    /**< Overange bitmask          */
#define FMC_RADIO_PPS (1 << 19)                        /**< PPS in                    */
#define FMC_RADIO_OVR_CLEAR (1 << 20)                  /**< Clear all overrange       */
#define FMC_RADIO_PA_LOW_EN (1 << 21)                  /**< Enable the PA low-band    */
#define FMC_RADIO_PA_HIGH_EN (1 << 22)                 /**< Enable the PA high-band   */
#define FMC_RADIO_WIDE_INPUT_SW(x) ((x & 0x3) << 23)   /**< Control the Wideband input switch */
#define FMC_RADIO_DESIGN_CLK_EN (1 << 25)              /**< Enable design clock       */
#define FMC_RADIO_CORE_RESET_PULSE (1 << 26)           /**< Pulse core reset          */
#define FMC_RADIO_FIFO_RESET (1 << 27)                 /**< ADC and DAC FIFO reset    */

#define FMC_RADIO_CLK_MUX_ALL_BITS  (FMC_RADIO_CLK_MUX_CONFIG | \
                                     FMC_RADIO_CLK_MUX_LOAD | \
                                     FMC_RADIO_2CLKMUXSIN(0x3) | \
                                     FMC_RADIO_2CLKMUXSOUT(0x3) ) /**< Clock switch configuration bit mask */

typedef enum E_FMCRADIO_REV /**< Radio42X revision board */ {
    FMCRADIOREVSDR_A = 0,
    FMCRADIOREVSDR_B = 1,
    FMCRADIOREVSDR_C = 2,
    FMCRADIOREVSDR_D = 3,
    FMCRADIOREVLTE_A = 0x10,
    FMCRADIOREVSPA_A = 0x20,
} FMCRADIO_REV /**< Radio42X revision board */;

typedef enum E_FMCRADIO_DEVICES  /**< Radio420X/Radio421X SPI channel */ {
    FMCRADIO_RF = 0,             /**< LIMEMICRO RF device */
    FMCRADIO_TXGAIN = 1,         /**< TX Gain device */
    FMCRADIO_RXGAIN = 2,         /**< RX Gain device */
    FMCRADIO_PLL = 3,            /**< PLL device */
    FMCRADIO_TCVCXODAC = 4,      /**< TCVXO device */
    FMCRADIO_CPLD = 5,
    FMCRADIO_OVR
} FMCRADIO_DEVICES /**< Radio420X SPI channel */;

typedef enum E_FMCRADIO_DATARATE_REF_30_72MHZ /**< Possible data rate with 30.72 MHz reference frequency and 30.72 MHz LMS6002 reference frequency */ {
    FMCRADIO_DATARATE_REF_30_72MHZ_5DOT76MHZ = 5760000,
    FMCRADIO_DATARATE_REF_30_72MHZ_7DOT68MHZ = 7680000,
    FMCRADIO_DATARATE_REF_30_72MHZ_8DOT64MHZ = 8640000,
    FMCRADIO_DATARATE_REF_30_72MHZ_9DOT60MHZ = 9600000,
    FMCRADIO_DATARATE_REF_30_72MHZ_10DOT24MHZ = 10240000,
    FMCRADIO_DATARATE_REF_30_72MHZ_11DOT52MHZ = 11520000,
    FMCRADIO_DATARATE_REF_30_72MHZ_12DOT80MHZ = 12800000,
    FMCRADIO_DATARATE_REF_30_72MHZ_14DOT40MHZ = 14400000,
    FMCRADIO_DATARATE_REF_30_72MHZ_15DOT36MHZ = 15360000,
    FMCRADIO_DATARATE_REF_30_72MHZ_17DOT28MHZ = 17280000,
    FMCRADIO_DATARATE_REF_30_72MHZ_19DOT20MHZ = 19200000,
    FMCRADIO_DATARATE_REF_30_72MHZ_20DOT48MHZ = 20480000,
    FMCRADIO_DATARATE_REF_30_72MHZ_23DOT04MHZ = 23040000,
    FMCRADIO_DATARATE_REF_30_72MHZ_25DOT60MHZ = 25600000,
    FMCRADIO_DATARATE_REF_30_72MHZ_28DOT80MHZ = 28800000,
    FMCRADIO_DATARATE_REF_30_72MHZ_30DOT72MHZ = 30720000,
    FMCRADIO_DATARATE_REF_30_72MHZ_34DOT56MHZ = 34560000,
    FMCRADIO_DATARATE_REF_30_72MHZ_38DOT40MHZ = 38400000,
    FMCRADIO_DATARATE_REF_30_72MHZ_40DOT96MHZ = 40960000,
    FMCRADIO_DATARATE_REF_30_72MHZ_46DOT08MHZ = 46080000,
    FMCRADIO_DATARATE_REF_30_72MHZ_51DOT20MHZ = 51200000,
    FMCRADIO_DATARATE_REF_30_72MHZ_57DOT60MHZ = 57600000,
    FMCRADIO_DATARATE_REF_30_72MHZ_61DOT44MHZ = 61440000,
    FMCRADIO_DATARATE_REF_30_72MHZ_69DOT12MHZ = 69120000,
    FMCRADIO_DATARATE_REF_30_72MHZ_76DOT80MHZ = 76800000,
    FMCRADIO_DATARATE_REF_30_72MHZ_81DOT92MHZ = 81920000,
} FMCRADIO_DATARATE_REF_30_72MHZ ;

typedef enum E_FMCRADIO_DATARATE_REF_10_MHZ /**< Possible data rate with 10 MHz reference frequency and 30 MHz LMS6002 reference frequency */ {
    FMCRADIO_DATARATE_4DOT5MHZ = 4500000,
    FMCRADIO_DATARATE_6MHZ = 6000000,
    FMCRADIO_DATARATE_7DOT2MHZ = 7200000,
    FMCRADIO_DATARATE_7DOT5MHZ = 7500000,
    FMCRADIO_DATARATE_8MHZ = 8000000,
    FMCRADIO_DATARATE_9MHZ = 9000000,
    FMCRADIO_DATARATE_9DOT6MHZ = 9600000,
    FMCRADIO_DATARATE_10MHZ = 10000000,
    FMCRADIO_DATARATE_10DOT8MHZ = 10800000,
    FMCRADIO_DATARATE_12MHZ = 12000000,
    FMCRADIO_DATARATE_12DOT5MHZ = 12500000,
    FMCRADIO_DATARATE_13DOT5MHZ = 13500000,
    FMCRADIO_DATARATE_14DOT4MHZ = 14400000,
    FMCRADIO_DATARATE_15MHZ = 15000000,
    FMCRADIO_DATARATE_16MHZ = 16000000,
    FMCRADIO_DATARATE_18MHZ = 18000000,
    FMCRADIO_DATARATE_19DOT2MHZ = 19200000,
    FMCRADIO_DATARATE_20MHZ = 20000000,
    FMCRADIO_DATARATE_21DOT6MHZ = 21600000,
    FMCRADIO_DATARATE_22DOT5MHZ = 22500000,
    FMCRADIO_DATARATE_24MHZ = 24000000,
    FMCRADIO_DATARATE_25MHZ = 25000000,
    FMCRADIO_DATARATE_27MHZ = 27000000,
    FMCRADIO_DATARATE_30MHZ = 30000000,
    FMCRADIO_DATARATE_32MHZ = 32000000,
    FMCRADIO_DATARATE_36MHZ = 36000000,
    FMCRADIO_DATARATE_37DOT5MHZ = 37500000,
    FMCRADIO_DATARATE_40MHZ = 40000000,
    FMCRADIO_DATARATE_45MHZ = 45000000,
    FMCRADIO_DATARATE_48MHZ = 48000000,
    FMCRADIO_DATARATE_50MHZ = 50000000,
    FMCRADIO_DATARATE_54MHZ = 54000000,
    FMCRADIO_DATARATE_60MHZ = 60000000,
    FMCRADIO_DATARATE_67DOT5MHZ = 67500000,
    FMCRADIO_DATARATE_72MHZ = 72000000,
    FMCRADIO_DATARATE_75MHZ = 75000000,
    FMCRADIO_DATARATE_80MHZ = 80000000,
} FMCRADIO_DATARATE_REF_10_MHZ ;

typedef enum E_FMCRADIO_CLOCK_INPUT_ID /**< Radio420X/Radio421X clock multiplexer inputs. */
{
    FMCRADIO_CLOCKIN_PLLCLKOUT,       /**< Clock input from CDCE62005 PLL*/
    FMCRADIO_CLOCKIN_EXTCLK,          /**< Clock input from external clock port*/
    FMCRADIO_CLOCKIN_FMCCLK2,         /**< Clock input from FMC CLK2*/
    FMCRADIO_CLOCKIN_FMCCLK3          /**< Clock input from FMC CLK3*/
} FMCRADIO_CLOCK_INPUT_ID /**< Radio420X/Radio421X clock multiplexer inputs. */;

typedef enum E_FMCRADIO_CLOCK_OUTPUT_ID /**< Radio420X/Radio421X clock multiplexer outputs. */
{
    FMCRADIO_CLOCKOUT_PLLIN2,           /**< Clock output going to CDCE62005 PLL*/
    FMCRADIO_CLOCKOUT_NC,               /**< Clock output not connected*/
    FMCRADIO_CLOCKOUT_FMCCLK0,          /**< Clock output going to FMC CLK0*/
    FMCRADIO_CLOCKOUT_FMCCLK1           /**< Clock output going to FMC CLK1*/
} FMCRADIO_CLOCK_OUTPUT_ID /**< Radio420X/Radio421X clock multiplexer outputs. */;

typedef enum E_FMCRADIOLTE_WIDE_INPUT_SWITCH_ID /**< Radio421X wideband input switch. */
{
    FMCRADIOLTE_WIDESWITCH_RF_GROUND,      /**< RF ground */
    FMCRADIOLTE_WIDESWITCH_WIDE_INPUT,     /**< Wideband input */
    FMCRADIOLTE_WIDESWITCH_HIGH_INPUT,     /**< High-band input */
    FMCRADIOLTE_WIDESWITCH_LOW_INPUT       /**< Low-band input */
} FMCRADIOLTE_WIDE_INPUT_SWITCH_ID /**< FMC Radio clock multiplexer outputs. */;

typedef enum E_FMCRADIOSDR_FILTER_BANK /**< Radio420X RF filters */
{
    /* Revision C */
    FMCRADIOSDR_REVC_FILTER_2140MHZ = 0,   /**< Fc = 2140 MHz, BW = 60 MHz */
    FMCRADIOSDR_REVC_FILTER_1950MHZ = 1,   /**< Fc = 1950 MHz, BW = 60 MHz */
    FMCRADIOSDR_REVC_FILTER_BYPASS  = 2,   /**<     No filter, bypass      */
    FMCRADIOSDR_REVC_FILTER_1960MHZ = 3,   /**< Fc = 1960 MHz, BW = 60 MHz */
    FMCRADIOSDR_REVC_FILTER_1880MHZ = 4,   /**< Fc = 1880 MHz, BW = 60 MHz */
    FMCRADIOSDR_REVC_FILTER_2495MHZ = 5,   /**< Fc = 2495 MHz, BW = 390 MHz */
    FMCRADIOSDR_REVC_FILTER_1748MHZ = 6,   /**< Fc = 1748 MHz, BW = 75 MHz */
    FMCRADIOSDR_REVC_FILTER_1843MHZ = 7,   /**< Fc = 1843 MHz, BW = 75 MHz */
    FMCRADIOSDR_REVC_FILTER_882MHZ  = 0,   /**< Fc =  882 MHz, BW = 25 MHz */
    FMCRADIOSDR_REVC_FILTER_837MHZ  = 1,   /**< Fc =  837 MHz, BW = 25 MHz */
    FMCRADIOSDR_REVC_FILTER_943MHZ  = 3,   /**< Fc =  943 MHz, BW = 35 MHz */
    FMCRADIOSDR_REVC_FILTER_898MHZ  = 4,   /**< Fc =  898 MHz, BW = 35 MHz */
    FMCRADIOSDR_REVC_FILTER_915MHZ  = 5,   /**< Fc =  915 MHz, BW = 26 MHz */
    FMCRADIOSDR_REVC_FILTER_782MHZ  = 6,   /**< Fc =  782 MHz, BW = 10 MHz */
    FMCRADIOSDR_REVC_FILTER_751MHZ  = 7,   /**< Fc =  751 MHz, BW = 10 MHz */

    /* Revision B */
    FMCRADIOSDR_REVB_FILTER_2140MHZ = 0,
    FMCRADIOSDR_REVB_FILTER_1950MHZ = 1,
    FMCRADIOSDR_REVB_FILTER_TESTPOINT=2,
    FMCRADIOSDR_REVB_FILTER_1960MHZ = 3,
    FMCRADIOSDR_REVB_FILTER_1880MHZ = 4,
    FMCRADIOSDR_REVB_FILTER_NONE    = 5,
    FMCRADIOSDR_REVB_FILTER_2495MHZ = 6,
    FMCRADIOSDR_REVB_FILTER_3600MHZ = 7,
    FMCRADIOSDR_REVB_FILTER_882MHZ  = 0,
    FMCRADIOSDR_REVB_FILTER_837MHZ  = 1,
    FMCRADIOSDR_REVB_FILTER_943MHZ  = 3,
    FMCRADIOSDR_REVB_FILTER_898MHZ  = 4,
} FMCRADIOSDR_FILTER_BANK /**< Radio420X RF filters */;

typedef enum E_FMCRADIO_FPGA_CTRL_LIST /**< Radio420X FPGA Control list */
{
	FMCRADIO_CTRL_REF_CLOCK = 0, /**< Reference Clock control */
	FMCRADIO_CTRL_RF 		= 1, /**< Radio Frequency control */
	FMCRADIO_CTRL_RX_GAIN 	= 2, /**< RX Gain control */
	FMCRADIO_CTRL_TX_GAIN 	= 3, /**< TX gain control */
	FMCRADIO_CTRL_PLL_CPLD 	= 4, /**< PLL/CPLD control */
} FMCRADIO_FPGA_CTRL_LIST /**< Radio420X FPGA Control list */;

typedef enum E_FMCRADIO_SPI_CTRL /**< Radio420X SPI Control */
{
	FMCRADIO_HOST = 0,  /**< enables Host control of the SPI */
	FMCRADIO_FPGA = 1,  /**< enables FPGA control of the SPI */
} FMCRADIO_SPI_CTRL /**< Radio420X SPI Control */;

typedef enum E_FMCRADIO_CLK_SOURCE /**< Radio420x Clock Source */
{
    FMCRADIO_CLK_SRC_INTERNAL = 0, /**< Internal Clock source */
    FMCRADIO_CLK_SRC_EXTERNAL = 1, /**< External Clock source */
} FMCRADIO_CLK_SOURCE /**< Radio420x Clock Source */;

#endif

