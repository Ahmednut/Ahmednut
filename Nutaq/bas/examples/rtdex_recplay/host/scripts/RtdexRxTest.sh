APP_ROOT=/opt/Nutaq/bas/tools/bin

CARRIERIPADDRESS="192.168.0.101"
RTDEXTEST="RtdexTestUplink.ini"
STREAMINGINIFILENAME="RxStreaming.ini"

#Configuring the RTDEx Test core
${APP_ROOT}/RtdexTestUtil $CARRIERIPADDRESS $RTDEXTEST init

#Enabling the RTDEx Test core to generate a test ramp and send it to RTDEx
#core for transmission to host
${APP_ROOT}/RtdexTestUtil $CARRIERIPADDRESS $RTDEXTEST enable

#Enabling RTDEx core to transmit data to host
${APP_ROOT}/RxStreaming $STREAMINGINIFILENAME $CARRIERIPADDRESS

#Stopping test and displaying stats
${APP_ROOT}/RtdexTestUtil $CARRIERIPADDRESS $RTDEXTEST stop

