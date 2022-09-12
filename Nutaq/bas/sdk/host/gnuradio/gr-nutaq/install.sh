#!/bin/sh

./compile.sh
cd build
echo "Administrator priviledge are required to install gr-nutaq."
sudo make install

cd ..
sudo cp LaunchGNURadioCompanion.sh ../../../../