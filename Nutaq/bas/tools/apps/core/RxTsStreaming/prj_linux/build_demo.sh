#!/bin/bash

DIR="../../../../bin"

if [ ! -d $DIR ];then
mkdir $DIR
fi

make -f Makefile
