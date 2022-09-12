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
 * @file     adac250_types.h
 * @brief    Adac250 module related type definitions.
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup ADAC250
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

#ifndef __ADAC250_TYPES__
#define __ADAC250_TYPES__


typedef enum ADAC250_DEVICES    /**< Onboard ADAC250 configurable SPI devices.*/
{
    ADAC250_PLL = 0,            /**< PLL device */
    ADAC250_DAC = 1,            /**< DAC device */
    ADAC250_ADC = 2,            /**< ADC device */
    ADAC250_TCVCXO = 3,         /**< TCVXO device */
    ADAC250_OVR
}ADAC250_DEVICES;

typedef enum ADAC250_CLOCK_SOURCES		    /**< ADAC250 clock sources.*/
{
    ADAC250_CLOCKIN_10MHZREF,           	/**< 10MHz Internal reference device */ 
    ADAC250_CLOCKIN_EXTERNALREF,    		/**< External reference from clk in connector */ 
    ADAC250_CLOCKIN_EXTERNALCLOCK			/**< External reference from clk in connector */ 
}ADAC250_CLOCK_SOURCES;

typedef struct ADAC250_PLL_PARAM			 /**< ADAC250 PLL parameter structure*/
{
    unsigned Afactor;
    unsigned Bfactor;
    unsigned Rfactor;
    unsigned Pfactor;
    unsigned VCOFreq;
    unsigned pfdFreq;
    unsigned char divisorADCClk;
    unsigned char divisorDACInClk;
    unsigned char divisorClkOutClk;
    unsigned char divisorRsvdClk;
    unsigned char divisorDACDataClk;
} ADAC250_PLL_PARAM;

typedef struct ADAC250_PLL_SLEEP		
{
    union
    {
        struct
        {
#ifdef _BIG_ENDIAN
            unsigned Select_CLK_In:1,
                     CLK1_PD:1,
                     CLK2_PD:1,
                     CLK_To_PLL_PD:1,
                     REF_In_PD:1,
                     CLK_In_PD:1,
                     Unused:2;
#else
			unsigned Unused:2,
                     CLK_In_PD:1,
                     REF_In_PD:1,
                     CLK_To_PLL_PD:1,
                     CLK2_PD:1,
                     CLK1_PD:1,
                     Select_CLK_In:1;
#endif
        }Bit;
        char Byte;
    };
} ADAC250_PLL_SLEEP;

typedef enum ADAC250_PLL_STATUS	/**< ADAC250 PLL Statuses */
{
    ADAC250_PLL_UNLOCKED,		/**< PLL is unlocked */ 
    ADAC250_PLL_LOCKED,			/**< PLL is locked */ 
} ADAC250_PLL_STATUS;

typedef enum ADAC250_ADC_DATAFORMAT	/**< ADAC250 ADC data formats */
{
    ADAC250_ADC_TWOCMP = 2,		/**< 2's complement format */ 
    ADAC250_ADC_OFFSET			/**< offset format */ 
} ADAC250_ADC_DATAFORMAT;

typedef enum ADAC250_ADC_SLEEP_MODE		/**< ADAC250 ADC Sleep modes */
{
    ADAC250_ADC_NORMAL = 0x8,			/**< Normal ADC function */ 
    ADAC250_ADC_OUTB_DISABLED,			/**< Channel B data output disabled */
    ADAC250_ADC_OUTA_DISABLED,			/**< Channel A data output disabled */
    ADAC250_ADC_OUTAB_DISABLED,			/**< Channels A and B data output disabled */
    ADAC250_ADC_GLOBALPOWERDN,			/**< Global powerdown */
    ADAC250_ADC_CHB_STANDBY,			/**< Channel B standby */
    ADAC250_ADC_CHA_STANDBY				/**< Channel A standby */
} ADAC250_ADC_SLEEP_MODE;

typedef enum ADAC250_ADC_CHANNEL		/**< ADC channels */
{
    ADAC250_ADC_CHANNEL_A,				/**< Channel A */ 
    ADAC250_ADC_CHANNEL_B,				/**< Channel B */ 
} ADAC250_ADC_CHANNEL;	

typedef enum ADAC250_CHANNEL_OCT		/**< Channels Offset correction times */
{
    ADAC250_ADC_OCT_256K = 0x0,
    ADAC250_ADC_OCT_512K = 0x1,
    ADAC250_ADC_OCT_1M   = 0x2,
    ADAC250_ADC_OCT_2M   = 0x3,
    ADAC250_ADC_OCT_4M   = 0x4,
    ADAC250_ADC_OCT_8M   = 0x5,
    ADAC250_ADC_OCT_16M  = 0x6,
    ADAC250_ADC_OCT_32M  = 0x7,
    ADAC250_ADC_OCT_64M  = 0x8,
    ADAC250_ADC_OCT_128M = 0x9,
    ADAC250_ADC_OCT_256M = 0xA,
    ADAC250_ADC_OCT_512M = 0xB,
    ADAC250_ADC_OCT_NULL = 0xF
} ADAC250_CHANNEL_OCT;

typedef enum ADAC250_ADC_GAIN			/**< ADC gains */
{
    ADAC250_ADC_0_0dB,					/**< Gain is 0 dB */ 
    ADAC250_ADC_0_5dB,					/**< Gain is 0.5 dB */ 
    ADAC250_ADC_1_0dB,					/**< Gain is 1 dB */ 
    ADAC250_ADC_1_5dB,					/**< Gain is 1.5 dB */ 
    ADAC250_ADC_2_0dB,					/**< Gain is 2 dB */ 
    ADAC250_ADC_2_5dB,					/**< Gain is 2.5 dB */ 
    ADAC250_ADC_3_0dB,					/**< Gain is 3 dB */ 
    ADAC250_ADC_3_5dB,					/**< Gain is 3.5 dB */ 
    ADAC250_ADC_4_0dB,					/**< Gain is 4 dB */ 
    ADAC250_ADC_4_5dB,					/**< Gain is 4.5 dB */ 
    ADAC250_ADC_5_0dB,					/**< Gain is 5 dB */ 
    ADAC250_ADC_5_5dB,					/**< Gain is 5.5 dB */ 
    ADAC250_ADC_6_0dB					/**< Gain is 6 dB */ 
} ADAC250_ADC_GAIN;

typedef enum ADAC250_ADC_STATUSES		/**< ADC statuses */
{
    ADAC250_ADC_NORMAL_STATUS,			/**< Normal status */ 
    ADAC250_ADC_OVERRANGE,				/**< ADC is in overrange */ 
} ADAC250_ADC_STATUSES;

typedef enum ADAC250_DAC_STATUSES		/**< DAC statuses */
{
    ADAC250_DAC_STATUS_PLL_LOCK,		
    ADAC250_DAC_STATUS_DLL_LOCK,		
    ADAC250_DAC_STATUS_DEVICE_ID,		
    ADAC250_DAC_STATUS_VERSION,			
    ADAC250_DAC_STATUS_SELFTST_ERR,		
    ADAC250_DAC_STATUS_FIFO_ERR,		
    ADAC250_DAC_STATUS_PATTERN_ERR		
} ADAC250_DAC_STATUSES;

typedef enum ADAC250_DAC_CMIX_MODE		/**< DAC CMIX modes */
{
    ADAC250_DAC_CMIX_LP = 0,
    ADAC250_DAC_CMIX_HP = 1,
    ADAC250_DAC_CMIX_PosFDac = 2,
    ADAC250_DAC_CMIX_NegFDac = 3,
} ADAC250_DAC_CMIX_MODE;

typedef enum ADAC250_DAC_DATAFORMAT		/**< DAC data formats */
{
    ADAC250_DAC_TWOCMP = 0x2,			/**< 2's complement */ 
    ADAC250_DAC_OFFSET = 0x3,			/**< Offset */ 
} ADAC250_DAC_DATAFORMAT;

typedef enum ADAC250_DAC_INTERPOLATION   /**< DAC interpolation modes */
{
    ADAC250_DAC_INTERPOLATION_NONE = 1,	 /**< No interpolation */ 
    ADAC250_DAC_INTERPOLATION_2X = 2,	 /**< 2x interpolation */ 
    ADAC250_DAC_INTERPOLATION_4X = 4     /**< 4x interpolation */ 
} ADAC250_DAC_INTERPOLATION;

typedef enum ADAC250_DAC_SLEEP			/**< DAC sleep modes */
{
    ADAC250_DAC_SLEEP_NONE,				/**< Normal DAC function */ 
    ADAC250_DAC_SLEEP_A,				/**< Channel A sleep */ 
    ADAC250_DAC_SLEEP_B,				/**< Channel B sleep */ 
    ADAC250_DAC_SLEEP_AB				/**< Channels A and B sleep */ 
} ADAC250_DAC_SLEEP;


typedef enum ADAC250_DAC_CHANNEL		/**< DAC channels */
{
    ADAC250_DAC_CHANNEL_A,				/**< Channel A */ 
    ADAC250_DAC_CHANNEL_B,				/**< Channel B */ 
} ADAC250_DAC_CHANNEL;

typedef enum ADAC250_DAC_SYNC			/**< DAC sync choices */
{
    ADAC250_DAC_SYNC_OFF,				/**< sync is off */
    ADAC250_DAC_SYNC_ON					/**< sync is on */
} ADAC250_DAC_SYNC;

typedef enum ADAC250_DAC_SYNC_SOURCE		/**< DAC sync sources */
{
    ADAC250_DAC_SYNC_SOURCE_FPGA,			/**< source is FPGA */
    ADAC250_DAC_SYNC_SOURCE_DACREG			/**< source is DAC registers */
} ADAC250_DAC_SYNC_SOURCE;

typedef enum ADAC250_CLOCK_INPUT_ID	/**< Clock multiplexer input choices */
{
    ADAC250_CLOCKIN_10MHZ,          /**< Clock input number 0*/
    ADAC250_CLOCKIN_REFIN,          /**< Clock input number 1*/
    ADAC250_CLOCKIN_PLLCLKOUT2,     /**< Clock input number 2*/
    ADAC250_CLOCKIN_FMCCLK3         /**< Clock input number 3*/
} ADAC250_CLOCK_INPUT_ID;

typedef enum ADAC250_CLOCK_OUTPUT_ID /**< Clock multiplexer output choices */
{
    ADAC250_CLOCKOUT_REFOUT,        /**< Clock output number 0*/
    ADAC250_CLOCKOUT_PLLCLK1,       /**< Clock output number 1*/
    ADAC250_CLOCKOUT_PLLREF,        /**< Clock output number 2*/
    ADAC250_CLOCKOUT_FMCCLK1        /**< Clock output number 3*/
} ADAC250_CLOCK_OUTPUT_ID;

#endif
