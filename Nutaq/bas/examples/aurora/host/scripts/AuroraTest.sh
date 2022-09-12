APP_ROOT=/opt/Nutaq/bas/tools/bin

#Application parameters
CARRIER1IPADDRESS="192.168.0.101"
CARRIER2IPADDRESS="192.168.0.102"
RTDEXTESTRX="RtdexTestRx.ini"
RTDEXTESTTX="RtdexTestTx.ini"

#For this Aurora transmission and reception test, the RTDExTest core is used as 
#a ramp generator and validator inside the FPGA and serves the same purpose 
#as in the RTDEx test, even though no RTDEx channel is instantiated in this test
#and no data is transmitted from or to the host. Data is transmitted between 
#carrier 1 and carrier 2 instead. 

#RTDEx core reset, on carrier 1. The RTDExTestRx.ini file is used arbitrarily. 
#The Tx version could have been used as well.
${APP_ROOT}/RtdexTestUtil $CARRIER1IPADDRESS $RTDEXTESTRX init

#RTDEx core reset, on carrier 2. The RTDExTestRx.ini file is used arbitrarily. 
#The Tx version could have been used as well.
${APP_ROOT}/RtdexTestUtil $CARRIER2IPADDRESS $RTDEXTESTRX init

echo "--- RTDExTest cores on both carriers initialized."

#Initialize all 3 instantiated Aurora channels on carrier 1, in both directions
${APP_ROOT}/AuroraInitChannel $CARRIER1IPADDRESS 1 RXTX

${APP_ROOT}/AuroraInitChannel $CARRIER1IPADDRESS 2 RXTX

${APP_ROOT}/AuroraInitChannel $CARRIER1IPADDRESS 3 RXTX

echo "--- Aurora cores on carrier 1 initialized."

#Initialize all 3 instantiated Aurora channels on carrier 2, in both directions
${APP_ROOT}/AuroraInitChannel $CARRIER2IPADDRESS 1 RXTX
                                                 
${APP_ROOT}/AuroraInitChannel $CARRIER2IPADDRESS 2 RXTX
                                                 
${APP_ROOT}/AuroraInitChannel $CARRIER2IPADDRESS 3 RXTX

echo "--- Aurora cores on carrier 2 initialized."

#RTDExTest core enable, Rx direction, on carrier 1
${APP_ROOT}/RtdexTestUtil $CARRIER1IPADDRESS $RTDEXTESTRX enable

#RTDExTest core enable, Rx direction, on carrier 2
${APP_ROOT}/RtdexTestUtil $CARRIER2IPADDRESS $RTDEXTESTRX enable

#RTDExTest core enable, Tx direction, on carrier 1
${APP_ROOT}/RtdexTestUtil $CARRIER1IPADDRESS $RTDEXTESTTX enable

#RTDExTest core enable, Tx direction, on carrier 2
${APP_ROOT}/RtdexTestUtil $CARRIER2IPADDRESS $RTDEXTESTTX enable

echo "--- RTDExTest cores on both carriers transmitting data."

#Monitoring Aurora channels on carrier 1
gnome-terminal -e "./new_console_with_pause.sh ${APP_ROOT}/AuroraMonitor $CARRIER1IPADDRESS" &

#Monitoring Aurora channels on carrier 2
gnome-terminal -e "./new_console_with_pause.sh ${APP_ROOT}/AuroraMonitor $CARRIER2IPADDRESS" &

read -p "Press enter to continue" nothing

echo "--- Stopping test, in uplink direction for carriers 1 and 2"
#Stop/get RTDExTest core status, carrier 1, uplink
${APP_ROOT}/RtdexTestUtil $CARRIER1IPADDRESS $RTDEXTESTTX stop

#Stop/get RTDExTest core status, carrier 2, uplink
${APP_ROOT}/RtdexTestUtil $CARRIER2IPADDRESS $RTDEXTESTTX stop

echo "--- Stopping test, in downlink direction for carriers 1 and 2"
#Stop/get RTDExTest core status, carrier 1, downlink
${APP_ROOT}/RtdexTestUtil $CARRIER1IPADDRESS $RTDEXTESTRX stop

#Stop/get RTDExTest core status, carrier 2, downlink
${APP_ROOT}/RtdexTestUtil $CARRIER2IPADDRESS $RTDEXTESTRX stop

echo "--- Test stopped."

read -p "Press enter to continue" nothing







