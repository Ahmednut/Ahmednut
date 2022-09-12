#!/bin/sh

PATH=/bin:/sbin:/usr/bin:/usr/sbin
ifconfig eth0 down
ethtool -A eth0 autoneg on rx on tx on
ifconfig eth0 mtu 9000 up
ethtool -r eth0

echo "Starting cce: "
cce &

