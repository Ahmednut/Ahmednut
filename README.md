# BAS
Nutaq sodtware suite
Preparing the system
1. Make sure the user account used has administrator rights and can use the command sudo.
2. Update the system prior to the installation using the user interface tools or through a terminal with the
command:
    sudo apt-get upgrade
3. Install the standard development packages on the system using the user interface tools or through terminal
with the commands:
    sudo apt-get install build-essential
    sudo apt-get install libpcap-dev
    sudo apt-get install libtool

Configuring the Environment
1. Modify the installation folder owner with:
    sudo chown -R {username} //opt/Nutaq
2. Modify the installation folder group with:
    sudo chgrp -R {username} //opt/Nutaq
3. Modify the installation folder permissions with:
    sudo chmod -R +x //opt/Nutaq
4. Modify the cache folder permissions with:
    sudo chown -R {username} //var/cache
