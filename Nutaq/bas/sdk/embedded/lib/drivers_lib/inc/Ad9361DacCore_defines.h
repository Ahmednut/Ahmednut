/***************************************************************************//**
 *   @file   Ad9361DacCore.h
 *   @brief  Header file of DAC Core Driver.
 *   @author DBogdan (dragos.bogdan@analog.com)
********************************************************************************
 * Copyright 2013(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/
#ifndef AD9361_DAC_CORE_DEFINES_H_
#define AD9361_DAC_CORE_DEFINES_H_

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/

/******************************************************************************/
/********************** Macros and Constants Definitions **********************/
/******************************************************************************/
#define DAC_REG_VERSION			0x0000
#define DAC_VERSION(x)			(((x) & 0xffffffff) << 0)
#define VERSION_IS(x,y,z)		((x) << 16 | (y) << 8 | (z))
#define DAC_REG_ID				0x0004
#define DAC_ID(x)				(((x) & 0xffffffff) << 0)
#define DAC_REG_SCRATCH			0x0008
#define DAC_SCRATCH(x)			(((x) & 0xffffffff) << 0)

#define PCORE_VERSION_MAJOR(version)	((version >> 8) & 0xFF)

#define DAC_REG_RSTN			0x0040
#define DAC_RSTN				(1 << 0)
#define DAC_MMCM_RSTN 			(1 << 1)

#define DAC_REG_RATECNTRL		0x004C
#define DAC_RATE(x)				(((x) & 0xFF) << 0)
#define DAC_TO_RATE(x)			(((x) >> 0) & 0xFF)

#define DAC_REG_CNTRL_1			0x0044
#define DAC_ENABLE				(1 << 0) /* v7.0 */
#define DAC_SYNC				(1 << 0) /* v8.0 */

#define DAC_REG_CNTRL_2			0x0048
#define DAC_PAR_TYPE			(1 << 7)
#define DAC_PAR_ENB				(1 << 6)
#define DAC_R1_MODE				(1 << 5)
#define DAC_DATA_FORMAT			(1 << 4)
#define DAC_DATA_SEL(x)			(((x) & 0xF) << 0) /* v7.0 */
#define DAC_TO_DATA_SEL(x)		(((x) >> 0) & 0xF) /* v7.0 */31

#define DAC_REG_DELAY_CNTRL		0x0060	/* <= v8.0 */
#define DAC_DELAY_SEL			(1 << 17)
#define DAC_DELAY_RWN			(1 << 16)
#define DAC_DELAY_ADDRESS(x)	(((x) & 0xFF) << 8)
#define DAC_TO_DELAY_ADDRESS(x)	(((x) >> 8) & 0xFF)
#define DAC_DELAY_WDATA(x)		(((x) & 0x1F) << 0)
#define DAC_TO_DELAY_WDATA(x)	(((x) >> 0) & 0x1F)

#define REG_DAC_COEFFICIENT_CH0      0x00C0

#define REG_DAC_COEFFICIENT_CH1      0x00C4

#define REG_DAC_DATA_DELAY_CH0       0x00C8

#define REG_DAC_DATA_DELAY_CH1       0x00CC


typedef enum Ad9361DacCore_eDataSel {
	DATA_SEL_DDS,
	DATA_SEL_SED,
	DATA_SEL_USER,
	DATA_SEL_ZERO,	/* OUTPUT 0 */
	DATA_SEL_PN7,
	DATA_SEL_PN15,
	DATA_SEL_PN23,
	DATA_SEL_PN31,
	DATA_SEL_LB,	/* loopback data (ADC) */
	DATA_SEL_PNXX,	/* (Device specific) */
} Ad9361DacCore_eDataSel;


struct Ad9361DacCore_stState
{
	enum Ad9361DacCore_eDataSel	cached_datasel[8];
	uint32_t				*dac_clk;
	uint32_t				pcore_version;
	uint32_t				num_dac_channels;
	uint8_t					enable;
	uint8_t					rx2tx2;
};

#define DAC_REG_CHAN_CNTRL_7(c)		(0x0418 + (c) * 0x40) /* v8.0 */


#endif
