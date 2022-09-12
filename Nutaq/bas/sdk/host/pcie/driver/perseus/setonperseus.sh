rmmod perseus_mailbox
insmod perseus_mailbox.ko 
ip link set mailbox0 up
ip  addr add 192.168.101.2/24 dev mailbox0
mkdir /dev/perseus &> /dev/null
mkdir /dev/perseus/0 &> /dev/null
grep perseus-mailbox /proc/devices | while read MAILBOX_MAJ JUNK
do
	rm    /dev/perseus/0/mailbox
	mknod /dev/perseus/0/mailbox c $MAILBOX_MAJ 0
done
mkdir /dev/perseus/rtdex &> /dev/null
grep rtdex /proc/devices | while read RTDEX_MAJ JUNK
do
	rm    /dev/perseus/0/rtdex/0
	mknod /dev/perseus/0/rtdex/0 c $RTDEX_MAJ 0
done
