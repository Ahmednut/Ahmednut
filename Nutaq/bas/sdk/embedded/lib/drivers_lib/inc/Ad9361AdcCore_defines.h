/***************************************************************************//**
 *   @file   Ad9361AdcCore_defines.h
 *   @brief  Header file of ADC Core Driver.
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
#ifndef AD9361_ADC_CORE_DEFINES_H_
#define AD9361_ADC_CORE_DEFINES_H_

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/

/******************************************************************************/
/********************** Macros and Constants Definitions **********************/
/******************************************************************************/
/* ADC COMMON */
#define ADC_REG_RSTN			0x0040
#define ADC_RSTN				(1 << 0)
#define ADC_MMCM_RSTN 			(1 << 1)

#define ADC_REG_CNTRL			0x0044
#define ADC_R1_MODE				(1 << 2)
#define ADC_DDR_EDGESEL			(1 << 1)
#define ADC_PIN_MODE			(1 << 0)

#define ADC_REG_STATUS			0x005C
#define ADC_MUX_PN_ERR			(1 << 3)
#define ADC_MUX_PN_OOS			(1 << 2)
#define ADC_MUX_OVER_RANGE		(1 << 1)
#define ADC_STATUS				(1 << 0)

#define ADC_REG_DELAY_CNTRL		0x0060	/* <= v8.0 */
#define ADC_DELAY_SEL			(1 << 17)
#define ADC_DELAY_RWN			(1 << 16)
#define ADC_DELAY_ADDRESS(x)	(((x) & 0xFF) << 8)
#define ADC_TO_DELAY_ADDRESS(x)	(((x) >> 8) & 0xFF)
#define ADC_DELAY_WDATA(x)		(((x) & 0x1F) << 0)
#define ADC_TO_DELAY_WDATA(x)	(((x) >> 0) & 0x1F)

#define ADC_REG_DMA_CNTRL		0x0080
#define ADC_DMA_STREAM			(1 << 1)
#define ADC_DMA_START			(1 << 0)

#define ADC_REG_DMA_COUNT		0x0084
#define ADC_DMA_COUNT(x)		(((x) & 0xFFFFFFFF) << 0)
#define ADC_TO_DMA_COUNT(x)		(((x) >> 0) & 0xFFFFFFFF)

#define ADC_REG_DMA_STATUS		0x0088
#define ADC_DMA_OVF				(1 << 2)
#define ADC_DMA_UNF				(1 << 1)
#define ADC_DMA_STATUS			(1 << 0)

#define ADC_REG_DMA_BUSWIDTH	0x008C
#define ADC_DMA_BUSWIDTH(x)		(((x) & 0xFFFFFFFF) << 0)
#define ADC_TO_DMA_BUSWIDTH(x)	(((x) >> 0) & 0xFFFFFFFF)

/* ADC CHANNEL */
#define ADC_REG_CHAN_CNTRL(c)	(0x0400 + (c) * 0x40)
#define ADC_LB_EN				(1 << 11)
#define ADC_PN_SEL				(1 << 10)
#define ADC_IQCOR_ENB			(1 << 9)
#define ADC_DCFILT_ENB			(1 << 8)
#define ADC_FORMAT_SIGNEXT		(1 << 6)
#define ADC_FORMAT_TYPE			(1 << 5)
#define ADC_FORMAT_ENABLE		(1 << 4)
#define ADC_PN23_TYPE			(1 << 1)
#define ADC_ENABLE				(1 << 0)

#define ADC_REG_CHAN_STATUS(c)	(0x0404 + (c) * 0x40)
#define ADC_PN_ERR				(1 << 2)
#define ADC_PN_OOS				(1 << 1)
#define ADC_OVER_RANGE			(1 << 0)

#define ADC_REG_CHAN_CNTRL_1(c)		(0x0410 + (c) * 0x40)
#define ADC_DCFILT_OFFSET(x)		(((x) & 0xFFFF) << 16)
#define ADC_TO_DCFILT_OFFSET(x)		(((x) >> 16) & 0xFFFF)
#define ADC_DCFILT_COEFF(x)			(((x) & 0xFFFF) << 0)
#define ADC_TO_DCFILT_COEFF(x)		(((x) >> 0) & 0xFFFF)

#define ADC_REG_CHAN_CNTRL_2(c)		(0x0414 + (c) * 0x40)
#define ADC_IQCOR_COEFF_1(x)		(((x) & 0xFFFF) << 16)
#define ADC_TO_IQCOR_COEFF_1(x)		(((x) >> 16) & 0xFFFF)
#define ADC_IQCOR_COEFF_2(x)		(((x) & 0xFFFF) << 0)
#define ADC_TO_IQCOR_COEFF_2(x)		(((x) >> 0) & 0xFFFF)

#define ADC_REG_CHAN_CNTRL_3(c)		(0x0418 + (c) * 0x40) /* v8.0 */
#define ADC_ADC_PN_SEL(x)			(((x) & 0xF) << 16)
#define ADC_TO_ADC_PN_SEL(x)		(((x) >> 16) & 0xF)
#define ADC_ADC_DATA_SEL(x)			(((x) & 0xF) << 0)
#define ADC_TO_ADC_DATA_SEL(x)		(((x) >> 0) & 0xF)

#define REG_DESIGN_CLK_EN           0x00C0
#define DESIGN_CLK_EN		        (1 << 0)

#define REG_SYNC_DELAY              0x00C4
#define REFCLK_IODELAY_OFFSET(x)    (((x) & 0x1F) << 0)
#define SYNC_ODELAY_OFFSET(x)       (((x) & 0x1F) << 5)
#define SYNC_IDELAY_OFFSET(x)       (((x) & 0x1F) << 10)

#define REG_SOFT_SYNC               0x00C8
#define SOFT_SYNC		            (1 << 0)

#define REG_SYNC_SEL                0x00CC
#define SYNC_SEL_OFFSET(x)          (((x) & 0xF) << 0)
#define SYNC_BIT_SLIP		        (1 << 4)

#define REG_BIT_SLIP                0x00D0
#define RX_BIT_SLIP                 (1 << 0)
#define TX_BIT_SLIP                 (1 << 1)

#define REG_ADC_COEFFICIENT_CH0      0x00D4

#define REG_ADC_COEFFICIENT_CH1      0x00D8

#define REG_ADC_DATA_DELAY_CH0       0x00DC

#define REG_ADC_DATA_DELAY_CH1       0x00E0


enum Ad9361AdcCore_ePnSel {
	ADC_PN9 = 0,
	ADC_PN23A = 1,
	ADC_PN7 = 4,
	ADC_PN15 = 5,
	ADC_PN23 = 6,
	ADC_PN31 = 7,
	ADC_PN_CUSTOM = 9,
	ADC_PN_END = 10,
};


#endif
