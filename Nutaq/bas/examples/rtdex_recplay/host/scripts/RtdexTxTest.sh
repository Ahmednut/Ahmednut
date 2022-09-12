APP_ROOT=/opt/Nutaq/bas/tools/bin

CARRIERIPADDRESS="192.168.0.101"
RTDEXTEST="RtdexTestDownlink.ini"
STREAMINGINIFILENAME="TxStreaming.ini"

#Configuring the RTDEx Test core
${APP_ROOT}/RtdexTestUtil $CARRIERIPADDRESS $RTDEXTEST init

#Enabling the RTDEx Test core to receive a test ramp from the RTDEx 
#core and validate it 
${APP_ROOT}/RtdexTestUtil $CARRIERIPADDRESS $RTDEXTEST enable

#Enabling RTDEx core to receive data from host
${APP_ROOT}/TxStreaming $STREAMINGINIFILENAME $CARRIERIPADDRESS

#Stopping test and displaying stats
${APP_ROOT}/RtdexTestUtil $CARRIERIPADDRESS $RTDEXTEST stop

