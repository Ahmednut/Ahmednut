/***************************************************************************//**
 *   @file   ad9361_api.c
 *   @brief  Implementation of AD9361 API Driver.
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
#include "fmcomms3.h"
#include "ad9361_api.h"
#include "ad9361.h"
#include "dac_core.h"
#include "platform.h"
#include "util.h"
#include "bas_debug.h"

/******************************************************************************/
/************************ Constants Definitions *******************************/
/******************************************************************************/

typedef struct Fmcomms3_stFmcomms3
{
    struct ad9361_rf_phy *ad9361_phy;
}Fmcomms3_stFmcomms3_t;


Result Fmcomms3_open (Fmcomms3_Handle_t * phFmcomms3,
    uint32_t uiAd9361BaseAddr, uint32_t uiSpiBaseAddr,
    int (*pfUsleep)(unsigned int usec))
{
    Result res;
	Fmcomms3_Handle_t hFmcomms3;
    
    if(((*(unsigned int *)uiAd9361BaseAddr) >> 16) != 0x9361)
        return eFmcomms3NoCore;
    
    *phFmcomms3 = (Fmcomms3_Handle_t)zmalloc(sizeof(Fmcomms3_stFmcomms3_t));
    if (!*phFmcomms3) {
		return eFmcomms3OutOfMem;
	}

    // Define a handle to easier understanding
    hFmcomms3 = *phFmcomms3;
    
    res = ad9361_open(&hFmcomms3->ad9361_phy, uiAd9361BaseAddr, uiSpiBaseAddr, pfUsleep);
    if(FAILURE(res))
    {
        hFmcomms3->ad9361_phy = NULL;
        goto error;
    }

	return eFmcomms3Ok;
    
error:
	Fmcomms3_close(hFmcomms3);
    *phFmcomms3 = NULL;
	debug("%s : FMCOMMS3 open error\n", "Fmcomms3_open");

	return eFmcomms3NoDev;
}

/**
 * Initialize the AD9361 part.
 * @param init_param The structure that contains the AD9361 initial parameters.
 * @return A structure that contains the AD9361 current state in case of
 *         success, negative error code otherwise.
 */
Result Fmcomms3_init (Fmcomms3_Handle_t hFmcomms3, AD9361_InitParam *init_param)
{
    Result res;

    // Hardware reset the AD9361 chip with the reset pin
    /* TODO
        // Set resetb pin low to reset chip
        // Sleep ~1ms
        // Set resetb pin high to clear reset
        // Sleep ~1ms
    */

    res = ad9361_init(hFmcomms3->ad9361_phy, init_param);
    if(FAILURE(res)) goto error;
    
    res = Fmcomms3_set_dac_datasel(hFmcomms3, DATA_SEL_ZERO);
    if(FAILURE(res)) goto error;

	return eFmcomms3Ok;

error:
	debug("%s : FMCOMMS3 initialization error\n", "Fmcomms3_init");

	return eFmcomms3NoDev;
}

Result Fmcomms3_digital_calibration(Fmcomms3_Handle_t hFmcomms3)
{
    Result res;
    
    res = ad9361_post_setup(hFmcomms3->ad9361_phy);
    if(FAILURE(res)) return res;
    
    res = Fmcomms3_set_dac_datasel(hFmcomms3, DATA_SEL_USER);
    if(FAILURE(res)) return res;
    
	return eFmcomms3Ok;
}

void Fmcomms3_close(Fmcomms3_Handle_t hFmcomms3)
{
    if(hFmcomms3->ad9361_phy) ad9361_close(hFmcomms3->ad9361_phy);
    hFmcomms3->ad9361_phy = NULL;
    
    free(hFmcomms3);
}

Result Fmcomms3_set_sync_delay(Fmcomms3_Handle_t hFmcomms3, int32_t delay)
{
    struct ad9361_rf_phy *phy = hFmcomms3->ad9361_phy;
    struct axiadc_state *st = phy->adc_state;
    uint32_t reg;
    
    reg = axiadc_read(st, REG_SYNC_DELAY);
    
    // Clear target bits
    reg &= (~SYNC_IODELAY_OFFSET(0xFFFFFFFF));
    // Set desired bits
	reg |= SYNC_IODELAY_OFFSET(delay);
    
    axiadc_write(st, REG_SYNC_DELAY, reg);
    
    return eFmcomms3Ok;
}

Result Fmcomms3_set_refclk_delay(Fmcomms3_Handle_t hFmcomms3, int32_t delay)
{
    struct ad9361_rf_phy *phy = hFmcomms3->ad9361_phy;
    struct axiadc_state *st = phy->adc_state;
    uint32_t reg;
    
    reg = axiadc_read(st, REG_SYNC_DELAY);
    
    // Clear target bits
    reg &= (~REFCLK_IODELAY_OFFSET(0xFFFFFFFF));
    // Set desired bits
	reg |= REFCLK_IODELAY_OFFSET(delay);
    
    axiadc_write(st, REG_SYNC_DELAY, reg);
    
    return eFmcomms3Ok;
}

Result Fmcomms3_set_sync_bitslip(Fmcomms3_Handle_t hFmcomms3, int32_t bitslip)
{
    struct ad9361_rf_phy *phy = hFmcomms3->ad9361_phy;
    struct axiadc_state *st = phy->adc_state;
    uint32_t reg;
    
    reg = axiadc_read(st, REG_SYNC_SEL);
    
    // Clear bit
    reg &= (~SYNC_BIT_SLIP);
    
    if(bitslip)
        reg |= SYNC_BIT_SLIP;
    
    axiadc_write(st, REG_SYNC_SEL, reg);
    
    return eFmcomms3Ok;
}

Result Fmcomms3_set_sync_src(Fmcomms3_Handle_t hFmcomms3, int32_t src)
{
    struct ad9361_rf_phy *phy = hFmcomms3->ad9361_phy;
    struct axiadc_state *st = phy->adc_state;
    uint32_t reg;
    
    reg = axiadc_read(st, REG_SYNC_SEL);
    
    // Clear target bits
    reg &= (~SYNC_SEL_OFFSET(0xFFFFFFFF));
    // Set desired bits
	reg |= SYNC_SEL_OFFSET(src);
    
    axiadc_write(st, REG_SYNC_SEL, reg);
    
    return eFmcomms3Ok;
}

Result Fmcomms3_soft_sync(Fmcomms3_Handle_t hFmcomms3)
{
    struct ad9361_rf_phy *phy = hFmcomms3->ad9361_phy;
    struct axiadc_state *st = phy->adc_state;
    
	axiadc_write(st, REG_SOFT_SYNC, 0);
	axiadc_write(st, REG_SOFT_SYNC, SOFT_SYNC);
	axiadc_write(st, REG_SOFT_SYNC, 0);
    
    return eFmcomms3Ok;
}

Result Fmcomms3_mcs(Fmcomms3_Handle_t hFmcomms3, int32_t step)
{
    struct ad9361_rf_phy *phy = hFmcomms3->ad9361_phy;
    
    return ad9361_mcs(phy, step);
}

Result Fmcomms3_set_master_calib_result(Fmcomms3_Handle_t hFmcomms3, Fmcomms3_Handle_t hMasterFmcomms3)
{
    return ad9361_set_master_calib_result(hFmcomms3->ad9361_phy, hMasterFmcomms3->ad9361_phy);
}

Result Fmcomms3_set_ext_master_calib_result(Fmcomms3_Handle_t hFmcomms3, sAd9361CalibResult_t ExtCalibResult)
{
    return ad9361_set_ext_master_calib_result(hFmcomms3->ad9361_phy, ExtCalibResult);
}

Result Fmcomms3_get_calib_result(Fmcomms3_Handle_t hFmcomms3, sAd9361CalibResult_t ** ppCalibResult)
{
    *ppCalibResult = &(hFmcomms3->ad9361_phy->CalibResult);
    return eFmcomms3Ok;
}

/**
 * Set the Enable State Machine (ENSM) mode.
 * @param phy The AD9361 current state structure.
 * @param mode The ENSM mode (SLEEP, ALERT, FDD, PINCTRL).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_set_en_state_machine_mode (Fmcomms3_Handle_t hFmcomms3,
										  uint32_t mode)
{
	int32_t ret;

	ret = ad9361_set_en_state_machine_mode (hFmcomms3->ad9361_phy,
										  mode);
	return ret;
}

/**
 * Get the Enable State Machine (ENSM) mode.
 * @param phy The AD9361 current state structure.
 * @param mode A variable to store the selected ENSM mode.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_en_state_machine_mode (Fmcomms3_Handle_t hFmcomms3,
										  uint32_t *mode)
{
	int32_t ret;

	ret = ad9361_get_en_state_machine_mode (hFmcomms3->ad9361_phy,
										  mode);

	return ret;
}

/**
 * Set the receive RF gain for the selected channel.
 * @param phy The AD9361 current state structure.
 * @param ch The desired channel number (1, 2).
 * @param gain_db The RF gain (dB).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_set_rx_rf_gain (Fmcomms3_Handle_t hFmcomms3,
							   uint8_t ch, int32_t gain_db)
{
    int32_t ret;

	ret = ad9361_set_rx_rf_gain (hFmcomms3->ad9361_phy,
										  ch, gain_db);

	return ret;
}

/**
 * Get current receive RF gain for the selected channel.
 * @param phy The AD9361 current state structure.
 * @param ch The desired channel number (1, 2).
 * @param gain_db A variable to store the RF gain (dB).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_rx_rf_gain (Fmcomms3_Handle_t hFmcomms3,
							   uint8_t ch, int32_t *gain_db)
{
    int32_t ret;

	ret = ad9361_get_rx_rf_gain (hFmcomms3->ad9361_phy,
										  ch, gain_db);

	return ret;
}

/**
 * Set the RX RF bandwidth.
 * @param phy The AD9361 current state structure.
 * @param bandwidth_hz The desired bandwidth (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_set_rx_rf_bandwidth (Fmcomms3_Handle_t hFmcomms3,
									uint32_t bandwidth_hz)
{
    int32_t ret;

	ret = ad9361_set_rx_rf_bandwidth (hFmcomms3->ad9361_phy,
										  bandwidth_hz);

	return ret;
}

/**
 * Get the RX RF bandwidth.
 * @param phy The AD9361 current state structure.
 * @param bandwidth_hz A variable to store the bandwidth value (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_rx_rf_bandwidth (Fmcomms3_Handle_t hFmcomms3,
									uint32_t *bandwidth_hz)
{
    int32_t ret;

	ret = ad9361_get_rx_rf_bandwidth (hFmcomms3->ad9361_phy,
										  bandwidth_hz);

	return ret;
}

/**
 * Set the TX and RX sampling frequency.
 * @param phy The AD9361 current state structure.
 * @param sampling_freq_hz The desired frequency (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_set_sampling_freq (Fmcomms3_Handle_t hFmcomms3,
									 uint32_t sampling_freq_hz)
{
    int32_t ret;
    uint32_t chan;

    // ad9361_set_rx_sampling_freq and ad9361_set_tx_sampling_freq functions
    // do exactly the same thing
	ret = ad9361_set_rx_sampling_freq (hFmcomms3->ad9361_phy,
										  sampling_freq_hz);
    if(FAILURE(ret))
        return ret;

	for(chan = DDS_CHAN_TX1_I_F1; chan <= DDS_CHAN_TX2_Q_F2; chan++)
    {
        enum dds_data_select sel;

        dac_get_datasel(hFmcomms3->ad9361_phy, chan, &sel);

        if(sel == DATA_SEL_DDS)
        {
            dds_update(hFmcomms3->ad9361_phy);
            break;
        }
    }

	return ret;
}

/**
 * Get current TX and RX sampling frequency.
 * @param phy The AD9361 current state structure.
 * @param sampling_freq_hz A variable to store the frequency value (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_sampling_freq (Fmcomms3_Handle_t hFmcomms3,
									 uint32_t *sampling_freq_hz)
{
    int32_t ret;

    // ad9361_get_rx_sampling_freq and ad9361_get_tx_sampling_freq functions
    // do exactly the same thing
	ret = ad9361_get_rx_sampling_freq (hFmcomms3->ad9361_phy,
										  sampling_freq_hz);

	return ret;
}

/**
 * Set the RX LO frequency.
 * @param phy The AD9361 current state structure.
 * @param lo_freq_hz The desired frequency (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_set_rx_lo_freq (Fmcomms3_Handle_t hFmcomms3,
							   uint64_t lo_freq_hz)
{
    int32_t ret;

	ret = ad9361_set_rx_lo_freq (hFmcomms3->ad9361_phy,
										  lo_freq_hz);

	return ret;
}

/**
 * Get current RX LO frequency.
 * @param phy The AD9361 current state structure.
 * @param lo_freq_hz A variable to store the frequency value (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_rx_lo_freq (Fmcomms3_Handle_t hFmcomms3,
							   uint64_t *lo_freq_hz)
{
    int32_t ret;

	ret = ad9361_get_rx_lo_freq (hFmcomms3->ad9361_phy,
										  lo_freq_hz);

	return ret;
}

/**
 * Get the RSSI for the selected channel.
 * @param phy The AD9361 current state structure.
 * @param ch The desired channel (1, 2).
 * @param rssi A variable to store the RSSI.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_rx_rssi (Fmcomms3_Handle_t hFmcomms3,
							uint8_t ch, struct rf_rssi *rssi)
{
    int32_t ret;

	ret = ad9361_get_rx_rssi (hFmcomms3->ad9361_phy,
										  ch, rssi);

	return ret;
}

/**
 * Set the gain control mode for the selected channel.
 * @param phy The AD9361 current state structure.
 * @param ch The desired channel (1, 2).
 * @param gc_mode The gain control mode (GAIN_MGC, GAIN_FASTATTACK_AGC,
 *                GAIN_SLOWATTACK_AGC, GAIN_HYBRID_AGC).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_set_rx_gain_control_mode (Fmcomms3_Handle_t hFmcomms3,
										 uint8_t ch, uint8_t gc_mode)
{
    int32_t ret;

	ret = ad9361_set_rx_gain_control_mode (hFmcomms3->ad9361_phy,
										  ch, gc_mode);

	return ret;
}

/**
 * Get the gain control mode for the selected channel.
 * @param phy The AD9361 current state structure.
 * @param ch The desired channel (1, 2).
 * @param gc_mode A variable to store the gain control mode.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_rx_gain_control_mode (Fmcomms3_Handle_t hFmcomms3,
										 uint8_t ch, uint8_t *gc_mode)
{
    int32_t ret;

	ret = ad9361_get_rx_gain_control_mode (hFmcomms3->ad9361_phy,
										  ch, gc_mode);

	return ret;
}

/**
 * Set the RX FIR filter configuration.
 * @param phy The AD9361 current state structure.
 * @param fir_cfg FIR filter configuration.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_set_rx_fir_config (Fmcomms3_Handle_t hFmcomms3,
								  AD9361_RXFIRConfig fir_cfg)
{
    int32_t ret;

	ret = ad9361_set_rx_fir_config (hFmcomms3->ad9361_phy,
										  fir_cfg);

	return ret;
}

/**
 * Get the RX FIR filter configuration.
 * @param phy The AD9361 current state structure.
 * @param tx_ch The selected RX channel (1, 2).
 * @param fir_cfg FIR filter configuration output file.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_rx_fir_config(Fmcomms3_Handle_t hFmcomms3, uint8_t rx_ch, AD9361_RXFIRConfig *fir_cfg)
{
    int32_t ret;

	ret = ad9361_get_rx_fir_config (hFmcomms3->ad9361_phy,
			rx_ch, fir_cfg);

	return ret;
}

/**
 * Enable/disable the RX FIR filter.
 * @param phy The AD9361 current state structure.
 * @param en_dis The option (ENABLE, DISABLE).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_set_rx_fir_en_dis (Fmcomms3_Handle_t hFmcomms3,
								  uint8_t en_dis)
{
    int32_t ret;

	ret = ad9361_set_rx_fir_en_dis (hFmcomms3->ad9361_phy,
										  en_dis);

	return ret;
}

/**
 * Get the status of the RX FIR filter.
 * @param phy The AD9361 current state structure.
 * @param en_dis The enable/disable status buffer.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_rx_fir_en_dis (Fmcomms3_Handle_t hFmcomms3,
								  uint8_t *en_dis)
{
    int32_t ret;

	ret = ad9361_get_rx_fir_en_dis (hFmcomms3->ad9361_phy,
										  en_dis);

	return ret;
}

/**
 * Enable/disable the RX RFDC Tracking.
 * @param phy The AD9361 current state structure.
 * @param en_dis The option (ENABLE, DISABLE).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_set_rx_rfdc_track_en_dis (Fmcomms3_Handle_t hFmcomms3,
										 uint8_t en_dis)
{
    int32_t ret;

	ret = ad9361_set_rx_rfdc_track_en_dis (hFmcomms3->ad9361_phy,
										  en_dis);

	return ret;
}

/**
 * Get the status of the RX RFDC Tracking.
 * @param phy The AD9361 current state structure.
 * @param en_dis The enable/disable status buffer.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_rx_rfdc_track_en_dis (Fmcomms3_Handle_t hFmcomms3,
										 uint8_t *en_dis)
{
    int32_t ret;

	ret = ad9361_get_rx_rfdc_track_en_dis (hFmcomms3->ad9361_phy,
										  en_dis);

	return ret;
}

/**
 * Enable/disable the RX BasebandDC Tracking.
 * @param phy The AD9361 current state structure.
 * @param en_dis The option (ENABLE, DISABLE).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_set_rx_bbdc_track_en_dis (Fmcomms3_Handle_t hFmcomms3,
										 uint8_t en_dis)
{
    int32_t ret;

	ret = ad9361_set_rx_bbdc_track_en_dis (hFmcomms3->ad9361_phy,
										  en_dis);

	return ret;
}

/**
 * Get the status of the RX BasebandDC Tracking.
 * @param phy The AD9361 current state structure.
 * @param en_dis The enable/disable status buffer.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_rx_bbdc_track_en_dis (Fmcomms3_Handle_t hFmcomms3,
										 uint8_t *en_dis)
{
    int32_t ret;

	ret = ad9361_get_rx_bbdc_track_en_dis (hFmcomms3->ad9361_phy,
										  en_dis);

	return ret;
}

/**
 * Enable/disable the RX Quadrature Tracking.
 * @param phy The AD9361 current state structure.
 * @param en_dis The option (ENABLE, DISABLE).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_set_rx_quad_track_en_dis (Fmcomms3_Handle_t hFmcomms3,
										 uint8_t en_dis)
{
    int32_t ret;

	ret = ad9361_set_rx_quad_track_en_dis (hFmcomms3->ad9361_phy,
										  en_dis);

	return ret;
}

/**
 * Get the status of the RX Quadrature Tracking.
 * @param phy The AD9361 current state structure.
 * @param en_dis The enable/disable status buffer.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_rx_quad_track_en_dis (Fmcomms3_Handle_t hFmcomms3,
										 uint8_t *en_dis)
{
    int32_t ret;

	ret = ad9361_get_rx_quad_track_en_dis (hFmcomms3->ad9361_phy,
										  en_dis);

	return ret;
}

/**
 * Set the transmit attenuation for the selected channel.
 * @param phy The AD9361 current state structure.
 * @param ch The desired channel number (1, 2).
 * @param attenuation_mdb The attenuation (mdB).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_set_tx_attenuation (Fmcomms3_Handle_t hFmcomms3,
								   uint8_t ch, uint32_t attenuation_mdb)
{
    int32_t ret;

	ret = ad9361_set_tx_attenuation (hFmcomms3->ad9361_phy,
										  ch, attenuation_mdb);

	return ret;
}

/**
 * Get current transmit attenuation for the selected channel.
 * @param phy The AD9361 current state structure.
 * @param ch The desired channel number (1, 2).
 * @param attenuation_mdb A variable to store the attenuation value (mdB).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_tx_attenuation (Fmcomms3_Handle_t hFmcomms3,
								   uint8_t ch, uint32_t *attenuation_mdb)
{
    int32_t ret;

	ret = ad9361_get_tx_attenuation (hFmcomms3->ad9361_phy,
										  ch, attenuation_mdb);

	return ret;
}

/**
 * Set the TX RF bandwidth.
 * @param phy The AD9361 current state structure.
 * @param bandwidth_hz The desired bandwidth (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_set_tx_rf_bandwidth (Fmcomms3_Handle_t hFmcomms3,
									uint32_t  bandwidth_hz)
{
    int32_t ret;

	ret = ad9361_set_tx_rf_bandwidth (hFmcomms3->ad9361_phy,
										  bandwidth_hz);

	return ret;
}

/**
 * Get the TX RF bandwidth.
 * @param phy The AD9361 current state structure.
 * @param bandwidth_hz A variable to store the bandwidth value (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_tx_rf_bandwidth (Fmcomms3_Handle_t hFmcomms3,
									uint32_t *bandwidth_hz)
{
    int32_t ret;

	ret = ad9361_get_tx_rf_bandwidth (hFmcomms3->ad9361_phy,
										  bandwidth_hz);

	return ret;
}

/**
 * Set the TX LO frequency.
 * @param phy The AD9361 current state structure.
 * @param lo_freq_hz The desired frequency (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_set_tx_lo_freq (Fmcomms3_Handle_t hFmcomms3,
							   uint64_t lo_freq_hz)
{
    int32_t ret;

	ret = ad9361_set_tx_lo_freq (hFmcomms3->ad9361_phy,
										  lo_freq_hz);

	return ret;
}

/**
 * Get current TX LO frequency.
 * @param phy The AD9361 current state structure.
 * @param lo_freq_hz A variable to store the frequency value (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_tx_lo_freq (Fmcomms3_Handle_t hFmcomms3,
							   uint64_t *lo_freq_hz)
{
    int32_t ret;

	ret = ad9361_get_tx_lo_freq (hFmcomms3->ad9361_phy,
										  lo_freq_hz);

	return ret;
}

/**
 * Set the TX FIR filter configuration.
 * @param phy The AD9361 current state structure.
 * @param fir_cfg FIR filter configuration.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_set_tx_fir_config (Fmcomms3_Handle_t hFmcomms3,
								  AD9361_TXFIRConfig fir_cfg)
{
    int32_t ret;

	ret = ad9361_set_tx_fir_config (hFmcomms3->ad9361_phy,
										  fir_cfg);

	return ret;
}

/**
 * Get the TX FIR filter configuration.
 * @param phy The AD9361 current state structure.
 * @param tx_ch The selected TX channel (1, 2).
 * @param fir_cfg FIR filter configuration output file.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_tx_fir_config(Fmcomms3_Handle_t hFmcomms3, uint8_t tx_ch, AD9361_TXFIRConfig *fir_cfg)
{
    int32_t ret;

	ret = ad9361_get_tx_fir_config (hFmcomms3->ad9361_phy,
			tx_ch, fir_cfg);

	return ret;
}

/**
 * Enable/disable the TX FIR filter.
 * @param phy The AD9361 current state structure.
 * @param en_dis The option (ENABLE, DISABLE).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_set_tx_fir_en_dis (Fmcomms3_Handle_t hFmcomms3,
								  uint8_t en_dis)
{
    int32_t ret;

	ret = ad9361_set_tx_fir_en_dis (hFmcomms3->ad9361_phy,
										  en_dis);

	return ret;
}

/**
 * Get the status of the TX FIR filter.
 * @param phy The AD9361 current state structure.
 * @param en_dis The enable/disable status buffer.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_tx_fir_en_dis (Fmcomms3_Handle_t hFmcomms3,
								  uint8_t *en_dis)
{
    int32_t ret;

	ret = ad9361_get_tx_fir_en_dis (hFmcomms3->ad9361_phy,
										  en_dis);

	return ret;
}

/**
 * Set the RX and TX path rates.
 * @param phy The AD9361 state structure.
 * @param rx_path_clks RX path rates buffer.
 * @param tx_path_clks TX path rates buffer.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_set_trx_path_clks(Fmcomms3_Handle_t hFmcomms3,
	uint32_t *rx_path_clks,
	uint32_t *tx_path_clks)
{
    int32_t ret;

	ret = ad9361_set_trx_path_clks (hFmcomms3->ad9361_phy,
			rx_path_clks, tx_path_clks);

	return ret;
}

/**
 * Get the RX and TX path rates.
 * @param phy The AD9361 state structure.
 * @param rx_path_clks RX path rates buffer.
 * @param tx_path_clks TX path rates buffer.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Fmcomms3_get_trx_path_clks(Fmcomms3_Handle_t hFmcomms3,
	uint32_t *rx_path_clks,
	uint32_t *tx_path_clks)
{
    int32_t ret;

	ret = ad9361_get_trx_path_clks (hFmcomms3->ad9361_phy,
										  rx_path_clks, tx_path_clks);

	return ret;
}

Result Fmcomms3_spi_read(Fmcomms3_Handle_t hFmcomms3, uint32_t reg, uint8_t * val)
{
    int32_t res;
    
    res = ad9361_spi_read(hFmcomms3->ad9361_phy->spi, reg);
    if(res < 0) return eFmcomms3SpiError;
    
    *val = (uint8_t)res;
    return eFmcomms3Ok;
}

Result Fmcomms3_spi_write(Fmcomms3_Handle_t hFmcomms3, uint32_t reg, uint8_t val)
{
    int32_t res;
    
    res = ad9361_spi_write(hFmcomms3->ad9361_phy->spi, reg, val);
    if(res < 0) return eFmcomms3SpiError;
    
    return eFmcomms3Ok;
}

/**************************************************************************//***
 * @brief Sets data selection for the DAC.
 *        Only DATA_SEL_USER and DATA_SEL_DDS are allowed.
 *
 * @return Success status.
*******************************************************************************/
Result Fmcomms3_set_dac_datasel(Fmcomms3_Handle_t hFmcomms3, dds_data_select_e sel)
{
	switch (sel) {
        case DATA_SEL_DDS:
            dds_default_setup(hFmcomms3->ad9361_phy, DDS_CHAN_TX1_I_F1, 90000, 1000000, 250000);
            dds_default_setup(hFmcomms3->ad9361_phy, DDS_CHAN_TX1_I_F2, 90000, 1000000, 250000);
            dds_default_setup(hFmcomms3->ad9361_phy, DDS_CHAN_TX1_Q_F1, 0, 1000000, 250000);
            dds_default_setup(hFmcomms3->ad9361_phy, DDS_CHAN_TX1_Q_F2, 0, 1000000, 250000);
            if(hFmcomms3->ad9361_phy->dds_st.rx2tx2)
            {
                dds_default_setup(hFmcomms3->ad9361_phy, DDS_CHAN_TX2_I_F1, 90000, 1000000, 250000);
                dds_default_setup(hFmcomms3->ad9361_phy, DDS_CHAN_TX2_I_F2, 90000, 1000000, 250000);
                dds_default_setup(hFmcomms3->ad9361_phy, DDS_CHAN_TX2_Q_F1, 0, 1000000, 250000);
                dds_default_setup(hFmcomms3->ad9361_phy, DDS_CHAN_TX2_Q_F2, 0, 1000000, 250000);
            }
            dac_write(hFmcomms3->ad9361_phy, DAC_REG_CNTRL_2, 0);
            dac_datasel(hFmcomms3->ad9361_phy, -1, DATA_SEL_DDS);
            break;
            
        case DATA_SEL_USER:
            dac_write(hFmcomms3->ad9361_phy, DAC_REG_CNTRL_2, 0);
            dac_datasel(hFmcomms3->ad9361_phy, -1, DATA_SEL_USER);
            break;
            
        case DATA_SEL_ZERO:
            dac_write(hFmcomms3->ad9361_phy, DAC_REG_CNTRL_2, 0);
            dac_datasel(hFmcomms3->ad9361_phy, -1, DATA_SEL_ZERO);
            break;
            
        default:
            return eFmcomms3InvArg;
	}
    
	dac_start_sync(hFmcomms3->ad9361_phy, 0);
    
    return eFmcomms3Ok;
}

/**************************************************************************//***
 * @brief Gets current data selection for the DAC.
 *
 * @return Success status.
*******************************************************************************/
Result Fmcomms3_get_dac_datasel(Fmcomms3_Handle_t hFmcomms3, dds_data_select_e * sel)
{
    // Only read channel 0 since all channels are always configured the same by
    // Fmcomms3_set_dac_datasel function
	*sel = hFmcomms3->ad9361_phy->dds_st.cached_datasel[0];
    
    return eFmcomms3Ok;
}

/**************************************************************************//***
 * @brief Gets current DDS frequency [Hz].
 *
 * @return Success status.
*******************************************************************************/
Result Fmcomms3_get_dds_freq(Fmcomms3_Handle_t hFmcomms3, uint8_t ch, uint8_t tone, uint32_t * freq)
{
    if(ch == 1 && tone == 0)
        *freq = hFmcomms3->ad9361_phy->dds_st.cached_freq[DDS_CHAN_TX1_I_F1];
    else if(ch == 1 && tone == 1)
        *freq = hFmcomms3->ad9361_phy->dds_st.cached_freq[DDS_CHAN_TX1_I_F2];
    else if(ch == 2 && tone == 0)
        *freq = hFmcomms3->ad9361_phy->dds_st.cached_freq[DDS_CHAN_TX2_I_F1];
    else if(ch == 2 && tone == 1)
        *freq = hFmcomms3->ad9361_phy->dds_st.cached_freq[DDS_CHAN_TX2_I_F2];
    else
        return eFmcomms3InvArg;

    return eFmcomms3Ok;
}

/**************************************************************************//***
 * @brief Sets the DDS frequency [Hz].
 *
 * @return Success status.
*******************************************************************************/
Result Fmcomms3_set_dds_freq(Fmcomms3_Handle_t hFmcomms3, uint8_t ch, uint8_t tone, uint32_t freq)
{
    if(ch == 1 && tone == 0)
    {
		dds_set_frequency(hFmcomms3->ad9361_phy, DDS_CHAN_TX1_I_F1, freq);
		dds_set_frequency(hFmcomms3->ad9361_phy, DDS_CHAN_TX1_Q_F1, freq);
    }
    else if(ch == 1 && tone == 1)
    {
		dds_set_frequency(hFmcomms3->ad9361_phy, DDS_CHAN_TX1_I_F2, freq);
		dds_set_frequency(hFmcomms3->ad9361_phy, DDS_CHAN_TX1_Q_F2, freq);
    }
    else if(ch == 2 && tone == 0)
    {
		dds_set_frequency(hFmcomms3->ad9361_phy, DDS_CHAN_TX2_I_F1, freq);
		dds_set_frequency(hFmcomms3->ad9361_phy, DDS_CHAN_TX2_Q_F1, freq);
    }
    else if(ch == 2 && tone == 1)
    {
		dds_set_frequency(hFmcomms3->ad9361_phy, DDS_CHAN_TX2_I_F2, freq);
		dds_set_frequency(hFmcomms3->ad9361_phy, DDS_CHAN_TX2_Q_F2, freq);
    }
    else
    {
        return eFmcomms3InvArg;
    }

    return eFmcomms3Ok;
}

/**************************************************************************//***
 * @brief Gets current DDS phase [mDegrees].
 *
 * @return Success status.
*******************************************************************************/
Result Fmcomms3_get_dds_phase(Fmcomms3_Handle_t hFmcomms3, uint8_t ch, uint8_t tone, uint32_t * phase)
{

    if(ch == 1 && tone == 0)
    {
        *phase = hFmcomms3->ad9361_phy->dds_st.cached_phase[DDS_CHAN_TX1_I_F1];
    }
    else if(ch == 1 && tone == 1)
    {
        *phase = hFmcomms3->ad9361_phy->dds_st.cached_phase[DDS_CHAN_TX1_I_F2];
    }
    else if(ch == 2 && tone == 0)
    {
        *phase = hFmcomms3->ad9361_phy->dds_st.cached_phase[DDS_CHAN_TX2_I_F1];
    }
    else if(ch == 2 && tone == 1)
    {
        *phase = hFmcomms3->ad9361_phy->dds_st.cached_phase[DDS_CHAN_TX2_I_F2];
    }
    else
    {
        return eFmcomms3InvArg;
    }

    return eFmcomms3Ok;
}

/**************************************************************************//***
 * @brief Sets the DDS phase [mDegrees].
 *
 * @return Success status.
*******************************************************************************/
Result Fmcomms3_set_dds_phase(Fmcomms3_Handle_t hFmcomms3, uint8_t ch, uint8_t tone, uint32_t phase)
{
    uint32_t uiPhaseI;
    uint32_t uiPhaseQ;

    uiPhaseI = phase;

    if (phase < (90 * 1000))
        uiPhaseQ = phase + (270 * 1000);
    else
        uiPhaseQ = phase - (90 * 1000);

    if(ch == 1 && tone == 0)
    {
        dds_set_phase(hFmcomms3->ad9361_phy, DDS_CHAN_TX1_I_F1, uiPhaseI);
		dds_set_phase(hFmcomms3->ad9361_phy, DDS_CHAN_TX1_Q_F1, uiPhaseQ);
    }
    else if(ch == 1 && tone == 1)
    {
        dds_set_phase(hFmcomms3->ad9361_phy, DDS_CHAN_TX1_I_F2, uiPhaseI);
		dds_set_phase(hFmcomms3->ad9361_phy, DDS_CHAN_TX1_Q_F2, uiPhaseQ);
    }
    else if(ch == 2 && tone == 0)
    {
        dds_set_phase(hFmcomms3->ad9361_phy, DDS_CHAN_TX2_I_F1, uiPhaseI);
		dds_set_phase(hFmcomms3->ad9361_phy, DDS_CHAN_TX2_Q_F1, uiPhaseQ);
    }
    else if(ch == 2 && tone == 1)
    {
        dds_set_phase(hFmcomms3->ad9361_phy, DDS_CHAN_TX2_I_F2, uiPhaseI);
		dds_set_phase(hFmcomms3->ad9361_phy, DDS_CHAN_TX2_Q_F2, uiPhaseQ);
    }
    else
    {
        return eFmcomms3InvArg;
    }

    return eFmcomms3Ok;
}


/**************************************************************************//***
 * @brief Gets current DDS scale in micro unit.
 *
 * @return Success status.
*******************************************************************************/
Result Fmcomms3_get_dds_scale(Fmcomms3_Handle_t hFmcomms3, uint8_t ch, uint8_t tone, int32_t * scale)
{
    if(ch == 1 && tone == 0)
    {
        *scale = hFmcomms3->ad9361_phy->dds_st.cached_scale[DDS_CHAN_TX1_I_F1];
    }
    else if(ch == 1 && tone == 1)
    {
        *scale = hFmcomms3->ad9361_phy->dds_st.cached_scale[DDS_CHAN_TX1_I_F2];
    }
    else if(ch == 2 && tone == 0)
    {
        *scale = hFmcomms3->ad9361_phy->dds_st.cached_scale[DDS_CHAN_TX2_I_F1];
    }
    else if(ch == 2 && tone == 1)
    {
        *scale = hFmcomms3->ad9361_phy->dds_st.cached_scale[DDS_CHAN_TX2_I_F2];
    }
    else
    {
        return eFmcomms3InvArg;
    }

    return eFmcomms3Ok;
}

/**************************************************************************//***
 * @brief Sets the DDS scale.
 *
 * @return Success status.
*******************************************************************************/
Result Fmcomms3_set_dds_scale(Fmcomms3_Handle_t hFmcomms3, uint8_t ch, uint8_t tone, int32_t scale)
{
    if(ch == 1 && tone == 0)
    {
		dds_set_scale(hFmcomms3->ad9361_phy, DDS_CHAN_TX1_I_F1, scale);
		dds_set_scale(hFmcomms3->ad9361_phy, DDS_CHAN_TX1_Q_F1, scale);
    }
    else if(ch == 1 && tone == 1)
    {
		dds_set_scale(hFmcomms3->ad9361_phy, DDS_CHAN_TX1_I_F2, scale);
		dds_set_scale(hFmcomms3->ad9361_phy, DDS_CHAN_TX1_Q_F2, scale);
    }
    else if(ch == 2 && tone == 0)
    {
		dds_set_scale(hFmcomms3->ad9361_phy, DDS_CHAN_TX2_I_F1, scale);
		dds_set_scale(hFmcomms3->ad9361_phy, DDS_CHAN_TX2_Q_F1, scale);
    }
    else if(ch == 2 && tone == 1)
    {
		dds_set_scale(hFmcomms3->ad9361_phy, DDS_CHAN_TX2_I_F2, scale);
		dds_set_scale(hFmcomms3->ad9361_phy, DDS_CHAN_TX2_Q_F2, scale);
    }
    else
    {
        return eFmcomms3InvArg;
    }

    return eFmcomms3Ok;
}
