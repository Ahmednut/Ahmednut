host=192.168.0.100

tftp -g -r driver/perseus/getonperseus.sh $host
tftp -g -r driver/perseus/irq.sh $host
tftp -g -r driver/perseus/setonperseus.sh $host
tftp -g -r driver/perseus/perseus_mailbox.ko $host
chmod 777 *.sh
