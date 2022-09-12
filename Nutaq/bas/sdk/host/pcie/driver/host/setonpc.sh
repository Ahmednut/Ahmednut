#!/bin/bash

lsmod | grep host_drv >/dev/null
if [ $? -eq 0 ]
then
  rmmod host_drv
fi

# module parameters:
#   cdmasizek : sizeof of CDMA bufer (host to fpga), in K bytes. Default is 1024
#   dmsizek : sizeof of DM bufer (fpga to host), in K bytes. Default is 256
insmod ./host_drv.ko 
#insmod ./host_drv.ko cdmasizek=1024 dmsizek=256

# wait some time to let linux install driver & create device names:
sleep 1
chmod 777 -R /dev/perseus*


ip link set mailbox0 up
ip  addr add 192.168.101.99/24 dev mailbox0
