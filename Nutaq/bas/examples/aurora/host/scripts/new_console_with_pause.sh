#!/bin/sh

# Call the application from the argument
$*

# Verify return value
OUT=$?
if [ $OUT -lt 0 ]; then
   read -p "Press enter to continue" nothing
   exit
fi

read -p "Press enter to continue" nothing
exit





