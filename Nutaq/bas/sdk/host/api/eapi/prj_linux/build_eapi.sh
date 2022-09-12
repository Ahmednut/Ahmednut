#!/bin/bash
echo     ======================================================================
echo			A ............. Complicated build
echo			include
pwd
echo     ======================================================================
INCLUDES="-D__linux__ -I../inc -I../../record_playback/inc -I../../rtdex/inc -I../../rtdexsync/inc -I../../rtdexts/inc -I../../utils/inc/ -I../../../../embedded/lib/adac250_lib/inc/  -I../../../../embedded/lib/adc5000_lib/inc/ -I../../../../embedded/lib/carrier_lib/inc -I../../../../embedded/lib/radio420_lib/inc -I../../../../embedded/lib/radio640_lib/inc/ -I../../../../embedded/lib/mi250_lib/inc  -I../../../../embedded/lib/mi125_lib/inc -I../../../../embedded/lib/mo1000_lib/inc -I../../../../embedded/lib/lvdsxinxout_lib/inc -I../../../../embedded/lib/lvds_lib/inc -I../../../../embedded/lib/sysmon_lib/inc -I../../../../embedded/lib/timestamp_lib/inc -I../../../../embedded/lib/FMCEEprom_lib/inc/ -I../../../../embedded/lib/aurora_lib/inc/ -I../../../../embedded/lib/ppssync_lib/inc/ -I../../../../embedded/lib/drivers_lib/inc/ -I../../../../embedded/lib/common/inc/ -I../../../../../tools/apps/core/LoadDataToPlayback/inc -I../../../../../tools/apps/core/PlaybackData/inc -I../../../../../tools/apps/core/PlaybackStop/inc -I../../../../../tools/apps/core/RecordData/inc -I../../../../../tools/apps/core/RetrieveRecordedData/inc"


swig -python $INCLUDES  ../../../cli/swig/eapi.i
echo     ======================================================================
echo			B ............. Complicated build
echo			gcc
pwd
echo     ======================================================================
gcc -fPIC -c -O2 $(python-config --includes) $INCLUDES ../../../cli/swig/eapi_wrap.c ../../utils/src/linux_utils.c

# In some system must be : $(python-config --cflags)

echo     ======================================================================
echo			C ............. Complicated build
echo			link
pwd
echo     ======================================================================
ld --build-id -shared eapi_wrap.o linux_utils.o libeapi.a librtdex.a librtdexsync.a librtdexts.a librecplay.a -lc -lrt -lpcap -o _eapi.so



