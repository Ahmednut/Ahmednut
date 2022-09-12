APP_ROOT=/opt/Nutaq/bas/tools/bin

#Application parameters
CARRIERIPADDRESS="192.168.0.101"
RECPLAYTEST="RecplayTestPlayback.ini"
#Playback size, in bytes. Must be the same as the value specified in RecplayTestPlayback.ini
PLAYBACKSIZE="1048576"

#Configuring the Record/playback Test core 
${APP_ROOT}/RecPlayTestUtil $CARRIERIPADDRESS $RECPLAYTEST init

#Playback start
${APP_ROOT}/LoadDataToPlayback $CARRIERIPADDRESS 0 8192 0 ../bin/datatobeplaybacked.bin

#Enabling the Record/playback Test core data validator
${APP_ROOT}/RecPlayTestUtil $CARRIERIPADDRESS $RECPLAYTEST enable

#Playbacking data
${APP_ROOT}/PlaybackData $CARRIERIPADDRESS 1 0 $PLAYBACKSIZE 0

#Stopping data validation and retrieving test results
${APP_ROOT}/RecPlayTestUtil $CARRIERIPADDRESS $RECPLAYTEST stop
