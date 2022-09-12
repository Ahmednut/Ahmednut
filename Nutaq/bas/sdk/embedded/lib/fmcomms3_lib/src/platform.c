/***************************************************************************//**
 *   @file   Platform.c
 *   @brief  Implementation of Platform Driver.
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

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include "xspi.h"

#include "util.h"
#include "adc_core.h"
#include "dac_core.h"
#include "platform.h"
#include "fmcomms3_defines.h"

/******************************************************************************/
/************************ Variables Definitions *******************************/
/******************************************************************************/

/***************************************************************************//**
 * @brief spi_init
*******************************************************************************/
int32_t spi_init(struct spi_device *spi,
                 uint32_t BaseAddr,
				 uint8_t  clk_pha,
				 uint8_t  clk_pol)
{
	uint32_t control_val = 0;

	XSpi_Config ConfigTable =
	{
		0, 			//XPAR_AXI_SPI_0_DEVICE_ID,
		BaseAddr, 	//XPAR_AXI_SPI_0_BASEADDR,
		1, 			//XPAR_AXI_SPI_0_FIFO_EXIST,
		0, 			//XPAR_AXI_SPI_0_SPI_SLAVE_ONLY,
		1, 			//XPAR_AXI_SPI_0_NUM_SS_BITS,
		8, 			//XPAR_AXI_SPI_0_NUM_TRANSFER_BITS,
		0, 			//XPAR_AXI_SPI_0_SPI_MODE
	};
    
    if(spi->spi_instance != NULL) free(spi->spi_instance);
    spi->spi_instance = (void *)malloc(sizeof(XSpi));
    
    if(spi->spi_instance == NULL) return eFmcomms3OutOfMem;

	// Stop SPI
	XSpi_CfgInitialize((XSpi *)spi->spi_instance, &ConfigTable,
			ConfigTable.BaseAddress);
	XSpi_Stop((XSpi *)spi->spi_instance);

	// Start SPI
	XSpi_CfgInitialize((XSpi *)spi->spi_instance, &ConfigTable,
			ConfigTable.BaseAddress);
	control_val = XSP_MASTER_OPTION |
				  XSP_CLK_PHASE_1_OPTION |
				  XSP_MANUAL_SSELECT_OPTION;
	XSpi_SetOptions((XSpi *)spi->spi_instance, control_val);
	XSpi_Start((XSpi *)spi->spi_instance);
	XSpi_IntrGlobalDisable((XSpi *)spi->spi_instance);
	XSpi_SetSlaveSelect((XSpi *)spi->spi_instance, 1);

	return eFmcomms3Ok;
}

int32_t spi_close(struct spi_device *spi)
{
    if(spi->spi_instance != NULL)
    {
        free(spi->spi_instance);
        spi->spi_instance = NULL;
    }
    
    return eFmcomms3Ok;
}

/***************************************************************************//**
 * @brief spi_read
*******************************************************************************/
int32_t spi_read(struct spi_device *spi,
				 uint8_t *data,
				 uint8_t bytes_number)
{
	uint32_t cnt		 = 0;

	uint8_t send_buffer[20];

	for(cnt = 0; cnt < bytes_number; cnt++)
	{
		send_buffer[cnt] = data[cnt];
	}

	XSpi_Transfer((XSpi *)spi->spi_instance, send_buffer, data, bytes_number);

	return eFmcomms3Ok;
}

/***************************************************************************//**
 * @brief spi_write_then_read
*******************************************************************************/
int spi_write_then_read(struct spi_device *spi,
		const unsigned char *txbuf, unsigned n_tx,
		unsigned char *rxbuf, unsigned n_rx)
{
	uint8_t buffer[20] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00};
	uint8_t byte;

	for(byte = 0; byte < n_tx; byte++)
	{
		buffer[byte] = (unsigned char)txbuf[byte];
	}
	spi_read(spi, buffer, n_tx + n_rx);
	for(byte = n_tx; byte < n_tx + n_rx; byte++)
	{
		rxbuf[byte - n_tx] = buffer[byte];
	}

	return eFmcomms3Ok;
}

/***************************************************************************//**
 * @brief axiadc_init
*******************************************************************************/
void axiadc_init(struct ad9361_rf_phy *phy)
{
	adc_init(phy);
	dac_init(phy, DATA_SEL_DDS);
}

/***************************************************************************//**
 * @brief axiadc_read
*******************************************************************************/
unsigned int axiadc_read(struct axiadc_state *st, unsigned long reg)
{
	uint32_t val;

	adc_read(st->phy, reg, &val);

	return val;
}

/***************************************************************************//**
 * @brief axiadc_write
*******************************************************************************/
void axiadc_write(struct axiadc_state *st, unsigned reg, unsigned val)
{
	adc_write(st->phy, reg, val);
}

/***************************************************************************//**
 * @brief axiadc_set_pnsel
*******************************************************************************/
int axiadc_set_pnsel(struct axiadc_state *st, int channel, enum adc_pn_sel sel)
{
	unsigned reg;

	uint32_t version = axiadc_read(st, 0x4000);

	if (PCORE_VERSION_MAJOR(version) > 7) {
		reg = axiadc_read(st, ADI_REG_CHAN_CNTRL_3(channel));
		reg &= ~ADI_ADC_PN_SEL(~0);
		reg |= ADI_ADC_PN_SEL(sel);
		axiadc_write(st, ADI_REG_CHAN_CNTRL_3(channel), reg);
	} else {
		reg = axiadc_read(st, ADI_REG_CHAN_CNTRL(channel));

		if (sel == ADC_PN_CUSTOM) {
			reg |= ADI_PN_SEL;
		} else if (sel == ADC_PN9) {
			reg &= ~ADI_PN23_TYPE;
			reg &= ~ADI_PN_SEL;
		} else {
			reg |= ADI_PN23_TYPE;
			reg &= ~ADI_PN_SEL;
		}

		axiadc_write(st, ADI_REG_CHAN_CNTRL(channel), reg);
	}

	return 0;
}
