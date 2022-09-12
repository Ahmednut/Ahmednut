#!/bin/sh

cp _eapi.so ../../../cli/src/
cp ../../../cli/swig/eapi.py ../../../cli/src/

#create cli command for terminal
echo "-----------------------------------------------"
echo "Creating cli command for terminal..." 
touch cli
echo "#! /bin/sh" > cli
echo "cd /opt/Nutaq/bas/sdk/host/cli/src" >> cli
echo "python bas_cli.py" >> cli
chmod +x cli
sudo cp cli /usr/bin/
rm cli
echo -e "Done\n"


