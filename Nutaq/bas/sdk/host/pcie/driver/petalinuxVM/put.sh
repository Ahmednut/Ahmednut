# put.sh
# copy driver binary to s100

ftp -n s100 <<-EOF
user adp_pcie nutaq
prompt off
binary

cd driver/perseus
lcd ../user-modules/perseus_mailbox
put perseus_mailbox.ko
bye

EOF

