PATH=/usr/lib/qt-3.3/bin:/usr/kerberos/bin:/usr/lib/ccache:/usr/local/bin:/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/sbin:${HOME}/bin
#export PETALINUX=${HOME}/petalinux-v0.40-final/
#export PATH=${PATH}:${PETALINUX}/tools/linux-i386/microblaze-uclinux-tools/bin/
#BIN=${PETALINUX}/tools/linux-i386/microblaze-glibc-root/bin/
#BIN=${PETALINUX}/tools/linux-i386/microblaze-glibc-root/bin/
#BIN=${PETALINUX}/tools/linux-i386/microblaze-glibc-root/microblaze-linux-elf/bin/
#BIN=${PETALINUX}/tools/linux-i386/microblaze-uclinux-tools/bin/
BIN=${HOME}/microblaze_v1.0/microblaze-unknown-linux-gnu/bin/
export PATH=${BIN}:${PATH}

export CROSS_COMPILE=mb-linux-
BINPREFIX=${CROSS_COMPILE}
#BINPREFIX=
#BINPREFIX=microblaze-uclinux-
#BINPREFIX=microblaze-unknown-linux-gnu-

#=${BIN}/${BINPREFIX}-addr2line
export AR=${BIN}/${BINPREFIX}ar
export AS=${BIN}/${BINPREFIX}as
#=${BIN}/${BINPREFIX}-c++
#=${BIN}/${BINPREFIX}-c++filt
#=${BIN}/${BINPREFIX}-cpp
export CXX=${BIN}/${BINPREFIX}g++a
export CC=${BIN}/${BINPREFIX}gcc
#=${BIN}/${BINPREFIX}-gcc-4.1.1
#=${BIN}/${BINPREFIX}-gccbug
#=${BIN}/${BINPREFIX}-gcov
#=${BIN}/${BINPREFIX}-gdb
#=${BIN}/${BINPREFIX}-gdbtui
#=${BIN}/${BINPREFIX}-insight
export LD=${BIN}/${BINPREFIX}ld
export NM=${BIN}/${BINPREFIX}nm
#=${BIN}/${BINPREFIX}-objcopy
#=${BIN}/${BINPREFIX}-objdump
#=${BIN}/${BINPREFIX}-ranlib
#=${BIN}/${BINPREFIX}-readelf
#=${BIN}/${BINPREFIX}-size
#=${BIN}/${BINPREFIX}-strings
#=${BIN}/${BINPREFIX}-strip
#export LIBS=.

export host=i686-redhat-linux
#export target=microblaze-uclinux
export target=mb-linux
#export target=microblaze-linux-elf

./configure  --host ${host} --target ${target} --enable-static
make sperseusd cce


