/***************************************************************************//**
 *   @file   ad9361_api.h
 *   @brief  Header file of AD9361 API Driver.
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
#ifndef FMCOMMS3_H_
#define FMCOMMS3_H_

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include "util.h"
#include "ad9361_api.h"
#include "adc_core.h"
#include "bas_error.h"
#include "fmcomms3_defines.h"

/******************************************************************************/
/************************ Functions Declarations ******************************/
/******************************************************************************/
/* Allocate memory and initialize the FMCOMMS3 card. */
Result Fmcomms3_open (Fmcomms3_Handle_t * phFmcomms3,
    uint32_t uiAd9361BaseAddr, uint32_t uiSpiBaseAddr,
    int (*pfUsleep)(unsigned int usec));
Result Fmcomms3_init (Fmcomms3_Handle_t hFmcomms3, AD9361_InitParam *init_param);
    
Result Fmcomms3_digital_calibration(Fmcomms3_Handle_t hFmcomms3);
    
/* Deallocate memory */
void Fmcomms3_close(Fmcomms3_Handle_t hFmcomms3);

Result Fmcomms3_set_sync_delay(Fmcomms3_Handle_t hFmcomms3, int32_t delay);
Result Fmcomms3_set_refclk_delay(Fmcomms3_Handle_t hFmcomms3, int32_t delay);
Result Fmcomms3_set_sync_bitslip(Fmcomms3_Handle_t hFmcomms3, int32_t bitslip);
Result Fmcomms3_set_sync_src(Fmcomms3_Handle_t hFmcomms3, int32_t src);
Result Fmcomms3_soft_sync(Fmcomms3_Handle_t hFmcomms3);
Result Fmcomms3_mcs(Fmcomms3_Handle_t hFmcomms3, int32_t step);
Result Fmcomms3_set_master_calib_result(Fmcomms3_Handle_t hFmcomms3, Fmcomms3_Handle_t hMasterFmcomms3);
Result Fmcomms3_set_ext_master_calib_result(Fmcomms3_Handle_t hFmcomms3, sAd9361CalibResult_t ExtCalibResult);
Result Fmcomms3_get_calib_result(Fmcomms3_Handle_t hFmcomms3, sAd9361CalibResult_t ** ppCalibResult);

/* Set the Enable State Machine (ENSM) mode. */
Result Fmcomms3_set_en_state_machine_mode (Fmcomms3_Handle_t hFmcomms3, uint32_t mode);
/* Get the Enable State Machine (ENSM) mode. */
Result Fmcomms3_get_en_state_machine_mode (Fmcomms3_Handle_t hFmcomms3, uint32_t *mode);
/* Set the TX and RX sampling frequency. */
Result Fmcomms3_set_sampling_freq (Fmcomms3_Handle_t hFmcomms3, uint32_t sampling_freq_hz);
/* Get current TX and RX sampling frequency. */
Result Fmcomms3_get_sampling_freq (Fmcomms3_Handle_t hFmcomms3, uint32_t *sampling_freq_hz);
/* Set the TX LO frequency. */
Result Fmcomms3_set_tx_lo_freq (Fmcomms3_Handle_t hFmcomms3, uint64_t lo_freq_hz);
/* Get current TX LO frequency. */
Result Fmcomms3_get_tx_lo_freq (Fmcomms3_Handle_t hFmcomms3, uint64_t *lo_freq_hz);

/* Set the receive RF gain for the selected channel. */
Result Fmcomms3_set_rx_rf_gain (Fmcomms3_Handle_t hFmcomms3, uint8_t ch, int32_t gain_db);
/* Get current receive RF gain for the selected channel. */
Result Fmcomms3_get_rx_rf_gain (Fmcomms3_Handle_t hFmcomms3, uint8_t ch, int32_t *gain_db);
/* Set the RX RF bandwidth. */
Result Fmcomms3_set_rx_rf_bandwidth (Fmcomms3_Handle_t hFmcomms3, uint32_t bandwidth_hz);
/* Get the RX RF bandwidth. */
Result Fmcomms3_get_rx_rf_bandwidth (Fmcomms3_Handle_t hFmcomms3, uint32_t *bandwidth_hz);
/* Set the RX LO frequency. */
Result Fmcomms3_set_rx_lo_freq (Fmcomms3_Handle_t hFmcomms3, uint64_t lo_freq_hz);
/* Get current RX LO frequency. */
Result Fmcomms3_get_rx_lo_freq (Fmcomms3_Handle_t hFmcomms3, uint64_t *lo_freq_hz);
/* Get the RSSI for the selected channel. */
Result Fmcomms3_get_rx_rssi (Fmcomms3_Handle_t hFmcomms3, uint8_t ch, struct rf_rssi *rssi);
/* Set the gain control mode for the selected channel. */
Result Fmcomms3_set_rx_gain_control_mode (Fmcomms3_Handle_t hFmcomms3, uint8_t ch, uint8_t gc_mode);
/* Get the gain control mode for the selected channel. */
Result Fmcomms3_get_rx_gain_control_mode (Fmcomms3_Handle_t hFmcomms3, uint8_t ch, uint8_t *gc_mode);
/* Set the RX FIR filter configuration. */
Result Fmcomms3_set_rx_fir_config (Fmcomms3_Handle_t hFmcomms3, AD9361_RXFIRConfig fir_cfg);
/* Get the RX FIR filter configuration. */
Result Fmcomms3_get_rx_fir_config(Fmcomms3_Handle_t hFmcomms3, uint8_t rx_ch, AD9361_RXFIRConfig *fir_cfg);
/* Enable/disable the RX FIR filter. */
Result Fmcomms3_set_rx_fir_en_dis (Fmcomms3_Handle_t hFmcomms3, uint8_t en_dis);
/* Get the status of the RX FIR filter. */
Result Fmcomms3_get_rx_fir_en_dis (Fmcomms3_Handle_t hFmcomms3, uint8_t *en_dis);
/* Enable/disable the RX RFDC Tracking. */
Result Fmcomms3_set_rx_rfdc_track_en_dis (Fmcomms3_Handle_t hFmcomms3, uint8_t en_dis);
/* Get the status of the RX RFDC Tracking. */
Result Fmcomms3_get_rx_rfdc_track_en_dis (Fmcomms3_Handle_t hFmcomms3, uint8_t *en_dis);
/* Enable/disable the RX BasebandDC Tracking. */
Result Fmcomms3_set_rx_bbdc_track_en_dis (Fmcomms3_Handle_t hFmcomms3, uint8_t en_dis);
/* Get the status of the RX BasebandDC Tracking. */
Result Fmcomms3_get_rx_bbdc_track_en_dis (Fmcomms3_Handle_t hFmcomms3, uint8_t *en_dis);
/* Enable/disable the RX Quadrature Tracking. */
Result Fmcomms3_set_rx_quad_track_en_dis (Fmcomms3_Handle_t hFmcomms3, uint8_t en_dis);
/* Get the status of the RX Quadrature Tracking. */
Result Fmcomms3_get_rx_quad_track_en_dis (Fmcomms3_Handle_t hFmcomms3, uint8_t *en_dis);
/* Set the transmit attenuation for the selected channel. */

Result Fmcomms3_set_tx_attenuation (Fmcomms3_Handle_t hFmcomms3, uint8_t ch, uint32_t attenuation_mdb);
/* Get current transmit attenuation for the selected channel. */
Result Fmcomms3_get_tx_attenuation (Fmcomms3_Handle_t hFmcomms3, uint8_t ch, uint32_t *attenuation_mdb);
/* Set the TX RF bandwidth. */
Result Fmcomms3_set_tx_rf_bandwidth (Fmcomms3_Handle_t hFmcomms3, uint32_t  bandwidth_hz);
/* Get the TX RF bandwidth. */
Result Fmcomms3_get_tx_rf_bandwidth (Fmcomms3_Handle_t hFmcomms3, uint32_t *bandwidth_hz);

/* Set the TX FIR filter configuration. */
Result Fmcomms3_set_tx_fir_config (Fmcomms3_Handle_t hFmcomms3, AD9361_TXFIRConfig fir_cfg);
/* Get the TX FIR filter configuration. */
Result Fmcomms3_get_tx_fir_config(Fmcomms3_Handle_t hFmcomms3, uint8_t tx_ch, AD9361_TXFIRConfig *fir_cfg);
/* Enable/disable the TX FIR filter. */
Result Fmcomms3_set_tx_fir_en_dis (Fmcomms3_Handle_t hFmcomms3, uint8_t en_dis);
/* Get the status of the TX FIR filter. */
Result Fmcomms3_get_tx_fir_en_dis (Fmcomms3_Handle_t hFmcomms3, uint8_t *en_dis);

/* Set the RX and TX path rates. */
Result Fmcomms3_set_trx_path_clks(Fmcomms3_Handle_t hFmcomms3, uint32_t *rx_path_clks, uint32_t *tx_path_clks);
/* Get the RX and TX path rates. */
Result Fmcomms3_get_trx_path_clks(Fmcomms3_Handle_t hFmcomms3, uint32_t *rx_path_clks, uint32_t *tx_path_clks);

/* Read SPI register of the AD9361 chip */
Result Fmcomms3_spi_read(Fmcomms3_Handle_t hFmcomms3, uint32_t reg, uint8_t * val);
/* Write SPI register of the AD9361 chip */
Result Fmcomms3_spi_write(Fmcomms3_Handle_t hFmcomms3, uint32_t reg, uint8_t val);

/* Sets data selection for the DAC. Only DATA_SEL_USER and DATA_SEL_DDS are allowed.*/
Result Fmcomms3_set_dac_datasel(Fmcomms3_Handle_t hFmcomms3, dds_data_select_e sel);
/* Gets current data selection for the DAC. */
Result Fmcomms3_get_dac_datasel(Fmcomms3_Handle_t hFmcomms3, dds_data_select_e * sel);

/* Gets current DDS frequency [Hz]. */
Result Fmcomms3_get_dds_freq(Fmcomms3_Handle_t hFmcomms3, uint8_t ch, uint8_t tone, uint32_t * freq);
/* Sets the DDS frequency [Hz]. */
Result Fmcomms3_set_dds_freq(Fmcomms3_Handle_t hFmcomms3, uint8_t ch, uint8_t tone, uint32_t freq);
/* Gets current DDS phase [mDegrees]. */
Result Fmcomms3_get_dds_phase(Fmcomms3_Handle_t hFmcomms3, uint8_t ch, uint8_t tone, uint32_t * phase);
/* Sets the DDS phase [mDegrees]. */
Result Fmcomms3_set_dds_phase(Fmcomms3_Handle_t hFmcomms3, uint8_t ch, uint8_t tone, uint32_t phase);
/* Gets current DDS scale in micro unit. */
Result Fmcomms3_get_dds_scale(Fmcomms3_Handle_t hFmcomms3, uint8_t ch, uint8_t tone, int32_t * scale);
/* Sets the DDS scale in micro unit. */
Result Fmcomms3_set_dds_scale(Fmcomms3_Handle_t hFmcomms3, uint8_t ch, uint8_t tone, int32_t scale);

#endif
