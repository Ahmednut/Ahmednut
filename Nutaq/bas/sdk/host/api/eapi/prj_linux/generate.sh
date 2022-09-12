#!/bin/sh

sudo env "PATH=$PATH" ./bootstrap.sh
sudo make -j4
./build_eapi.sh
sudo ./build_cli.sh

cp *.o *.la *.a ../../../lib64
