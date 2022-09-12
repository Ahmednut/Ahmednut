# get.sh
# copy driver source from s100

ftp -n s100 <<-EOF
user adp_pcie nutaq
prompt off

cd driver/perseus
lcd ../user-modules/perseus-pci
get perseus_drv.c
cd ../../inc
get perseus2hostregs.h
bye

EOF

