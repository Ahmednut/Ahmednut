#! /bin/sh
#set -x 
#touch README NEWS ChangeLog AUTHORS INSTALL
autoscan
libtoolize --force
aclocal --force 
#rh9 
#aclocal
#libtoolize --copy --force
autoconf --force
autoheader --force
automake --foreign --add-missing --copy --force-missing
echo    -----------------------------------------------------------------------
echo    -----------------------------------------------------------------------
echo 			bootstrap ./configure
pwd
echo    -----------------------------------------------------------------------
echo    -----------------------------------------------------------------------
sudo ./configure
echo    -----------------------------------------------------------------------
echo    -----------------------------------------------------------------------
echo 			bootstrap ./build
pwd
echo    -----------------------------------------------------------------------
echo    -----------------------------------------------------------------------
./build.sh

