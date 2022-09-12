APP_ROOT=/opt/Nutaq/bas/tools/bin

#Application parameters
CARRIERIPADDRESS="192.168.0.101"
RECPLAYTEST="RecplayTestRecord.ini"
#Record size, in bytes. Must be the same as the value specified in RecplayTestRecord.ini
RECORDSIZE="1048576"

#Configuring the Record/playback Test core 
${APP_ROOT}/RecPlayTestUtil $CARRIERIPADDRESS $RECPLAYTEST init

#Record start
${APP_ROOT}/RecordData $CARRIERIPADDRESS 2 $RECORDSIZE 0 0

#Enabling the Record/playback Test core data generator
${APP_ROOT}/RecPlayTestUtil $CARRIERIPADDRESS $RECPLAYTEST enable

#Retrieving data
${APP_ROOT}/RetrieveRecordedData $CARRIERIPADDRESS 0 8192 $RECORDSIZE 0 1000 ../bin/recordeddata.bin

#Stopping data generation and analyzing record file
${APP_ROOT}/RecPlayTestUtil $CARRIERIPADDRESS $RECPLAYTEST stop
