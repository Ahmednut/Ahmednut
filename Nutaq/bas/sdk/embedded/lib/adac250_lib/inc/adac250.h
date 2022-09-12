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
 *  @file       : adac250.h
 *  @brief      : ADAC250 core register and functions definitions 
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


#ifndef __ADAC250_H
#define __ADAC250_H

#include "bas_fmc_device_def.h"
#include "bas_spi.h"
#include "adac250_types.h"

/**
*****************************************************************************
* @file adac250.h
*****************************************************************************
* ADAC250 FMC API functions
****************************************************************************/

#define ADC_CTRL_ADC_A_RUN          0x0001 /**< reserved */
#define ADC_CTRL_ADC_B_RUN          0x0002 /**< reserved */
#define ADC_CTRL_ADC_A_START        0x0004 /**< reserved */
#define ADC_CTRL_ADC_B_START        0x0008 /**< reserved */
#define ADC_CTRL_ADC_DATA_TYPE      0x0010 /**< reserved */
#define ADC_CTRL_SPI_RESET          0x0020 /**< reserved */
#define ADC_CTRL_ADC_A_OVR_RANGE    0x0040 /**< reserved */
#define ADC_CTRL_READ_0             0x0080 /**< reserved */
#define ADC_CTRL_ADC_B_OVR_RANGE    0x0100 /**< reserved */
#define DAC_CTRL_DAC_RESET          0x0001 /**< reserved */
#define DAC_CTRL_DAC_DATACLK_LOCKED 0x0002 /**< reserved */
#define MISC_CTRL_PLL_STATUS        0x0001 /**< reserved */
#define MISC_CTRL_PLL_FUNCTION      0x0002 /**< reserved */
#define MISC_CTRL_CLK_MUX_CONFIG    0x0004 /**< reserved */
#define MISC_CTRL_CLK_MUX_LOAD      0x0008 /**< reserved */
#define MISC_CTRL_CLK_MUX_SIN_0     0x0010 /**< reserved */
#define MISC_CTRL_CLK_MUX_SIN_1     0x0020 /**< reserved */
#define MISC_CTRL_CLK_MUX_SOUT_0    0x0040 /**< reserved */
#define MISC_CTRL_CLK_MUX_SOUT_1    0x0080 /**< reserved */
#define MISC_CTRL_UPDATER_BUSY      0x0400 /**< reserved */
#define MISC_CTRL_SPI_BUSY          0x0800 /**< reserved */
#define MISC_CTRL_REQUEST_2         0x1000 /**< reserved */
#define MISC_CTRL_GRANT_2           0x2000 /**< reserved */
#define MISC_CTRL_ACK_2             0x4000 /**< reserved */



#define MISC_CTRL_CLK_MUX_ALL_BITS  (MISC_CTRL_CLK_MUX_CONFIG | \
                                     MISC_CTRL_CLK_MUX_LOAD | \
                                     MISC_CTRL_CLK_MUX_SIN_0 | \
                                     MISC_CTRL_CLK_MUX_SIN_1 | \
                                     MISC_CTRL_CLK_MUX_SOUT_0 | \
                                     MISC_CTRL_CLK_MUX_SOUT_1)    /**< Clock mux configuration bits */   


/**
*****************************************************************************
*
* ADAC250 control register
*
****************************************************************************/
typedef enum ADAC250_CONTROL_REGISTER
{
  ADAC250_COREIDVERS  = 0,          /**< Adac250 CoreID*/
  ADAC250_ADC_CONTROL = 1,          /**< A/D converter control register */
  ADAC250_DAC_CONTROL = 2,          /**< D/A converter control register */
  ADAC250_MISC_CONTROL = 3,         /**< Miscellaneous control register */
  ADAC250_ADCDELAY_CONTROL = 4,     /**< ADC DELAY control register */
  ADAC250_DACDELAY_CONTROL = 5,     /**< DAC DELAY control register */
  ADAC250_FREQCNTCLK_CONTROL = 6    /**< Clock frequency counter control register */
}ADAC250_CONTROL_REGISTER;

typedef struct ADAC250Adaptator /** ADAC250 FPGA core memory map */
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
    unsigned int Register;
  }Core_ID_Vers;

  union
  {
    struct
    {
#ifdef _BIG_ENDIAN
      unsigned Unused:16,
               CoreResetPulse:1,
               TriggerDelay:5,
               AdcResetMmcm:1,
               ADC_B_OverRange:1,
               Unused2:1,
               ADC_A_OverRange:1,
               ADC_SPI_Reset:1,
               ADC_DataType:1,
               ADC_B_Start:1,
               ADC_A_Start:1,
               ADC_B_Run:1,
               ADC_A_Run:1;
#else
      unsigned ADC_A_Run:1,
               ADC_B_Run:1,
               ADC_A_Start:1,
               ADC_B_Start:1,
               ADC_DataType:1,
               ADC_SPI_Reset:1,
               ADC_A_OverRange:1,
               Unused2:1,
               ADC_B_OverRange:1,
               AdcResetMmcm:1,
               TriggerDelay:5,
               CoreResetPulse:1,
               Unused:16;
#endif
    };
    unsigned int Register;
  }ADC_Control; /**< ADAC250 ADC control */

  union
  {
    struct
    {
#ifdef _BIG_ENDIAN
      unsigned Unused:24,
               DAC_MuxSelectB:3,
               DAC_MuxSelectA:3,
               DAC_DataClk_Locked:1,
               DAC_Reset:1;
#else
      unsigned DAC_Reset:1,
               DAC_DataClk_Locked:1,
               DAC_MuxSelectA:3,
               DAC_MuxSelectB:3,
               Unused:24;
#endif
    };
    unsigned int Register;
  }DAC_Control; /**< ADAC250 DAC control */

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
               Unused2:2,
               ClockMux_SOUT:2,
               ClockMux_SIN:2,
               ClockMux_Load:1,
               ClockMux_Config:1,
               PLL_Function:1,
               PLL_Status:1;
#else
      unsigned PLL_Status:1,
               PLL_Function:1,
               ClockMux_Config:1,
               ClockMux_Load:1,
               ClockMux_SIN:2,
               ClockMux_SOUT:2,
               Unused2:2,
               UpdaterBusy:1,
               SPI_busy:1,
               Request2:1,
               Grant2:1,
               Acknowledge2:1,
               Unused:17;
#endif
    };
    unsigned int Register;
  }Misc_Control; /**< ADAC250 other control */

  struct S_SPI_RegisterMap SPIDevice; /**< ADAC250 Spi controller */
  
  union
  {
    struct
    {
#ifdef _BIG_ENDIAN
      unsigned Unused:21,
               AdcPatternError:1,
               AdcClkIdelayValue:5,
               AdcIdelayValue:5;
#else
      unsigned AdcIdelayValue:5,
               AdcClkIdelayValue:5,
               AdcPatternError:1,
               Unused:21;
#endif
    };
    unsigned int Register;
  }AdcDelay_Control; /**< ADAC250 Adc Delay control */
    
  union
  {
    struct
    {
#ifdef _BIG_ENDIAN
      unsigned Unused:22,
               DacClkIdelayValue:5,
               DacIdelayValue:5;
#else
      unsigned DacIdelayValue:5,
               DacClkIdelayValue:5,
               Unused:22;
#endif
    };
    unsigned int Register;
  }DacDelay_Control; /**< ADAC250 Dac Delay control */
    
  union
  {
    struct
    {
#ifdef _BIG_ENDIAN
      unsigned Unused:14,
               FreqCntClkSel:2,
               FreqCntClkCnt:16;
#else
      unsigned FreqCntClkCnt:16,
               FreqCntClkSel:2,
               Unused:14;
#endif
    };
    unsigned int Register;
  }FreqCntClk_Control; /**< ADAC250 clock frequency counter control */
  
} ADAC250Adaptator_t;

/**
*****************************************************************************
*
* ADAC250 device instance
*
****************************************************************************/
typedef struct ADAC250Board
{
    int BaseAdd;                            /**< FPGA core base address */
    FMCBoardDevice Device[4];               /**< FMC adapter*/
    volatile ADAC250Adaptator_t * Adaptor;  /**< ADAC250 adapter */
    SPITransport * SPI_Transport;           /**< SPI controller transport */
} ADAC250Board_t;


#ifdef __cplusplus
    extern "C" {
#endif


/**
*****************************************************************************
* Function: ADAC250_Init
*****************************************************************************
*
* Initializes the ADAC250 driver.
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
adp_result_t  ADAC250_Init (void);

/**
*****************************************************************************
* Function: ADAC250_Open
*****************************************************************************
*
* Opens an instance of the ADAC250, as defined by the core address.
*
* @param[in]   ADAC250BaseAdd
*      Absolute address of the ADAC250 core.
*
* @param[in]   pfUsleep
*      Pointer to a sleep function that sleep during N microsecond.
*      This function is the one that will be used inside the library functions.
*
* @param[out]   ADAC250Board
*      Pointer address to an ADAC250Board_t structure. The function
*      allocates an instance of the structure and returns the address
*      to the location pointed to by the pointer.
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
adp_result_t  ADAC250_Open(unsigned int ADAC250BaseAdd,
                           int (*pfUsleep)(unsigned int usec),
                           ADAC250Board_t ** ADAC250Board);

/**
*****************************************************************************
* Function: ADAC250_WriteSPIRegisters
*****************************************************************************
*
* Writes an array of values in the register of an SPI device on the ADAC250.
*
* @param[in]   ADAC250Board
*      Pointer to an ADAC250Board_t structure describing the device to use.
*
* @param[in]   DeviceId
*      Target SPI device.
*
* @param[in]   BaseRegister
*      Base register address where to write.
*
* @param[in]   Nb
*      Number of registers to write. This value should be equal to or less than
*      the size of the array pointed by Value.
*
* @param[in]   Value
*      Pointer to an array that contains the values to write.
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
adp_result_t  ADAC250_WriteSPIRegisters( ADAC250Board_t * ADAC250Board,
                                    ADAC250_DEVICES DeviceId,
                                    int BaseRegister, int Nb, char * Value);

/**
*****************************************************************************
* Function: ADAC250_ReadSPIRegisters
*****************************************************************************
*
* Reads an array of values from the register of the SPI device onboard an ADAC250.
*
* @param[in]   ADAC250Board
*      Pointer to an ADAC250Board_t structure describing the device to use.
*
* @param[in]   DeviceId
*      Target SPI device.
*
* @param[in]   BaseRegister
*      Base register address from which to read.
*
* @param[in]   Nb
*      Number of registers to read. This value should be equal to or less than
*      the size of the array pointed to by Value.
*
* @param[out]   Value
*      Pointer to an array that contains the values read. The array must
*      be at least as large as the Nb value.
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
adp_result_t  ADAC250_ReadSPIRegisters( ADAC250Board_t * ADAC250Board,
                                    ADAC250_DEVICES DeviceId,
                                    int BaseRegister, int Nb, char * Value);


/**
*****************************************************************************
* Function: ADAC250_WriteControlRegister
*****************************************************************************
*
* Writes a value in a control register.
*
* @param[in]   ADAC250Board
*      Pointer to an ADAC250Board_t structure describing the device to use.
*
* @param[in]   Register
*      Register ID.
*
* @param[in]   RegisterValue
*      Value to write in the register.
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
adp_result_t  ADAC250_WriteControlRegister( ADAC250Board_t* ADAC250Board,
                                         ADAC250_CONTROL_REGISTER Register,
                                         unsigned int RegisterValue);

/**
*****************************************************************************
* Function: ADAC250_ReadControlRegister
*****************************************************************************
*
* Reads a value from a control register.
*
* @param[in]   ADAC250Board
*      Pointer to an ADAC250Board_t structure describing the device to use.
*
* @param[in]   Register
*      Register ID.
*
* @param[out]   RegisterValue
*      Pointer to an integer that contains the read value.
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
adp_result_t  ADAC250_ReadControlRegister( ADAC250Board_t * ADAC250Board,
                                          int Register, unsigned int * RegisterValue);

/**
*****************************************************************************
* Function: ADAC250_Close
*****************************************************************************
*
* Closes an instance of the ADAC250.
*
* @param[in]   ADAC250Board
*      Pointer to an ADAC250Board_t structure describing the device to use.
*
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
adp_result_t ADAC250_Close( ADAC250Board_t * ADAC250Board);

#ifdef __cplusplus
    }
#endif

#endif


