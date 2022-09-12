APP_ROOT=/opt/Nutaq/bas/tools/bin

#Application parameters
CARRIER1IPADDRESS="192.168.0.101"
RTDEXTESTRX="RtdexTestRx.ini"
RTDEXTESTTX="RtdexTestTx.ini"

#For this Aurora transmission and reception test, the RTDExTest core is used as 
#a ramp generator and validator inside the FPGA and serves the same purpose 
#as in the RTDEx test, even though no RTDEx channel is instantiated in this test
#and no data is transmitted from or to the host. Data is transmitted between 
#the FPGA carrier and itself instead.

#RTDEx core reset on FPGA carrier. The RTDExTestRx.ini file is used arbitrarily. 
#The Tx version could have been used as well.
${APP_ROOT}/RtdexTestUtil $CARRIER1IPADDRESS $RTDEXTESTRX init



echo "--- RTDExTest core on FPGA carrier initialized."

#Initialize all 7 instantiated Aurora channels on FPGA carrier, in both directions
${APP_ROOT}/AuroraInitChannel $CARRIER1IPADDRESS 1 RXTX

${APP_ROOT}/AuroraInitChannel $CARRIER1IPADDRESS 2 RXTX

${APP_ROOT}/AuroraInitChannel $CARRIER1IPADDRESS 3 RXTX

${APP_ROOT}/AuroraInitChannel $CARRIER1IPADDRESS 4 RXTX

${APP_ROOT}/AuroraInitChannel $CARRIER1IPADDRESS 5 RXTX

${APP_ROOT}/AuroraInitChannel $CARRIER1IPADDRESS 6 RXTX

${APP_ROOT}/AuroraInitChannel $CARRIER1IPADDRESS 7 RXTX

echo "--- Aurora cores on FPGA carrier initialized."



#RTDExTest core enable, Rx direction on FPGA carrier
${APP_ROOT}/RtdexTestUtil $CARRIER1IPADDRESS $RTDEXTESTRX enable

#RTDExTest core enable, Tx direction on FPGA carrier
${APP_ROOT}/RtdexTestUtil $CARRIER1IPADDRESS $RTDEXTESTTX enable


echo "--- RTDExTest core on FPGA carrier transmitting data."

#Monitoring Aurora channels on FPGA carrier
gnome-terminal -e "./new_console_with_pause.sh ${APP_ROOT}/AuroraMonitor $CARRIER1IPADDRESS" &


read -p "Press enter to continue" nothing

echo "--- Stopping test, in uplink direction"
#Stop/get RTDExTest core status, uplink
${APP_ROOT}/RtdexTestUtil $CARRIER1IPADDRESS $RTDEXTESTTX stop

echo "--- Stopping test, in downlink direction"
#Stop/get RTDExTest core status, downlink
${APP_ROOT}/RtdexTestUtil $CARRIER1IPADDRESS $RTDEXTESTRX stop

echo "--- Test stopped."

read -p "Press enter to continue" nothing







