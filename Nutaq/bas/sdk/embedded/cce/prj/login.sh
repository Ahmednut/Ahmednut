#!/bin/bash

script_name='login.sh'
#
# Check that we are being run using '. ./file' rather
# than './file'.
#
if [ `basename ./$0` == $script_name ]; then

  echo
  echo Please use \'. ./$script_name\'.
  echo

else
echo login user [user is cvs user name that will be used later]
export CVS_USER=$1
CVS_COMMAND="cvs -d :pserver:${CVS_USER}@repository:/lyrtech login"
${CVS_COMMAND}
fi

