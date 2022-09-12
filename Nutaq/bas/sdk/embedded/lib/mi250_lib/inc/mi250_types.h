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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       **** 
 *                                                                        ***       
 * 
 ************************************************************************//**  
 * 
 * @file    mi250_types.h
 * @brief   Onboard MI250 configurable SPI devices.
 *
 * $Date: 2016/01/25 21:50:42 $
 * $Revision: 1.7 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup MI250
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
 

#ifndef __MI250_DEFINED__
#define __MI250_DEFINED__

#include "bas_fmc_device_def.h"
#include "bas_spi.h"
#include "FMCEEprom_def.h"

/****************************************************************************
*
* Onboard MI250 configurable SPI devices.
*
****************************************************************************/

/** @name mi250 PLL parameters
 * 
 *  Holds the MI250 pll parameters to set in PLL chip
 */
typedef struct MI250_PLL_PARAM
{
    unsigned Afactor;						
    unsigned Bfactor;						
    unsigned Rfactor;
    unsigned Pfactor;
    unsigned VCOFreq;
    unsigned pfdFreq;
    unsigned char divisorADCClk;
    unsigned char divisorClkOutClk;
    unsigned char divisorRsvdClk;
} MI250_PLL_PARAM;

typedef struct MI250_PLL_SLEEP
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
				unsigned  Unused:2,
                     CLK_In_PD:1,
                     REF_In_PD:1,
                     CLK_To_PLL_PD:1,
                     CLK2_PD:1,
                     CLK1_PD:1,
                     USelect_CLK_In:1;
#endif
      
        }Bit;
        char Byte;
    };
} MI250_PLL_SLEEP;

/** @name MI250 PLL status types
 * 
 *  Possible PLL statuses
 */
typedef enum MI250_PLL_STATUS
{
    MI250_PLL_UNLOCKED,		///<   PLL is unlocked
    MI250_PLL_LOCKED,		///<   PLL is locked 
} MI250_PLL_STATUS;


/** @name MI250 ADCs armed status types
 * 
 *  Possible ADC armed statuses
 */
typedef enum MI250_ADC_ARMED_STATUS
{
    MI250_NOT_ARMED,		///<   ADCs are not armed
    MI250_ARMED				///<   ADCs are armed
} MI250_ADC_ARMED_STATUS;

/** @name MI250 PLL configuration mode types
 * 
 *  Possible MI250 PLL configuration modes
 */
typedef enum MI250_PLL_CFG_PARAM
{
    MI250_PLL_PHASE_NOISE,	///<  Configure PLL for minimum phase noise
    MI250_PLL_DUTY_50,		///<  Configure PLL for 50% duty cycle
} MI250_PLL_CFG_PARAM;


/** @name MI250 data format types
 * 
 *  Possible MI250 data format types
 */
typedef enum MI250_ADC_DATAFORMAT
{
    MI250_ADC_TWOCMP = 2,	///<  Data format is 2's complement
    MI250_ADC_OFFSET		///<  Data format is offset
} MI250_ADC_DATAFORMAT;		

/** @name MI250 adc sleep mode types
 * 
 *  Possible MI250 adc sleep modes
 */
typedef enum MI250_ADC_SLEEP_MODE
{
    MI250_ADC_NORMAL = 0x8,			///<  Normal operation
    MI250_ADC_OUT2_DISABLED,		///<  Output buffer disabled for channel 1
    MI250_ADC_OUT1_DISABLED,		///<  Output buffer disabled for channel 2
    MI250_ADC_OUT12_DISABLED,		///<  Output buffer disabled for channel 1 and 2
    MI250_ADC_GLOBALPOWERDN,		///<  Global power down
    MI250_ADC_CHB_STANDBY,			///<  Channel B standby
    MI250_ADC_CHA_STANDBY			///<  Channel A standby
} MI250_ADC_SLEEP_MODE;

/** @name MI250 device types
 * 
 *  List of MI250 devices
 */
typedef enum MI250_DEVICES
{
    MI250_ADC_12 = 0,           ///< ADC device with channels 1 and 2
    MI250_ADC_34 = 1,           ///< ADC device with channels 3 and 4
    MI250_ADC_56 = 2,           ///< ADC device with channels 5 and 6
    MI250_ADC_78 = 3,           ///< ADC device with channels 7 and 8
    MI250_PLL = 4               ///< PLL device 
}MI250_DEVICES;

/** @name MI250 channels
 * 
 *  List of MI250 channels per device
 */
typedef enum MI250_ADC_CHANNEL
{
    MI250_ADC_CHANNEL_1,		///< Channel 1
    MI250_ADC_CHANNEL_2,		///< Channel 2
} MI250_ADC_CHANNEL;


/** @name MI250 offset correction times
 * 
 *  List of MI250 channel offset correction times
 */
typedef enum MI250_CHANNEL_OCT
{
    MI250_ADC_OCT_256K = 0x0,
    MI250_ADC_OCT_512K = 0x1,
    MI250_ADC_OCT_1M   = 0x2,
    MI250_ADC_OCT_2M   = 0x3,
    MI250_ADC_OCT_4M   = 0x4,
    MI250_ADC_OCT_8M   = 0x5,
    MI250_ADC_OCT_16M  = 0x6,
    MI250_ADC_OCT_32M  = 0x7,
    MI250_ADC_OCT_64M  = 0x8,
    MI250_ADC_OCT_128M = 0x9,
    MI250_ADC_OCT_256M = 0xA,
    MI250_ADC_OCT_512M = 0xB,
    MI250_ADC_OCT_NULL = 0xF
} MI250_CHANNEL_OCT;

/** @name MI250 gain types
 * 
 *  List of possible MI250 channel gains
 */
typedef enum MI250_ADC_GAIN
{
    MI250_ADC_0_0dB,	///< Gaing is 0dB
    MI250_ADC_0_5dB,	///< Gaing is 0.5dB
    MI250_ADC_1_0dB,	///< Gaing is 1dB
    MI250_ADC_1_5dB,	///< Gaing is 1.5dB
    MI250_ADC_2_0dB,	///< Gaing is 2dB
    MI250_ADC_2_5dB,	///< Gaing is 2.5dB
    MI250_ADC_3_0dB,	///< Gaing is 3dB
    MI250_ADC_3_5dB,	///< Gaing is 3.5dB
    MI250_ADC_4_0dB,	///< Gaing is 4dB
    MI250_ADC_4_5dB,	///< Gaing is 4.5dB
    MI250_ADC_5_0dB,	///< Gaing is 5dB
    MI250_ADC_5_5dB,	///< Gaing is 5.5dB
    MI250_ADC_6_0dB		///< Gaing is 6dB
} MI250_ADC_GAIN;


typedef enum { 
    MI250_MON_INTERNAL_TEMPERATURE,
    MI250_MON_EXTERNAL_TEMPERATURE,
    MI250_MON_VDD,
    MI250_MON_AIN3,
    MI250_MON_AIN4,
    MI250_MON_AIN5,
    MI250_MON_AIN6,
    MI250_MON_AIN7,
    MI250_MON_AIN8,
} MI250_MONITOR_MEAS_ID;

/** @name MI250 clock sources
 * 
 *  List of possible MI250 clock sources
 */
typedef enum E_MI250_CLOCK_SOURCE
{
    MI250_CLOCKIN_100MHZ,         ///< Onboard reference
    MI250_CLOCKIN_EXTERNALREF,    ///< External reference
    MI250_CLOCKIN_EXTERNALCLOCK,  ///< External clock
} MI250_CLOCK_SOURCE;

/****************************************************************************
*
* MI250 control register.
*
****************************************************************************/
typedef enum E_MI250_CONTROL_REGISTER
{
    MI250_COREIDVERS = 0,         /**< ADC control register */
    MI250_CORERESET = 1,
    MI250_ADC_CONTROL = 2,         /**< ADC control register */
    MI250_PLL_CONTROL = 3,
    MI250_SPIREG_CONTROL = 4,        /**< SPIREG control register */
    MI250_ADC_12_CONTROL = 5,      /**< ADC 1 and 2 control register */
    MI250_ADC_34_CONTROL = 6,      /**< ADC 3 and 4 control register */
    MI250_ADC_56_CONTROL = 7,      /**< ADC 5 and 6 control register */
    MI250_ADC_78_CONTROL = 8,      /**< ADC 7 and 8 control register */
    MI250_VOLT_MON_CONTROL = 9,
    MI250_PLL_CLK_FREQ = 10,
    MI250_ADC_1234_CLK_FREQ = 11,
    MI250_ADC_5678_CLK_FREQ = 12,
    MI250_ADC_BURST_SIZE = 13,
    MI250_ADC_NB_BURST = 14,
    MI250_ADC_INTERVAL = 15,
}MI250_CONTROL_REGISTER;

typedef struct s_MI250Adaptor
{

       union
        {
            struct
            {
#ifdef _BIG_ENDIAN
                unsigned CoreID:16,
                         CoreVersion:16;
#else
				unsigned CoreVersion:16,
                         CoreID:16;
#endif
            };
            int Register;
        }Core_ID_Vers;
		
		union
        {
            struct
            {
#ifdef _BIG_ENDIAN
                unsigned Unused:28,
                         ClockMaster:1,
                         FmcConnector:2,
                         CoreReset:1;
#else
				unsigned CoreReset:1,
                         FmcConnector:2,
                         ClockMaster:1,
                         Unused:28;
#endif
            };
            int Register;
        }Core_Reset;
		
    union
        {
            struct
            {
#ifdef _BIG_ENDIAN
                unsigned Unused:22,
                         LeftJustify:1,
                         IoReset:1,
                         TestMode:2,
                         ChArmed:1,
                         Rsvd:3,
                         TriggerToFpga:1,
                         ADC_SPI_Reset:1;
#else
                unsigned ADC_SPI_Reset:1,
                         TriggerToFpga:1,
                         Rsvd:3,
                         ChArmed:1,
                         TestMode:2,
                         IoReset:1,
                         LeftJustify:1,
                         Unused:22;
#endif
            };
            int Register;
        }ADC_Control;
   
   
    union
        {
            struct
            {
#ifdef _BIG_ENDIAN
                unsigned Unused:28,
                         VCO_PwrEn:1,
                         PLL_RefEn:1,
                         PLL_Function:1,
                         PLL_Status:1;
#else
				unsigned PLL_Status:1,
                         PLL_Function:1,
                         PLL_RefEn:1,
                         VCO_PwrEn:1,
                         Unused:28;
#endif
            };
            int Register;
        }Pll_Control;

    union
        {
            struct
            {
#ifdef _BIG_ENDIAN
                unsigned Unused:17,
                         Acknowledge2:1,
                         Grant2:1,
                         Request2:1,
                         SPI_busy:1,
                         UpdaterBusy:1,
                         Unused2:10;
#else
				unsigned Unused2:10,
                         UpdaterBusy:1,
                         SPI_busy:1,
                         Request2:1,
                         Grant2:1,
                         Acknowledge2:1,
                         Unused:17;
#endif
            };
            int Register;
        }SpiReg_Control;

    SPI_RegisterMap SPIDevice;

    union
        {
            struct
            {
#ifdef _BIG_ENDIAN
                unsigned Reserved:8,
						 AdcBitSlip:1,
						 AdcPatternError:2,
						 AdcClkIdelayValue:5,
						 AdcIdelayValue:5;
                         ADC_MmcmRst:1,
                         ADC_MmcmLocked:1,
                         Reserved1:9;
#else
				unsigned Reserved1:9,
                         ADC_MmcmLocked:1,
                         ADC_MmcmRst:1,
						 AdcIdelayValue:5,
						 AdcClkIdelayValue:5,
						 AdcPatternError:2,
						 AdcBitSlip:1,
                         Reserved:8;
#endif
            };
            int Register;
        }ADC_12_Control;

    union
        {
            struct
            {
#ifdef _BIG_ENDIAN
                unsigned Reserved:8,
						 AdcBitSlip:1,
						 AdcPatternError:2,
						 AdcClkIdelayValue:5,
						 AdcIdelayValue:5;
                         ADC_MmcmRst:1,
                         ADC_MmcmLocked:1,
                         Reserved1:9;
#else
				unsigned Reserved1:9,
                         ADC_MmcmLocked:1,
                         ADC_MmcmRst:1,
						 AdcIdelayValue:5,
						 AdcClkIdelayValue:5,
						 AdcPatternError:2,
						 AdcBitSlip:1,
                         Reserved:8;
#endif
            };
            int Register;
        }ADC_34_Control;

    union
        {
            struct
            {
#ifdef _BIG_ENDIAN
                unsigned Reserved:8,
						 AdcBitSlip:1,
						 AdcPatternError:2,
						 AdcClkIdelayValue:5,
						 AdcIdelayValue:5;
                         ADC_MmcmRst:1,
                         ADC_MmcmLocked:1,
                         Reserved1:9;
#else
				unsigned Reserved1:9,
                         ADC_MmcmLocked:1,
                         ADC_MmcmRst:1,
						 AdcIdelayValue:5,
						 AdcClkIdelayValue:5,
						 AdcPatternError:2,
						 AdcBitSlip:1,
                         Reserved:8;
#endif
            };
            int Register;
        }ADC_56_Control;

    union
        {
            struct
            {
#ifdef _BIG_ENDIAN
                unsigned Reserved:8,
						 AdcBitSlip:1,
						 AdcPatternError:2,
						 AdcClkIdelayValue:5,
						 AdcIdelayValue:5;
                         ADC_MmcmRst:1,
                         ADC_MmcmLocked:1,
                         Reserved1:9;
#else
				unsigned Reserved1:9,
                         ADC_MmcmLocked:1,
                         ADC_MmcmRst:1,
						 AdcIdelayValue:5,
						 AdcClkIdelayValue:5,
						 AdcPatternError:2,
						 AdcBitSlip:1,
                         Reserved:8;
#endif
            };
            int Register;
        }ADC_78_Control;

    union
        {
            struct
            {
#ifdef _BIG_ENDIAN
                unsigned Unused:31,
                         monInterrupt:1;
#else
				unsigned monInterrupt:1,
                         Unused:31;
#endif
            };
            int Register;
        }VoltMon_Control;

    union
        {
            struct
            {
#ifdef _BIG_ENDIAN
                unsigned PLLClockFreq:16,      // Clock frequency in MHz.
						 ExternalClockFreq:16; // Clock frequency in MHz.
#else
				unsigned ExternalClockFreq:16, // Clock frequency in MHz.
						 PLLClockFreq:16;      // Clock frequency in MHz.                
#endif
            };
            int Register;
        }Pll_ClockFreq;

    union
        {
            struct
            {
#ifdef _BIG_ENDIAN
                unsigned ADC_34_ClockFreq:16, // Clock frequency in MHz.
						 ADC_12_ClockFreq:16; // Clock frequency in MHz.
#else
				unsigned ADC_12_ClockFreq:16, // Clock frequency in MHz.
						 ADC_34_ClockFreq:16; // Clock frequency in MHz.                
#endif
            };
            int Register;
        }ADC_1234_ClkFreq;

    union
        {
            struct
            {
#ifdef _BIG_ENDIAN
                unsigned ADC_78_ClockFreq:16, // Clock frequency in MHz.
						 ADC_56_ClockFreq:16; // Clock frequency in MHz.
#else
				unsigned ADC_56_ClockFreq:16, // Clock frequency in MHz.
						 ADC_78_ClockFreq:16; // Clock frequency in MHz.                
#endif
            };
            int Register;
        }ADC_5678_ClkFreq;

    union
        {
            struct
            {
#ifdef _BIG_ENDIAN
                unsigned BurstSize:32;
#else
				unsigned BurstSize:32;
#endif
            };
            int Register;
        }ADC_BurstSize;

    union
        {
            struct
            {
#ifdef _BIG_ENDIAN
                unsigned NbBurst:32;
#else
				unsigned NbBurst:32;
#endif
            };
            int Register;
        }ADC_NbBurst;

    union
        {
            struct
            {
#ifdef _BIG_ENDIAN
                unsigned Interval:32;
#else
				unsigned Interval:32;
#endif
            };
            int Register;
        }ADC_Interval;

} MI250Adaptator_t;



typedef struct sCalibResult
{
    char cRefIdelayMiddle;
    char cRefIdelayWidth;
    char cRefBitSlip;
    char cSucceeded;
    unsigned short usRefLaneDelay; //ps
} sCalibResult_t;

/****************************************************************************
*
* MI250 device instance.
*
****************************************************************************/
typedef struct s_MI250Board
{
    int BaseAdd;
    FMCBoardDevice Device[5];               /**< FMC adapter*/
    volatile MI250Adaptator_t * Adaptor;
    SPITransport * SPI_Transport;
    int (*pfUsleep)(unsigned int usec);
    sCalibResult_t CalibResult;
    sCalibResult_t * pMasterCalibResult;
    FMCEEPROM_HPC_DATA_Specs * pLaneDelay;
} MI250Board_t;

#endif
