
if [ "${TOOLCHAIN_PREFIX}" = "powerpc-linux-gnu-" ]
then
	CROSS_COMPILE=powerpc-linux-gnu- ARCH=powerpc make -C /opt/ltib-p2020ds-20100331/rpm/BUILD/linux M=$(pwd) modules
elif [ -f /etc/debian_version ] #Debian
then
	make -C /usr/src/linux-headers-$(uname -r) M=$(pwd) modules

elif [ -f /etc/lsb-release ] #Ubuntu
then
	make -C /usr/src/linux-headers-$(uname -r) M=$(pwd) modules
else #fedora
	make -C /usr/src/kernels/$(uname -r) M=$(pwd) modules

fi
