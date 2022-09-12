# get all *.c *.h
# run this script on "adp/sdk/PCIe/host" dir

HOST=192.168.0.100
USER=adp_pcie
PASS=nutaq
ftp -n $HOST <<EOF
user $USER $PASS
binary
prompt off

cd LYR_inc
mget *.h
cd ..

cd driver/host
get host_drv.c
mget *.sh
get Makefile
bye
EOF

dos2unix -k -q *.sh
