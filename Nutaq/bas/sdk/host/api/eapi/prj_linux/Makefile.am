CFLAGS = -Werror-implicit-function-declaration -fPIC
# MMEK I had to define ONLY_CALCULATE to make com_wrapper.c compile -D__linux__
AM_CFLAGS = -Iinclude -D__EAPI_BUILD__ -D__ONLY_CALCULATE__ -DONLY_CALCULATE -D__linux__ -D_MODULE_IDENT_
AUTOMAKE_OPTIONS = subdir-objects

### LIBRAIRIES ###
lib_LTLIBRARIES    = libeapi.la librtdex.la librtdexsync.la librtdexts.la librecplay.la
lib_LIBRARIES      = libeapi.a librtdex.a librtdexsync.a librtdexts.a librecplay.a

.PHONY: print_echo

print_echo:
	echo **********.............. makefile.am .........***********************
# ***************** rtdex ***********************
librtdex_la_SOURCES = ../../rtdex/src/rtdex.c ../../rtdex/src/rtdex_eth.c ../../rtdex/src/rtdex_pci.c ../../../pcie/src/rtdex_pcie.c ../../rtdex/src/rtdex_tcpip.c ../../rtdex/src/rtdex_axi.c ../../eapi/src/bas_time.c
librtdex_la_CFLAGS = $(AM_CFLAGS) -lrt

librtdex_a_SOURCES = ../../rtdex/src/rtdex.c ../../rtdex/src/rtdex_eth.c ../../rtdex/src/rtdex_pci.c ../../../pcie/src/rtdex_pcie.c ../../rtdex/src/rtdex_tcpip.c ../../rtdex/src/rtdex_axi.c ../../eapi/src/bas_time.c
librtdex_a_CFLAGS = $(AM_CFLAGS) -lrt

# ***************** rtdexsync ***********************
librtdexsync_la_SOURCES = ../../rtdexsync/src/rtdexsync.c
librtdexsync_la_CFLAGS = $(AM_CFLAGS) -lrt

librtdexsync_a_SOURCES = ../../rtdexsync/src/rtdexsync.c
librtdexsync_a_CFLAGS = $(AM_CFLAGS) -lrt

# ***************** rtdexts ***********************
librtdexts_la_SOURCES = ../../rtdexts/src/rtdexts.c
librtdexts_la_CFLAGS = $(AM_CFLAGS) -lrt

librtdexts_a_SOURCES = ../../rtdexts/src/rtdexts.c
librtdexts_a_CFLAGS = $(AM_CFLAGS) -lrt

# ***************** recplay ***********************
librecplay_la_SOURCES = ../../record_playback/src/recplay.c
librecplay_la_CFLAGS = $(AM_CFLAGS)

librecplay_a_SOURCES = ../../record_playback/src/recplay.c
librecplay_a_CFLAGS = $(AM_CFLAGS)

# ***************** eapi ***********************
libeapi_a_SOURCES = ../../eapi/src/bas_time.c ../../eapi/src/bas_error.c ../src/eapi.c ../src/eapi_serialize.c ../src/eapi_adac250.c ../../../../embedded/lib/adac250_lib/src/adac250_serialize.c ../../../../embedded/lib/adac250_lib/src/adac250_pll.c ../src/connection_state.c ../src/eapi_fmcomms3.c ../../../../embedded/lib/fmcomms3_lib/src/fmcomms3_serialize.c ../src/eapi_radio640.c ../../../../embedded/lib/radio640_lib/src/Radio640DevCom.c ../../../../embedded/lib/radio640_lib/src/Radio640Serialize.c ../../../../embedded/lib/radio640_lib/src/Radio640Spi.c ../src/eapi_radio420.c ../../../../embedded/lib/radio420_lib/src/fmc_radio.c ../../../../embedded/lib/radio420_lib/src/radio420_cdce62005.c ../../../../embedded/lib/radio420_lib/src/lms6002.c ../../../../embedded/lib/radio420_lib/src/fmcradio_serialize.c ../src/eapi_adc5000.c ../src/eapi_osconfig.c ../../../../embedded/lib/adc5000_lib/src/adc5000_serialize.c ../src/eapi_carrier.c ../../../../embedded/lib/adc5000_lib/src/ad9517.c  ../../../../embedded/lib/mi250_lib/src/mi250_serialize.c ../../../../embedded/lib/mi250_lib/src/mi250_pll.c ../src/eapi_mi250.c ../../../../embedded/lib/mi125_lib/src/mi125_serialize.c ../src/eapi_mi125.c ../src/eapi_lvdsxinxout.c ../src/eapi_sysmon.c ../src/eapi_timestamp.c ../src/eapi_lvds.c ../src/eapi_fmcvita.c ../src/eapi_aurora.c  ../src/eapi_ppssync.c ../src/eapi_mo1000.c ../../../../../tools/apps/core/PlaybackData/src/PlaybackData.c ../../../../../tools/apps/core/PlaybackStop/src/PlaybackStop.c ../../../../../tools/apps/core/RecordData/src/RecordData.c ../../../../../tools/apps/core/LoadDataToPlayback/src/LoadDataToPlayback.c ../../../../../tools/apps/core/RetrieveRecordedData/src/RetrieveRecordedData.c ../../utils/src/mac_utils.c ../../utils/src/media_utils.c ../../utils/src/linux_utils.c

libeapi_a_CFLAGS = $(AM_CFLAGS)

libeapi_la_SOURCES = ../../eapi/src/bas_time.c ../../eapi/src/bas_error.c ../src/eapi.c ../src/eapi_serialize.c ../src/eapi_adac250.c ../../../../embedded/lib/adac250_lib/src/adac250_serialize.c ../../../../embedded/lib/adac250_lib/src/adac250_pll.c ../src/connection_state.c ../src/eapi_fmcomms3.c ../../../../embedded/lib/fmcomms3_lib/src/fmcomms3_serialize.c ../src/eapi_radio640.c ../../../../embedded/lib/radio640_lib/src/Radio640DevCom.c ../../../../embedded/lib/radio640_lib/src/Radio640Serialize.c ../../../../embedded/lib/radio640_lib/src/Radio640Spi.c ../src/eapi_radio420.c ../../../../embedded/lib/radio420_lib/src/fmc_radio.c ../../../../embedded/lib/radio420_lib/src/radio420_cdce62005.c ../../../../embedded/lib/radio420_lib/src/lms6002.c ../../../../embedded/lib/radio420_lib/src/fmcradio_serialize.c ../src/eapi_adc5000.c ../src/eapi_osconfig.c ../../../../embedded/lib/adc5000_lib/src/adc5000_serialize.c ../src/eapi_carrier.c ../../../../embedded/lib/adc5000_lib/src/ad9517.c  ../../../../embedded/lib/mi250_lib/src/mi250_serialize.c ../../../../embedded/lib/mi250_lib/src/mi250_pll.c ../src/eapi_mi250.c ../../../../embedded/lib/mi125_lib/src/mi125_serialize.c ../src/eapi_mi125.c ../src/eapi_lvdsxinxout.c ../src/eapi_sysmon.c ../src/eapi_timestamp.c ../src/eapi_lvds.c ../src/eapi_fmcvita.c ../src/eapi_aurora.c  ../src/eapi_ppssync.c ../src/eapi_mo1000.c ../../../../../tools/apps/core/PlaybackData/src/PlaybackData.c ../../../../../tools/apps/core/PlaybackStop/src/PlaybackStop.c ../../../../../tools/apps/core/RecordData/src/RecordData.c ../../../../../tools/apps/core/LoadDataToPlayback/src/LoadDataToPlayback.c ../../../../../tools/apps/core/RetrieveRecordedData/src/RetrieveRecordedData.c ../../utils/src/mac_utils.c ../../utils/src/media_utils.c ../../utils/src/linux_utils.c

libeapi_la_CFLAGS = $(AM_CFLAGS)

### INCLUDES ###
DEFAULT_INCLUDES = -I../../utils/inc -I../../rtdex/inc -I../../rtdexsync/inc -I../../rtdexsync/src -I../../rtdexts/inc -I../../rtdexts/src -I../../../pcie/inc -I../../record_playback/inc/ -I/usr/local/include -I../inc/ -I../../../../embedded/lib/adac250_lib/inc  -I../../../../embedded/lib/carrier_lib/inc/ -I../../../../embedded/lib/fmcomms3_lib/inc -I../../../../embedded/lib/radio640_lib/inc -I../../../../embedded/lib/radio420_lib/inc  -I../../../../embedded/lib/adc5000_lib/inc -I../../../../embedded/lib/mi250_lib/inc -I../../../../embedded/lib/mi125_lib/inc -I../../../../embedded/lib/lvdsxinxout_lib/inc -I../../../../embedded/lib/lvds_lib/inc -I../../../../embedded/lib/sysmon_lib/inc -I../../../../embedded/lib/timestamp_lib/inc -I../../../../embedded/lib/drivers_lib/inc  -I../../../../embedded/lib/FMCEEprom_lib/inc -I../../../../embedded/lib/aurora_lib/inc  -I../../../../embedded/lib/ppssync_lib/inc -I../../../../embedded/lib/mo1000_lib/inc -I../../../../embedded/lib/common/inc -I../../../../../tools/apps/core/LoadDataToPlayback/inc -I../../../../../tools/apps/core/PlaybackData/inc -I../../../../../tools/apps/core/PlaybackStop/inc -I../../../../../tools/apps/core/RecordData/inc -I../../../../../tools/apps/core/RetrieveRecordedData/inc
