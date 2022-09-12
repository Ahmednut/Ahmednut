#!/bin/bash

script_name='logout.sh'
#
# Check that we are being run using '. ./file' rather
# than './file'.
#
if [ `basename ./$0` == $script_name ]; then

  echo
  echo Please use \'. ./$script_name\'.
  echo

else
echo logout [logout last user name use when login]
CVS_COMMAND="cvs -d :pserver:${CVS_USER}@repository:/lyrtech logout"
${CVS_COMMAND}
CVS_USER=
export CVS_USER
fi

