#!/bin/sh
myNewPath="/usr/bst/usrfs_new"
myThemePath="/usr/bst/usrfs_new/Theme"
myExePath="/usr/bst/usrfs_new/TFT"
changeDirName()
{
    mv /usr/bst/usrfs_new  /usr/bst/usrfs 
}

copyFileOldToNew()
{
    cp -rfv /usr/bst/usrfs/TFT  /usr/bst/usrfs_new
    cp -rfv /usr/bst/usrfs/*.ko  /usr/bst/usrfs_new
}

copyFileNewToOld()
{
    cp -rfv /usr/bst/usrfs_new/TFT  /usr/bst/usrfs
}

deleteDir()
{
    rm -rf /usr/bst/usrfs
}

deleteNewDir()
{
    rm -rf /usr/bst/usrfs_new
}

main()
{
    if [ ! -d "$myNewPath" ];then
        echo "/usr/bst/usrfs_new is not exists"    
    else
        if [ ! -d "$myThemePath" ];then
            echo "/usr/bst/usrfs_new/Theme is not exists" 
            copyFileNewToOld
            deleteNewDir
        else
            if [ ! -f "$myExePath" ];then
                copyFileOldToNew
                deleteDir
                changeDirName
                echo "/usr/bst/usrfs_new/Theme is exists"
            else
                deleteDir
                changeDirName
                echo "/usr/bst/usrfs_new/Theme is exists" 
            fi
        fi               
    fi
}

mkdir_pub()
{
    if [ ! -d "/home/ftp/pub" ];then
        mkdir -p /home/ftp/pub
        chmod -R 777 /home/ftp/pub
        echo "/home/ftp/pub is not exist will create"
    else
        echo "/home/ftp/pub is exist donothing"
    fi
}

delayTime()
{
    sleep 1
}

if [ -f "/media/mmcblk0p1/function/functiontest.sh" ]; then
	echo "Start functiontest.sh instead of Qt Demo"
else 
	# Start qt
	main	
	delayTime
	source /etc/profile
	/usr/bst/usrfs/TFT &
	mkdir_pub
	/usr/bin/boa &
	#cd /usr/bst/usrfs
	#./TFT &
fi
