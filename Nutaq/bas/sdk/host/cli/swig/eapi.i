
%module eapi
%{
#include <cce.h>
#include <bas_error.h>
#include <bas_types.h>

uint8_t err_lvl;
uint8_t VERBOSE_CTRL;

#include <eapi.h>
#include <eapi_serialize.h>
#include <eapi_adac250.h>
#include <eapi_adc5000.h>
#include <eapi_aurora.h>
#include <eapi_fmcvita.h>
#include <eapi_lvds.h>
#include <eapi_lvdsxinxout.h>
#include <eapi_mi125.h>
#include <eapi_mi250.h>
#include <eapi_mo1000.h>
#include <eapi_osconfig.h>
#include <eapi_ppssync.h>
#include <eapi_radio420.h>
#include <eapi_sysmon.h>
#include <eapi_timestamp.h>
#include <eapi_carrier.h>

#include <adac250_defines.h>
#include <adac250_types.h>
#include <adc5000_defines.h>
#include <aurora_defines.h>
#include <lvds_defines.h>
#include <MI125_mi125_defines.h>
#include <mi250_types.h>
#include <Mo1000_defines.h>
#include <Ad9148_defines.h>
#include <Cdce62005_defines.h>
#include <ev10aq190_defines.h>
#include <fmclvds_defines.h>
#include <fmclvds_pcf8574_defines.h>
#include <lms6002_defines.h>
#include <fmc_radio.h>
#include <fmc_radio_defines.h>
#include <radio420_cdce62005_defines.h>
#include <sysmon_def.h>
#include <timestamp_defines.h>
#include <carrier_service_def.h>
#include <carrier_fmc_service_def.h>

#include "media_types.h"
#include "rtdex_types.h"
#include "rtdex_eth_types.h"
#include "rtdex_media.h"
#include "rtdex_access.h"
#include "rtdex.h"
#include "rtdex_tcpip.h"
#include "recplay_types.h"
#include "recplay.h"

#include <PlaybackData.h>
#include <PlaybackStop.h>
#include <RecordData.h>
#include <RetrieveRecordedData.h>
#include <LoadDataToPlayback.h>

#include <media_utils.h>

%}

%include "typemaps.i"
%include "cstring.i"
%include "carrays.i"
%include "cdata.i"
%array_class(int, intArray);
%apply unsigned *OUTPUT { unsigned * index };
%apply unsigned *OUTPUT { unsigned * data };
%apply unsigned *OUTPUT { unsigned * size };
%apply unsigned *OUTPUT { unsigned * addr };
%cstring_output_allocate_size(char ** name, unsigned int * name_length, free(*$1));
%cstring_output_allocate_size(char ** type, unsigned int * type_length, free(*$1));
%cstring_output_allocate_size(char ** date, unsigned int * date_length, free(*$1));
%cstring_output_allocate_size(char ** hour, unsigned int * hour_length, free(*$1));
%cstring_output_allocate_size(char ** custom, unsigned int * custom_length, free(*$1));
%apply int *OUTPUT { int * locked };
%apply int *INOUT { int * buffsize };
%apply int *OUTPUT {ADAC250_ADC_STATUSES  * status};
%apply unsigned char *OUTPUT {unsigned char* pTrigOffset};

%cstring_output_allocate_size(char ** list, int * list_size, free(*$1));
%cstring_output_allocate_size(char ** buf, int * bufsize, buf_free(*$1));
%cstring_output_allocate_size(char ** data, int * size, free(*$1));

%apply unsigned char *OUTPUT {unsigned char *ndevices};
%cstring_chunk_output(I2C_stI2cScan *scanraw, sizeof(I2C_stI2cScan));
typedef int Result;
%include <connection_state.h>
%include <eapi.h>
%include <eapi_serialize.h>
%include <eapi_adac250.h>
%include <adac250_defines.h>
%include <adac250_types.h>

%cstring_output_allocate_size(char ** osconfig_outkey, int * osconfig_outkeysize, buf_free(*$1));
%cstring_output_allocate_size(char ** osconfig_outvalue, int * osconfig_outvaluesize, buf_free(*$1));
%include <eapi_osconfig.h>

%apply int *OUTPUT { ad9517_lock_status * lockstatus };
%apply int *OUTPUT  { unsigned char * adc_A_active };
%apply int *OUTPUT  { unsigned char * adc_B_active };
%apply int *OUTPUT  { unsigned char * adc_C_active };
%apply int *OUTPUT  { unsigned char * adc_D_active };
%apply int *OUTPUT { int * temperature};
%include <eapi_adc5000.h>
%include <adc5000_defines.h>
%include <ad9517_defines.h>
%include <ev10aq190_defines.h>

%apply int *OUTPUT  { unsigned int * pVersion };
%apply int *OUTPUT  { unsigned int * pStatus };
%apply unsigned long long *OUTPUT  { uint64_t * pValue };
%apply unsigned long long *OUTPUT  { uint32_t * pValue };
%apply int *OUTPUT  { uint8_t * pTxDiffCtrl };
%apply int *OUTPUT  { uint8_t * pTxPostEmphasis };
%apply int *OUTPUT  { uint8_t * pTxPreEmphasis };
%apply int *OUTPUT  { uint8_t * pRxEqMix };
%apply int *OUTPUT  { uint8_t * pDfeTap1 };
%apply int *OUTPUT  { uint8_t * pLane0 };
%apply int *OUTPUT  { uint8_t * pLane1 };
%apply int *OUTPUT  { uint8_t * pLane2 };
%apply int *OUTPUT  { uint8_t * pLane3 };
%include <eapi_aurora.h>
%include <aurora_defines.h>

%apply int *OUTPUT { FMCLVDS_CONFIG *lvdsconfig};
%include <eapi_lvdsxinxout.h>
%include <fmclvds_defines.h>
%include <fmclvds_pcf8574_defines.h>

%apply int *OUTPUT { unsigned int *value};
%include <eapi_lvds.h>
%include <lvds_defines.h>

%apply float *OUTPUT { float *pValue};
%include <eapi_sysmon.h>
%include <sysmon_def.h>

%apply unsigned long long *OUTPUT { uint64_t * pu64Timestamp};
%include <eapi_timestamp.h>
%include <timestamp_defines.h>

%apply int *OUTPUT { MI250_PLL_STATUS * locked};
%include <eapi_mi250.h>
%include <mi250_types.h>

%apply unsigned char *OUTPUT { unsigned char *detected };
%apply short *OUTPUT { short *temp};
%apply unsigned int *OUTPUT { unsigned int *channellanecalib};
%apply unsigned int *OUTPUT { unsigned int *channelcalibstatus};
%apply unsigned int *OUTPUT { unsigned int *core_version};
%apply unsigned int *OUTPUT { unsigned int *drv_version};
%apply unsigned char *OUTPUT { unsigned char *written };
%apply unsigned char *OUTPUT { unsigned char *verify };
%apply unsigned int *OUTPUT { unsigned int *master};
%apply unsigned char *OUTPUT { unsigned char *detect_ok };
%apply unsigned char *OUTPUT { unsigned char *ndevices };
%cstring_chunk_output(MI125_mi125_i2cscan *scanraw, sizeof(MI125_mi125_i2cscan));
%cstring_chunk_output(MI125_mi125_i2caddr *scan_res, sizeof(MI125_mi125_i2caddr));
%apply unsigned char *OUTPUT { unsigned char *result_ok };
%include <eapi_mi125.h>
%include <MI125_mi125_defines.h>

%cstring_chunk_output(Mo1000_stBoardStatus *pStatus, sizeof(Mo1000_stBoardStatus));
%cstring_chunk_output(Mo1000_stCompareErrors *pCompare, sizeof(Mo1000_stCompareErrors));
%apply unsigned int *OUTPUT { unsigned int *puValue};
%apply short *OUTPUT { short *psTemp};
%apply short *OUTPUT { short *psDac1};
%apply short *OUTPUT { short *psDac2};
%apply unsigned int *OUTPUT { unsigned int *puChannelLaneCalib};
%apply unsigned int *OUTPUT { unsigned int *puChannelFrameCalib};
%apply unsigned int *OUTPUT { unsigned int *puChannelSyncCalib};
%apply unsigned int *OUTPUT { unsigned int *puCalibStatus};
%apply unsigned int *OUTPUT { unsigned int *puMaster};
%apply unsigned int *OUTPUT { unsigned int *puCoreVersion};
%apply unsigned int *OUTPUT { unsigned int *puDriverVersion};
%apply unsigned int *OUTPUT { unsigned int *puResult};
%apply (char *STRING, int LENGTH) { (Mo1000_stI2cScan *pScanRaw, int iScanRawSize) };
%cstring_chunk_output(Mo1000_stI2cAddr *pScanRes, sizeof(Mo1000_stI2cAddr));
%apply unsigned char *OUTPUT { unsigned char *pucDetectOk };
%apply unsigned short *OUTPUT { unsigned short *pusCoreRefClock};
%apply unsigned short *OUTPUT { unsigned short *pusDacRefClock};
%apply unsigned {Cdce62005_eOutRefMode};
%apply unsigned {Cdce62005_eR2Values};
%apply unsigned {Cdce62005_eC2Values};
%apply unsigned {Cdce62005_eR3Values};
%apply unsigned {Cdce62005_eC3Values};
%apply unsigned {Cdce62005_eC1Values};
%apply unsigned {Cdce62005_eChargePump};
%include <eapi_mo1000.h>
%include <Mo1000_defines.h>
%include <Cdce62005_defines.h>
%include <Ad9148_defines.h>

%apply short *OUTPUT { Carrier_eType_t * pType};
%apply unsigned int *OUTPUT { unsigned int * addr};
%apply unsigned int *OUTPUT { unsigned int * value};
%apply short *OUTPUT { Carrier_eClkState_t * clkstate};
%apply short *OUTPUT { Carrier_eTClkSrc_t * source};
%apply short *OUTPUT { Carrier_eFmcBidirClkConnection_t * connection};
%apply short *OUTPUT { Carrier_eClkState_t * clkstate};
%apply short *OUTPUT { Carrier_eFClkAConnection_t * connection};

%include <eapi_carrier.h>
%include <carrier_service_def.h>

%cstring_chunk_output(char *mystring, EAPI_FMCVITA_MAXSTRING);
%cstring_chunk_output(FMCEEprom_eepromdata *oepdata, sizeof(FMCEEprom_eepromdata));
%include <eapi_fmcvita.h>

%apply short *OUTPUT {short * iTemp};
%apply short *OUTPUT {short * pPower};

%apply unsigned int *OUTPUT {unsigned int *idcode};
%apply unsigned int *OUTPUT {unsigned int *ppsver};
%apply unsigned int *OUTPUT {unsigned int *ref_dac_val};
%apply unsigned int *OUTPUT {unsigned int *vcxo_freq};
%apply float *OUTPUT {float *Kp};
%apply float *OUTPUT {float *Ki};
%cstring_chunk_output(PPSSync_Info_t *PPSSyncInfo, sizeof(PPSSync_Info_t));
%include "eapi_ppssync.h"
%apply unsigned *OUTPUT {unsigned * spidata};
%apply int *OUTPUT {int * pllstatus};
%include <fmc_radio_defines.h>
%include <radio420_cdce62005_defines.h>
%include <lms6002_defines.h>
%include <fmc_radio_common.h>
%include <eapi_radio420.h>



%include <bas_error.h>
%include <cce.h>

typedef char  		  	    int8_t;
typedef short 			    int16_t;       
typedef int  			    int32_t;         
typedef long long   	    int64_t;       
typedef unsigned char 	    uint8_t;         
typedef unsigned short      uint16_t;  
typedef unsigned int 	    uint32_t;       
typedef unsigned long long  uint64_t;
%include <bas_types.h>

%include "media_types.h"
%include "rtdex_types.h"
%include "rtdex_eth_types.h"
%include "rtdex_media.h"
%include "rtdex_access.h"
%include "rtdex.h"
%include "rtdex_tcpip.h"
%include "recplay_types.h"
%include "recplay.h"

%include "RecordData.h"
%include "PlaybackData.h"
%include "PlaybackStop.h"
%include "RetrieveRecordedData.h"
%include "LoadDataToPlayback.h"

#include <Winsock2.h>
#include <Ws2tcpip.h>
