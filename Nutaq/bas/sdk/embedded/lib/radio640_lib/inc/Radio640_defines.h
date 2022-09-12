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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       Radio640_defines.h
 * @brief      Radio640 module related structure and data definitions
 *
 * $Date: 2016/04/22 20:01:34 $
 * $Revision: 1.16 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup RADIO640
 *
 * This file is part of Nutaq’s ADP Software Suite.
 *
 * You may at your option receive a license to Nutaq’s ADP Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq’s ADP Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq’s ADP Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq’s ADP Software Suite under the GPL requires that your work based on
 * Nutaq’s ADP Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq’s ADP Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq’s ADP Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/

#ifndef RADIO640_DEFINES_H__
#define RADIO640_DEFINES_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_types.h"
#include "bas_error.h"
#include "Ad9361.h"

/******************************************************************************/
/*************************** Types Declarations *******************************/
/******************************************************************************/

#define MAX_RADIO640    4

typedef struct Radio640_stRadio640 * Radio640_hRadio640;

// List of clock source
typedef enum Radio640_eClkSrc {
    eRadio640ClkSrcExt = 0,
    eRadio640ClkSrcInt = 1,
    eRadio640ClkSrcCarrier = 2,
    eRadio640ClkSrcMaster = 3,
    eRadio640ClkSrcUnknown = 4
} Radio640_eClkSrc;

static const string_enum_element_t RADIO640_CLKSRC_STRING_TABLE[] = {
    {"eRadio640ClkSrcExt",      eRadio640ClkSrcExt},
    {"eRadio640ClkSrcInt",      eRadio640ClkSrcInt},
    {"eRadio640ClkSrcCarrier",  eRadio640ClkSrcCarrier},
    {"eRadio640ClkSrcMaster",   eRadio640ClkSrcMaster},
    {"eRadio640ClkSrcUnknown",  eRadio640ClkSrcUnknown}
};

// List Radio640 channel
typedef enum Radio640_eChannel {
    eRadio640ChannelNone = 0,
    eRadio640Channel1 = 1,
    eRadio640Channel2 = 2,
    eRadio640BothChannels = 3
} Radio640_eChannel;

static const string_enum_element_t RADIO640_CHANNEL_STRING_TABLE[] = {
    {"eRadio640ChannelNone",      eRadio640ChannelNone},
    {"eRadio640Channel1",      eRadio640Channel1},
    {"eRadio640Channel2",  eRadio640Channel2},
    {"eRadio640BothChannels",   eRadio640BothChannels}
};

// List Radio640 standard RX RF filter
typedef enum Radio640_eRxRfFilter {
    eRadio640RxRfFilter2G45 = 0,
    eRadio640RxRfFilter5G387 = 1,
    eRadio640RxRfFilter3G6 = 2,
    eRadio640RxRfFilterBypass = 3
} Radio640_eRxRfFilter;

static const string_enum_element_t RADIO640_RXRFFILTER_STRING_TABLE[] = {
    {"eRadio640RxRfFilter2G45",     eRadio640RxRfFilter2G45},
    {"eRadio640RxRfFilter5G387",    eRadio640RxRfFilter5G387},
    {"eRadio640RxRfFilter3G6",      eRadio640RxRfFilter3G6},
    {"eRadio640RxRfFilterBypass",   eRadio640RxRfFilterBypass}
};

typedef enum Radio640_eSyncSrc {
    eRadio640SyncSrcSoft = 0,
    eRadio640SyncSrcInputSyncOnRisingEdge = 1,
    eRadio640SyncSrcInputSyncOnFallingEdge = 2
} Radio640_eSyncSrc;

static const string_enum_element_t RADIO640_SYNCSRC_STRING_TABLE[] = {
    {"eRadio640SyncSrcSoft",                    eRadio640SyncSrcSoft},
    {"eRadio640SyncSrcInputSyncOnRisingEdge",   eRadio640SyncSrcInputSyncOnRisingEdge},
    {"eRadio640SyncSrcInputSyncOnFallingEdge",  eRadio640SyncSrcInputSyncOnFallingEdge}
};

// List Radio640 clocks available for frequency reading
typedef enum Radio640_eTestClk {
    eRadio640TestClkSerial = 0,
    eRadio640TestClkDesign = 1,
    eRadio640TestClkRef = 2,
    eRadio640TestClkAd9361ClkOut = 3
} Radio640_eTestClk;


// List Radio640 calibration
typedef enum Radio640_eCalib {
    eRadio640CalibTxQuad=0,
    eRadio640CalibRxRfDcOffset=1
} Radio640_eCalib;

static const string_enum_element_t RADIO640_CALIB_STRING_TABLE[] = {
    {"eRadio640CalibTxQuad",                    eRadio640CalibTxQuad},
    {"eRadio640CalibRxRfDcOffset",   eRadio640CalibRxRfDcOffset}
};

typedef struct Radio640_stInitParam
{
    /* Reference Clock */
    Radio640_eClkSrc    ClkSrc;                     /* Reference clock source */
    uint32_t            u32RefClkFreq;              /* reference_clk_rate */
    /* Base Configuration */
    uint8_t     u8FddModeEnable;                    /* frequency_division_duplex_mode_enable */
    uint8_t     u8FddIndependentModeEnable;         /* frequency_division_duplex_independent_mode_enable */
    uint8_t     u8TddSkipVcoCalEnable;              /* tdd_skip_vco_cal_enable */
    uint8_t     u8TddUseFddVcoTablesEnable;         /* tdd_use_fdd_vco_tables_enable */
    uint32_t    u32TxFastlockDelayNs;               /* tx_fastlock_delay_ns */
    uint32_t    u32RxFastlockDelayNs;               /* rx_fastlock_delay_ns */
    uint8_t     u8RxFastlockPinControlEnable;       /* rx_fastlock_pincontrol_enable */
    uint8_t     u8TxFastlockPinControlEnable;       /* tx_fastlock_pincontrol_enable */
    uint8_t     u8ExtRxLoEnable;                    /* external_rx_lo_enable */
    uint8_t     u8ExtTxLoEnable;                    /* external_tx_lo_enable */
    uint8_t     u8SplitGainTableModeEnable;         /* split_gain_table_mode_enable */
    uint8_t     u8RxQuadTrackSlowModeEnable;        /* qec_tracking_slow_mode_enable */
    /* DC Offset */
    uint8_t     u8DcOffsetTrackingUpdateEventMask;  /* dc_offset_tracking_update_event_mask */
    uint8_t     u8DcOffsetAttenuationHighRange;     /* dc_offset_attenuation_high_range */
    uint8_t     u8DcOffsetAttenuationLowRange;      /* dc_offset_attenuation_low_range */
    uint8_t     u8DcOffsetCountHighRange;           /* dc_offset_count_high_range */
    uint8_t     u8DcOffsetCountLowRange;            /* dc_offset_count_low_range */
    /* ENSM Control */
    uint8_t     u8EnsmEnablePinPulseModeEnable;     /* ensm_enable_pin_pulse_mode_enable */
        /*
         * 0 : ENABLE and TXNRX pin are level sensitive
         * 1 : ENABLE and TXNRX pin are pulse sensitive
         */
    uint8_t     u8EnsmEnableTxnrxControlEnable;     /* ensm_enable_txnrx_control_enable */
        /*
         * 0 : In TDD mode, the selection of the RF synthesizer is done by bit 4 of register 0x015
         * 1 : In TDD mode, the selection of the RF synthesizer is done by the TXNRX pin
         */
    /* LO Control */
    uint64_t    u64RxRfFreq;                        /* rx_synthesizer_frequency_hz */
    uint64_t    u64TxRfFreq;                        /* tx_synthesizer_frequency_hz */
    /* Rate & BW Control */
    uint32_t    au32RxClockFreq[6];                 /* rx_path_clock_frequencies */
    uint32_t    au32TxClockFreq[6];                 /* tx_path_clock_frequencies */
    uint32_t    u32RxRfBandwidth;                   /* rf_rx_bandwidth_hz */
    uint32_t    u32TxRfBandwidth;                   /* rf_tx_bandwidth_hz */
    /* TX Attenuation Control */
    int32_t     tx_attenuation_mdB;                                     /* adi,tx-attenuation-mdB */
    uint8_t     update_tx_gain_in_alert_enable;                         /* adi,update-tx-gain-in-alert-enable */
    /* Reference Clock Control */
    uint32_t    clk_output_mode_select;                                 /* adi,clk-output-mode-select */
    /* Gain Control */
    uint8_t     gc_rx1_mode;                                            /* adi,gc-rx1-mode */
    uint8_t     gc_rx2_mode;                                            /* adi,gc-rx2-mode */
    uint8_t     gc_adc_large_overload_thresh;                           /* adi,gc-adc-large-overload-thresh */
    uint8_t     gc_adc_ovr_sample_size;                                 /* adi,gc-adc-ovr-sample-size */
    uint8_t     gc_adc_small_overload_thresh;                           /* adi,gc-adc-small-overload-thresh */
    uint32_t    gc_dec_pow_measurement_duration;                        /* adi,gc-dec-pow-measurement-duration */
        /*
         * Number of RX cycle. The value will be rounded to the nearest supported value.
         * The supported values are the following power of 2 :
         * {16, 32, 64, 128, 256, ..., 131072, 262144, 524288}
         */
    uint8_t     gc_dig_gain_enable;                                     /* adi,gc-dig-gain-enable */
    uint16_t    gc_lmt_overload_high_thresh;                            /* adi,gc-lmt-overload-high-thresh */
    uint16_t    gc_lmt_overload_low_thresh;                             /* adi,gc-lmt-overload-low-thresh */
    uint8_t     gc_low_power_thresh;                                    /* adi,gc-low-power-thresh */
    uint8_t     gc_max_dig_gain;                                        /* adi,gc-max-dig-gain */
    /* Gain MGC Control */
    uint8_t     mgc_dec_gain_step;                                      /* adi,mgc-dec-gain-step */
    uint8_t     mgc_inc_gain_step;                                      /* adi,mgc-inc-gain-step */
    uint8_t     mgc_rx1_ctrl_inp_enable;                                /* adi,mgc-rx1-ctrl-inp-enable */
    uint8_t     mgc_rx2_ctrl_inp_enable;                                /* adi,mgc-rx2-ctrl-inp-enable */
    uint8_t     mgc_split_table_ctrl_inp_gain_mode;                     /* adi,mgc-split-table-ctrl-inp-gain-mode */
    /* Gain AGC Control */
    uint8_t     agc_adc_large_overload_exceed_counter;                  /* adi,agc-adc-large-overload-exceed-counter */
    uint8_t     agc_adc_large_overload_inc_steps;                       /* adi,agc-adc-large-overload-inc-steps */
    uint8_t     agc_adc_lmt_small_overload_prevent_gain_inc_enable;     /* adi,agc-adc-lmt-small-overload-prevent-gain-inc-enable */
    uint8_t     agc_adc_small_overload_exceed_counter;                  /* adi,agc-adc-small-overload-exceed-counter */
    uint8_t     agc_dig_gain_step_size;                                 /* adi,agc-dig-gain-step-size */
    uint8_t     agc_dig_saturation_exceed_counter;                      /* adi,agc-dig-saturation-exceed-counter */
    uint32_t    agc_gain_update_interval_us;                            /* adi,agc-gain-update-interval-us */
    uint8_t     agc_immed_gain_change_if_large_adc_overload_enable;     /* adi,agc-immed-gain-change-if-large-adc-overload-enable */
    uint8_t     agc_immed_gain_change_if_large_lmt_overload_enable;     /* adi,agc-immed-gain-change-if-large-lmt-overload-enable */
    uint8_t     agc_inner_thresh_high;                                  /* adi,agc-inner-thresh-high */
    uint8_t     agc_inner_thresh_high_dec_steps;                        /* adi,agc-inner-thresh-high-dec-steps */
    uint8_t     agc_inner_thresh_low;                                   /* adi,agc-inner-thresh-low */
    uint8_t     agc_inner_thresh_low_inc_steps;                         /* adi,agc-inner-thresh-low-inc-steps */
    uint8_t     agc_lmt_overload_large_exceed_counter;                  /* adi,agc-lmt-overload-large-exceed-counter */
    uint8_t     agc_lmt_overload_large_inc_steps;                       /* adi,agc-lmt-overload-large-inc-steps */
    uint8_t     agc_lmt_overload_small_exceed_counter;                  /* adi,agc-lmt-overload-small-exceed-counter */
    uint8_t     agc_outer_thresh_high;                                  /* adi,agc-outer-thresh-high */
    uint8_t     agc_outer_thresh_high_dec_steps;                        /* adi,agc-outer-thresh-high-dec-steps */
    uint8_t     agc_outer_thresh_low;                                   /* adi,agc-outer-thresh-low */
    uint8_t     agc_outer_thresh_low_inc_steps;                         /* adi,agc-outer-thresh-low-inc-steps */
    uint32_t    agc_attack_delay_extra_margin_us;                       /* adi,agc-attack-delay-extra-margin-us */
    uint8_t     agc_sync_for_gain_counter_enable;                       /* adi,agc-sync-for-gain-counter-enable */
    /* Fast AGC */
    uint32_t    fagc_dec_pow_measuremnt_duration;                       /* adi,fagc-dec-pow-measurement-duration */
    uint32_t    fagc_state_wait_time_ns;                                /* adi,fagc-state-wait-time-ns */
    /* Fast AGC - Low Power */
    uint8_t     fagc_allow_agc_gain_increase;                           /* adi,fagc-allow-agc-gain-increase-enable */
    uint32_t    fagc_lp_thresh_increment_time;                          /* adi,fagc-lp-thresh-increment-time */
    uint32_t    fagc_lp_thresh_increment_steps;                         /* adi,fagc-lp-thresh-increment-steps */
    /* Fast AGC - Lock Level */
    uint32_t    fagc_lock_level;                                        /* adi,fagc-lock-level */
    uint8_t     fagc_lock_level_lmt_gain_increase_en;                   /* adi,fagc-lock-level-lmt-gain-increase-enable */
    uint32_t    fagc_lock_level_gain_increase_upper_limit;              /* adi,fagc-lock-level-gain-increase-upper-limit */
    /* Fast AGC - Peak Detectors and Final Settling */
    uint32_t    fagc_lpf_final_settling_steps;                          /* adi,fagc-lpf-final-settling-steps */
    uint32_t    fagc_lmt_final_settling_steps;                          /* adi,fagc-lmt-final-settling-steps */
    uint32_t    fagc_final_overrange_count;                             /* adi,fagc-final-overrange-count */
    /* Fast AGC - Final Power Test */
    uint8_t     fagc_gain_increase_after_gain_lock_en;                  /* adi,fagc-gain-increase-after-gain-lock-enable */
    /* Fast AGC - Unlocking the Gain */
    uint32_t    fagc_gain_index_type_after_exit_rx_mode;                /* adi,fagc-gain-index-type-after-exit-rx-mode */
    uint8_t     fagc_use_last_lock_level_for_set_gain_en;               /* adi,fagc-use-last-lock-level-for-set-gain-enable */
    uint8_t     fagc_rst_gla_stronger_sig_thresh_exceeded_en;           /* adi,fagc-rst-gla-stronger-sig-thresh-exceeded-enable */
    uint32_t    fagc_optimized_gain_offset;                             /* adi,fagc-optimized-gain-offset */
    uint32_t    fagc_rst_gla_stronger_sig_thresh_above_ll;              /* adi,fagc-rst-gla-stronger-sig-thresh-above-ll */
    uint8_t     fagc_rst_gla_engergy_lost_sig_thresh_exceeded_en;       /* adi,fagc-rst-gla-engergy-lost-sig-thresh-exceeded-enable */
    uint8_t     fagc_rst_gla_engergy_lost_goto_optim_gain_en;           /* adi,fagc-rst-gla-engergy-lost-goto-optim-gain-enable */
    uint32_t    fagc_rst_gla_engergy_lost_sig_thresh_below_ll;          /* adi,fagc-rst-gla-engergy-lost-sig-thresh-below-ll */
    uint32_t    fagc_energy_lost_stronger_sig_gain_lock_exit_cnt;       /* adi,fagc-energy-lost-stronger-sig-gain-lock-exit-cnt */
    uint8_t     fagc_rst_gla_large_adc_overload_en;                     /* adi,fagc-rst-gla-large-adc-overload-enable */
    uint8_t     fagc_rst_gla_large_lmt_overload_en;                     /* adi,fagc-rst-gla-large-lmt-overload-enable */
    uint8_t     fagc_rst_gla_en_agc_pulled_high_en;                     /* adi,fagc-rst-gla-en-agc-pulled-high-enable */
    uint32_t    fagc_rst_gla_if_en_agc_pulled_high_mode;                /* adi,fagc-rst-gla-if-en-agc-pulled-high-mode */
    uint32_t    fagc_power_measurement_duration_in_state5;              /* adi,fagc-power-measurement-duration-in-state5 */
    /* RSSI Control */
    uint32_t    rssi_delay;                                             /* adi,rssi-delay */
    uint32_t    rssi_duration;                                          /* adi,rssi-duration */
    uint8_t     rssi_restart_mode;                                      /* adi,rssi-restart-mode */
    uint8_t     rssi_unit_is_rx_samples_enable;                         /* adi,rssi-unit-is-rx-samples-enable */
    uint32_t    rssi_wait;                                              /* adi,rssi-wait */
    /* Temperature Sensor Control */
    uint32_t    temp_sense_decimation;                                  /* adi,temp-sense-decimation */
    uint16_t    temp_sense_measurement_interval_ms;                     /* adi,temp-sense-measurement-interval-ms */
    int8_t      temp_sense_offset_signed;                               /* adi,temp-sense-offset-signed */
    uint8_t     temp_sense_periodic_measurement_enable;                 /* adi,temp-sense-periodic-measurement-enable */
    /* Control Out Setup */
    uint8_t     ctrl_outs_enable_mask;                                  /* adi,ctrl-outs-enable-mask */
    uint8_t     ctrl_outs_index;                                        /* adi,ctrl-outs-index */
}Radio640_stInitParam;

/** @name Radio640 Error codes
 *
 *  List all runtime related error codes of a Radio640 module.
 */
enum Radio640_eError {
    eRadio640Ok                     = MAKE_RESULT(eBasSeveritySuccess,   MODULE_RADIO640, 0),    ///< No error
    eRadio640NoCore                 = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 1),    ///< No matching FPGA core
    eRadio640IoError                = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 2),    ///< I/O error
    eRadio640GainError              = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 3),    ///< Cannot read the gain
    eRadio640OutOfMem               = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 4),    ///< Out of memory
    eRadio640BadAddr                = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 5),    ///< Bad address
    eRadio640NoDev                  = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 6),    ///< Device not present
    eRadio640InvArg                 = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 7),    ///< Invalid argument
    eRadio640Timeout                = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 8),    ///< Timeout
    eRadio640SpiError               = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 9),    ///< SPI error
    eRadio640MasterCalib            = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 10),   ///< Calibration from the master module is not done
    eRadio640InvHandle              = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 11),   ///< Invalid handle
    eRadio640EmbedFuncOnly          = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 12),   ///< This function is only available in the embedded library
    eRadio640HostFuncOnly           = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 13),   ///< This function is only available in the host library
    eRadio640Absent                 = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 14),   ///< The FMC board does not match the FPGA core
    eRadio640InvPtr                 = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 15),   ///< Invalid pointer
    eRadio640InvDevice              = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 16),   ///< Invalid device
    eRadio640ArgOutOfRange          = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 17),   ///< Argument value is out of range
    eRadio640ArgIsBoolean           = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 18),   ///< Argument value must only be 0 or 1
    eRadio640InvEnumValue           = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 19),   ///< Invalid enumeration value
    eRadio640NoSyncSent             = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 20),   ///< No Sync was sent to the TRX since the last MCS step
    eRadio640RefOutInvCfg           = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 21),   ///< The RefOut clock cannot be outputted in eRadio640ClkSrcMaster configuration since the clock buffer is bypassed
    eRadio640InvClkSrcMaster        = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 22),   ///< The clock source from master board cannot be selected if the Radio640 is directly connected to the carrier board.
    eRadio640InvClkSrcCarrier       = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 23),   ///< The clock source from carrier board cannot be selected if the Radio640 is not directly connected to the carrier board.
    eRadio640ClkToExtendedFmcInvCfg = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640, 24)    ///< The ClkToExtendedFmc clock cannot be outputted since the board does not have an extended  FMC connector.
};

#ifdef __cplusplus
}
#endif  // extern "C"
#endif
