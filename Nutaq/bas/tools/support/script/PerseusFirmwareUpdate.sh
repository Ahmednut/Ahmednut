#!/bin/bash

echo 	
echo  Perseus Firware update	 
echo 	
echo 

BASROOT=/opt/Nutaq/bas

#Get IP address from user
FICHIER=PerseusFirmwareUpdate.txt
echo Please enter your Perseus IP address: 
read IP

#Create PerseusFirmwareUpdate script CLI 
echo connect $IP>$FICHIER
echo version>>$FICHIER
echo update_uboot $BASROOT/sdk/embedded/bin/u-boot-s.bin>>$FICHIER
echo update_kernel $BASROOT/sdk/embedded/bin/image.ub>>$FICHIER
echo update_dtb $BASROOT/sdk/embedded/bin/system.dtb>>$FICHIER
echo update_jffs2 $BASROOT/sdk/embedded/bin/rootfs.jffs2>>$FICHIER
echo reboot>>$FICHIER

#Execute PerseusFirmwareUpdate script CLI 
sudo bash $BASROOT/LaunchCLI.sh $FICHIER
