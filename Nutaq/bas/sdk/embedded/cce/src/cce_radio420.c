
#include <unistd.h>
#include "cce.h"
#include "eapi_serialize.h"
#include "bas_debug.h"

#include "cce_radio420.h"
#include "fmc_radio.h"
#include "fmc_radio_calibration.h"
#include "fmcradio_serialize.h"

#include "FMCEEprom_eeprom_id.h"
#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_fmc_service.h"
#include "carrier_fmc_service_def.h"

extern Carrier_handle_t hCarrier;
extern unsigned selected_fmc_radio;
extern fmc_radio fmc_radio_handle[];

static FMCEEprom_module_types _modlst[]={FMCEEPROM_EEPROM_MOD_FMCRADIO,
                                         FMCEEPROM_EEPROM_MOD_FMCRADIOE, FMCEEPROM_EEPROM_MOD_FMCRADIOS,
                                         FMCEEPROM_EEPROM_MOD_FMCRADIOE1V8, FMCEEPROM_EEPROM_MOD_FMCRADIOS1V8 };

Result _FmcRadio_Presence()
{
    Result res;
    int iConnector, iBoard;
    CARRIER_detected mdetected = CARRIER_NOT_DETECTED;

    iBoard = selected_fmc_radio % CARRIER_FMC_MAXBOARD;
    iConnector = selected_fmc_radio / CARRIER_FMC_MAXBOARD;

    res = Carrier_FmcValidatePresence(hCarrier,
        iConnector, iBoard,
        _modlst, sizeof(_modlst)/sizeof(FMCEEprom_module_types),
        &mdetected);
    if FAILURE(res) return res;

    if (mdetected == CARRIER_NOT_DETECTED) return CCEINVALIDMOD;

    return OK;
}

Result fmc_radio_presence(connection_state * state)
{
    return _FmcRadio_Presence();
}

Result fmc_radio_powerup_recv(connection_state * state)
{
    Result res;
    int iConnector, iBoard;

    iBoard = selected_fmc_radio % CARRIER_FMC_MAXBOARD;
    iConnector = selected_fmc_radio / CARRIER_FMC_MAXBOARD;

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    fmc_radio_disable_clock(&fmc_radio_handle[0]);

    res = Carrier_FmcPowerUp(hCarrier,
        iConnector, iBoard,
        _modlst, sizeof(_modlst)/sizeof(FMCEEprom_module_types),
        VADJ_2_503V);

    return res;
}

Result fmc_radio_sdr_reset_recv(connection_state * state)
{
    Result res;

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_sdr_reset(&fmc_radio_handle[selected_fmc_radio]);

    return res;
}

Result fmc_radio_sdr_select_recv(connection_state * state)
{
    unsigned nb;
    Result res;
    unsigned int uiFmcMaxNum;

    nb = uint_recv(state);

    res = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if(FAILURE(res)) return res;

    if( nb >= uiFmcMaxNum)
    {
        return CCEINVALIDVALUE;
    }

    selected_fmc_radio = nb;

    return OK;
}

Result fmc_radio_spi_write_recv(connection_state * state)
{
    FMCRADIO_DEVICES device;
    unsigned addr;
    unsigned data;
    Result res;

    device = uint_recv(state);
    addr = uint_recv(state);
    data = uint_recv(state);
    debug("Radio420x SPI Write device=%x addr=%x data=%x\n", device, addr, data);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_spi_write(&fmc_radio_handle[selected_fmc_radio], device, addr, data);

    return res;
}

Result fmc_radio_spi_read_recv(connection_state * state)
{
    Result res;
    FMCRADIO_DEVICES device;
    unsigned addr;
    unsigned data = 0;

    device = uint_recv(state);
    addr = uint_recv(state);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        uint_send(state, data);
        return res;
    }

    res = fmc_radio_spi_read(&fmc_radio_handle[selected_fmc_radio], device, addr, &data);

    uint_send(state, data);
    debug("Radio420x SPI Read device=%x addr=%x data=%x\n", device, addr, data);
    return res;
}

Result fmc_radio_pll_setparam_recv(connection_state * state)
{
    Result res;
    cdce62005_param param;

    cdce62005_param_recv(state, &param);
    debug("out_freq[0]: %i\n", param.out_freq[0]);
    debug("out_freq[1]: %i\n", param.out_freq[1]);
    debug("out_freq[2]: %i\n", param.out_freq[2]);
    debug("out_freq[3]: %i\n", param.out_freq[3]);
    debug("out_freq[4]: %i\n", param.out_freq[4]);
    debug("pfd_freq: %i\n", param.pfd_freq);
    debug("ref_freq: %i\n", param.ref_freq);
    debug("synth_freq: %i\n", param.synth_freq);
    debug("feedback_divider: %i\n", param.feedback_divider);
    debug("input_divider: %i\n", param.input_divider);
    debug("out_divider[0]: %i\n", param.out_divider[0]);
    debug("out_divider[1]: %i\n", param.out_divider[1]);
    debug("out_divider[2]: %i\n", param.out_divider[2]);
    debug("out_divider[3]: %i\n", param.out_divider[3]);
    debug("out_divider[4]: %i\n", param.out_divider[4]);
    debug("prescale_divider: %i\n", param.prescale_divider);
    debug("secondary_clock: %i\n", param.secondary_clock);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_pll_setparam(&fmc_radio_handle[selected_fmc_radio], &param);

    return res;
}

Result fmc_radio_pll_lockstatus_recv(connection_state * state)
{
    Result res;
    pll_lock_status pll_state = CDCE62005_PLL_UNLOCKED;

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        uint_send(state, pll_state);
        return res;
    }

    res = fmc_radio_pll_lockstatus(&fmc_radio_handle[selected_fmc_radio], &pll_state);

    uint_send(state, pll_state);
    return res;
}

Result fmc_radio_pll_waitforlock_recv(connection_state * state)
{
    Result res;

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_pll_waitforlock(&fmc_radio_handle[selected_fmc_radio]);

    return res;
}

Result fmc_radio_clockmux_set_recv(connection_state * state)
{
    FMCRADIO_CLOCK_OUTPUT_ID dest;
    FMCRADIO_CLOCK_INPUT_ID src;
    Result res;

    dest = uint_recv(state);
    src = uint_recv(state);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_clockmux_set(&fmc_radio_handle[selected_fmc_radio], dest, src);

    return res;
}

Result fmc_radio_lime_setpllparam_recv(connection_state * state)
{
    LMS6002_DIR dir;
    lms6002_pll_param param;
    Result res;

    dir = uint_recv(state);
    lms6002_pll_param_recv(state, &param);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_lime_setpllparam(&fmc_radio_handle[selected_fmc_radio], dir, &param);

    return res;
}

Result fmc_radio_lime_calibratePLL_recv(connection_state * state)
{
    LMS6002_DIR pll;
    Result res;

    pll = uint_recv(state);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_lime_calibratePLL(&fmc_radio_handle[selected_fmc_radio], pll);

    return res;
}

Result fmc_radio_lime_reset_recv(connection_state * state)
{
    Result res;

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_lime_reset(&fmc_radio_handle[selected_fmc_radio]);

    return res;
}

Result fmc_radio_sdr_band_recv(connection_state * state)
{
    FMC_RADIO_SDR_BAND band;
    Result res;

    band = uint_recv(state);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_sdr_band(&fmc_radio_handle[selected_fmc_radio], band);

    return res;
}

Result fmc_radio_sdr_rx_gain_recv(connection_state * state)
{
    LMS6002_LNA_GAIN lna_gain;
    LMS6002_VGA1_GAIN rx_gain1;
    unsigned rx_gain2;
    unsigned rx_gain3;
    Result res;

    lna_gain = uint_recv(state);
    rx_gain1 = uint_recv(state);
    rx_gain2 = uchar_recv(state);
    rx_gain3 = uchar_recv(state);

    debug("lna_gain: %i\n", lna_gain);
    debug("rx_gain1: %i\n", rx_gain1);
    debug("rx_gain2: %i\n", rx_gain2);
    debug("rx_gain3: %i\n", rx_gain3);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_sdr_rx_gain(&fmc_radio_handle[selected_fmc_radio], lna_gain, rx_gain1, rx_gain2, rx_gain3);

    return res;
}

Result fmc_radio_rxvga_calibrate_recv(connection_state * state)
{
    Result res;

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_rxvga_calibrate(&fmc_radio_handle[selected_fmc_radio]);

    return res;
}

Result fmc_radio_tx_gain_recv(connection_state * state)
{
    int tx_vga1_gain;
    unsigned tx_vga2_gain;
    unsigned tx_gain3;
    Result res;

    tx_vga1_gain = uchar_recv(state);
    tx_vga2_gain = uchar_recv(state);
    tx_gain3 = uchar_recv(state);

    debug("tx_vga1_gain: %i\n", tx_vga1_gain);
    debug("tx_vga2_gain: %i\n", tx_vga2_gain);
    debug("tx_gain3: %i\n", tx_gain3);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_tx_gain(&fmc_radio_handle[selected_fmc_radio], tx_vga1_gain, tx_vga2_gain, tx_gain3);

    return res;
}

Result fmc_radio_lime_setrxfilter_recv(connection_state * state)
{
    FMCRADIOSDR_FILTER_BANK bank;
    Result res;

    bank = uint_recv(state);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_sdr_setrxfilter(&fmc_radio_handle[selected_fmc_radio], bank);

    return res;
}

Result fmc_radio_lpf_set_recv(connection_state * state)
{
    LMS6002_DIR dir;
    LMS6002_LPF_BANDWIDTH bwt;
    Result res;

    dir = uint_recv(state);
    bwt = uint_recv(state);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_lpf_set(&fmc_radio_handle[selected_fmc_radio], dir, bwt);

    return res;
}

Result fmc_radio_lpf_calibrate_recv(connection_state * state)
{
    unsigned ref_freq;
    LMS6002_DIR dir;
    Result res;

    ref_freq = uint_recv(state);
	dir = uint_recv(state);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_lpf_calibrate(&fmc_radio_handle[selected_fmc_radio], dir, ref_freq);

    return res;
}

Result fmc_radio_setIOexp_recv(connection_state * state)
{
    unsigned data;
    unsigned mask;
    Result res;

    data = uint_recv(state);
    mask = uint_recv(state);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_setIOexp(&fmc_radio_handle[selected_fmc_radio], data, mask);

    return res;
}

Result fmc_radio_path_enable_recv(connection_state * state)
{
    LMS6002_DIR dir;
    Result res;

    dir = uint_recv(state);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_path_enable(&fmc_radio_handle[selected_fmc_radio], dir);

    return res;
}

Result fmc_radio_path_disable_recv(connection_state * state)
{
    LMS6002_DIR dir;
    Result res;

    dir = uint_recv(state);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_path_disable(&fmc_radio_handle[selected_fmc_radio], dir);

    return res;
}

Result fmc_radio_rx_connect_recv(connection_state * state)
{
    LMS6002_BAND band;
    Result res;

    band = uint_recv(state);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }

    res = fmc_radio_rx_connect(&fmc_radio_handle[selected_fmc_radio], band);

    return res;
}

Result fmc_radio_rx_disconnect_recv(connection_state * state)
{
    LMS6002_BAND band;
    Result res;

    band = uint_recv(state);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }
    
    res = fmc_radio_rx_disconnect(&fmc_radio_handle[selected_fmc_radio], band);

    return res;
}

Result fmc_radio_loopback_recv(connection_state * state)
{
    LMS6002_LOOPBACK_MODE mode;
    Result res;

    mode = uint_recv(state);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }
      
    res = fmc_radio_loopback(&fmc_radio_handle[selected_fmc_radio], mode);

    return res;
}

Result fmc_radio_lo_leakage_calibration_recv(connection_state * state)
{
    Result res;
    unsigned acq_freq ;
    unsigned carrier_freq;
    lms6002_lo_leakage_calib calib;

    calib.i_adjust = 0;
    calib.q_adjust = 0;

    carrier_freq = uint_recv(state);
    acq_freq = uint_recv(state);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        lms6002_lo_leakage_calib_send(state, &calib);
        return res;
    }
    
    res = fmc_radio_lo_leakage_calibration(&fmc_radio_handle[selected_fmc_radio], carrier_freq, acq_freq, &calib);

    lms6002_lo_leakage_calib_send(state, &calib);
    return res;
}

Result fmc_radio_lo_leakage_calibration_set_recv(connection_state * state)
{
    lms6002_lo_leakage_calib calib;
    Result res;

    lms6002_lo_leakage_calib_recv(state, &calib);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }
    
    res = fmc_radio_lo_leakage_calibration_set(&fmc_radio_handle[selected_fmc_radio], &calib);

    return res;
}

Result fmc_radio_ssb_calibration_recv(connection_state * state)
{
    Result res;
    unsigned acq_freq ;
    unsigned carrier_freq;
    lms6002_ssb_calib calib;

    calib.phase_offset = 0;
    calib.gain_offset = 0;
    calib.backoff_offset = 0;

    carrier_freq = uint_recv(state);
    acq_freq = uint_recv(state);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        lms6002_ssb_calib_send(state, &calib);
        return res;
    }
    
    res = fmc_radio_ssb_calibration(&fmc_radio_handle[selected_fmc_radio], carrier_freq, acq_freq, &calib);

    lms6002_ssb_calib_send(state, &calib);
    return res;
}

Result fmc_radio_ssb_calibration_set_recv(connection_state * state)
{
    lms6002_ssb_calib calib;
    Result res;

    lms6002_ssb_calib_recv(state, &calib);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }
    
    res = fmc_radio_ssb_calibration_set(&fmc_radio_handle[selected_fmc_radio], &calib);

    return res;
}

Result fmc_radio_rx_dc_offset_calibration_recv(connection_state * state)
{
    Result res;
    lms6002_rx_dc_calib calib;

    calib.i_comp = 0;
    calib.q_comp = 0;

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        lms6002_rx_dc_calib_send(state, &calib);
        return res;
    }
    
    res = fmc_radio_rx_dc_offset_calibration(&fmc_radio_handle[selected_fmc_radio], &calib);

    lms6002_rx_dc_calib_send(state, &calib);
    return res;
}

Result fmc_radio_rx_dc_offset_calibration_set_recv(connection_state * state)
{
    lms6002_rx_dc_calib calib;
    Result res;

    lms6002_rx_dc_calib_recv(state, &calib);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }
    
    res = fmc_radio_rx_dc_offset_calibration_set(&fmc_radio_handle[selected_fmc_radio], &calib);

    return res;
}

Result fmc_radio_setrevision_recv(connection_state * state)
{
    FMCRADIO_REV revision;
    Result res;

    revision = uint_recv(state);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }
      
    res = fmc_radio_setrevision(&fmc_radio_handle[selected_fmc_radio], revision);

    return res;
}

Result fmc_radio_spi_control_set_recv(connection_state * state)
{
	Result res;
	unsigned control, spi_controller;

	control = uint_recv(state);
	spi_controller = uint_recv(state);

    res = _FmcRadio_Presence();
    if(FAILURE(res))
    {
        return res;
    }
    
    res = fmc_radio_spi_ctrl(&fmc_radio_handle[selected_fmc_radio], control, spi_controller);

    return res;
}

