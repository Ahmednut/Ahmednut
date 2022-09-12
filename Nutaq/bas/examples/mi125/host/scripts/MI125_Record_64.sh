APP_ROOT=/opt/Nutaq/bas/tools/bin

#Application parameters
CARRIERIPADDRESS="192.168.0.101"
INIFILENAME="MI125_Init_64.ini"
DESIREDRECORDSIZE=65536
#65536 + 64
RECORDSIZE=65600

${APP_ROOT}/MI125_Init $CARRIERIPADDRESS $INIFILENAME

#Select recording mode
#Value = 0 : 16 channels board #1
#Value = 1 : 16 channels board #2
#Value = 2 : 16 channels board #3
#Value = 3 : 16 channels board #4
#Value = 4 : 32 channels board #1 and #2 downsampled by 2
#Value = 5 : 32 channels board #3 and #4 downsampled by 2
#Value = 6 : 64 channels board #1, #2, #3 and #4 downsampled by 4
${APP_ROOT}/CustomRegister_Write $CARRIERIPADDRESS 13 6

#Trigger source selector
#Value = 0 : Software trigger with custom register #15
#Value = 1 : From MI125 front-panel trigger connector
${APP_ROOT}/CustomRegister_Write $CARRIERIPADDRESS 14 0

#Software trigger reset
${APP_ROOT}/CustomRegister_Write $CARRIERIPADDRESS 15 0

#Start data recording process
#Parameters 1 must be the Perseus IP address, ex 192.168.0.101
#Parameters 2 must be the Trigger source; External = 0, Software = 1, No Trigger = 2
#Parameters 3 must be the Record size in bytes
#Parameters 4 must be the Start address in bytes
#Parameters 5 must be the Trigger delay in bytes
${APP_ROOT}/RecordData $CARRIERIPADDRESS 0 $RECORDSIZE 0 0

#Software trigger set
${APP_ROOT}/CustomRegister_Write $CARRIERIPADDRESS 15 1

#Retrieve recorded data
#Parameters 1 must be the Perseus IP address, ex 192.168.0.101
#Parameters 2 must be the RTDEx channel
#Parameters 3 must be the Frame size in bytes
#Parameters 4 must be the Record size in bytes
#Parameters 5 must be the Start Address in bytes or "trigger"
#Parameters 6 must be the Record timeout in milliseconds
#Parameters 7 must be the File name, ex sinewave.bin
${APP_ROOT}/RetrieveRecordedData $CARRIERIPADDRESS 0 8192 $DESIREDRECORDSIZE trigger 1000 ../bin/record.bin