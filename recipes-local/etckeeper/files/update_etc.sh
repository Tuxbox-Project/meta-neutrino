#!/bin/sh

rootdevice=$(sed -e 's/^.*root=//' -e 's/ .*$//' < /proc/cmdline)
dev="HDD"
mountpoint -q /media/USB && dev="USB"

if [ $rootdevice = /dev/mmcblk0p3 ]; then
        GIT__URL="/media/$dev/service/partition1/git/etc.git"
elif [ $rootdevice = /dev/mmcblk0p5 ]; then
        GIT__URL="/media/$dev/service/partition2/git/etc.git"
elif [ $rootdevice = /dev/mmcblk0p7 ]; then
        GIT__URL="/media/$dev/service/partition3/git/etc.git"
elif [ $rootdevice = /dev/mmcblk0p9 ]; then
        GIT__URL="/media/$dev/service/partition4/git/etc.git"
fi

GIT_EXIST=$(echo $GIT__URL"/HEAD")

if [ -e $GIT_EXIST ];then
        if [ ! -e /etc/gitconfig ];then
                git config --system user.name "GIT_USER"
                git config --system user.email "MAIL"
                git config --system core.editor "nano"
                git config --system http.sslverify false
        fi
        if [ "$(cd $GIT__URL && git log -1 --pretty=format:"%cd")" == "$(cd /etc && git log -1 --pretty=format:"%cd")" ];then
                exit
        else
                systemctl stop neutrino.service
                echo "writing back /etc git remote from /dev/$dev"  > /dev/dbox/oled0
                cd /etc && etckeeper init
                git fetch -a
                git reset --hard origin/master
                echo "...done"
                sync
                etckeeper init
                echo "rebooting"
                systemctl reboot
        fi
else
        echo "no remote found"
fi