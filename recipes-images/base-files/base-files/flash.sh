#!/bin/sh

machine="hd51"
url="https://tuxbox-images.de/images/$machine"
imagename="neutrino-image-$machine.ext4.bz2"
image="neutrino-image_flash.zip"
imagesource="$url/$image"
hdd_mount="/media/USB"
mountpoint -q /media/HDD && hdd_mount="/media/HDD"
imagebase="$hdd_mount/service/image"
imageversion_online="$url/imageversion"
kernelname="kernel.bin"
devbase="/dev/mmcblk0p"
dev_display="/dev/dbox/oled0"
rootdevice="$(sed -e 's/^.*root=//' -e 's/ .*$//' < /proc/cmdline)"

function stop_services () {
	pidof smbd >> /dev/null && systemctl stop smb
	pidof nmbd >> /dev/null && systemctl stop nmb
	pidof xupnpd >> /dev/null && systemctl stop xupnpd
	pidof udpxy >> /dev/null && systemctl stop udpxy
	netstat -tl | grep nfs >> /dev/null && systemctl stop nfs-server && systemctl stop nfs-mountd && systemctl stop nfs-statd
	pidof oscam >> /dev/null && systemctl stop oscam
	pidof cccam >> /dev/null && systemctl stop cccam
	pidof gbox >> /dev/null && systemctl stop gbox
	netstat -tl | grep webmin >> /dev/null && systemctl stop webmin
	netstat -tl | grep sunrpc >> /dev/null && systemctl -q stop rpcbind
	netstat -tl | grep ftp >> /dev/null && systemctl stop proftpd
	pidof minidlnad >> /dev/null && systemctl stop minidlna
	pidof automount >> /dev/null && systemctl stop autofs
	pidof ntpdate >> /dev/null && systemctl stop ntpdate
	cd /etc && git status  >> /dev/null && systemctl -q stop etckeeper
	pidof neutrino >> /dev/null && systemctl stop neutrino
	pidof sshd >> /dev/null && systemctl stop sshd.socket
}

[[ $* = "-h" ]] || [[ $* = "--help" ]] && printf '\n\033[31m%s\n' "Give the destination partition number [1 - 4] as first argument.

As second argument you can specify the path where the image is stored.
If no second argument is given the image will be downloaded from

$imagesource
" && exit 0;

if [[ $1 =~ ^[0-9]+$ ]]; then
	if  [ $1 -lt 1 ] || [ $1 -gt 4 ]; then
		printf '\n\033[31m%s\n' "Choose a valid partition [1-4]" ; exit 1;
	elif [ $1 = 1 ]; then
		part=1; kernel=2; root=3
	elif [ $1 = 2 ]; then
		part=2; kernel=4; root=5
	elif [ $1 = 3 ]; then
		part=3; kernel=6; root=7
	elif [ $1 = 4 ]; then
		part=4; kernel=8; root=9
	fi
else
	if [[ -z $1 ]] || [[ -z $2 ]] || [[ $* = "restore" ]] || [[ $* = "force" ]]; then
		if echo $rootdevice | grep "3"; then
	                part=1; kernel=2; root=3
                elif echo $rootdevice | grep "5"; then
                        part=2; kernel=4; root=5
                elif echo $rootdevice | grep "7"; then
                        part=3; kernel=6; root=7
                elif echo $rootdevice | grep "9"; then
                        part=4; kernel=8; root=9
		fi
	else
		printf '\n\033[31m%s\n' "Please give the partition number [1-4] as first argument" ; exit 1;
	fi
fi

imageversion_local="$imagebase/imageversion_partition_$part"

partition_sectorsize="$(gdisk -l $devbase$root | grep 'Total free space' | awk '{print $5}')"
[ $partition_sectorsize -ne 1638333 ] && printf '\n\033[31m%s\n' "The partition scheme is invalid ... use ofgwrite for flashing" && exit 2;
[ -z $2 ] ||  echo $2 | grep "/" >> /dev/null || [ $2 = "force" ] || [ $2 = "restore" ] || { printf '\n\033[31m%s\n' "Please choose a valid path" && exit 9; } 

clear

function write_image () {
[[ $rootdevice = $devbase$root ]] && stop_services
printf '\n\033[1m%s\n\033[0m' "Writing image into partition $part"
echo "Writing image into partition $part" > $dev_display
printf '\033[33m%s\033[37m%s\n' "Writing kernel into $devbase$kernel"
pv < $imagedir/$kernelname > $devbase$kernel || { printf '\033[31m%s\n' "Writing the kernel failed" ; sleep 3; clear; exit 7; }
printf '\033[33m%s\033[37m%s\n' "Writing rootfs into $devbase$root"
bzip2 -cd $imagedir/$imagename | pv > $devbase$root || { printf '\033[31m%s\n' "Writing rootfs into $devbase$root failed" ; sleep 3; clear; exit 8; }
printf '\n\033[1m\033[32m%s\033[0m\n' "Flash successful"
printf '\033[1m\033[0m'
[ $rootdevice = $devbase$root ] && echo "...Reboot" > $dev_display && reboot="$(echo 1 > /proc/sys/kernel/sysrq && echo b > /proc/sysrq-trigger)"
echo "Flash succeeded" > $dev_display
sleep 3
clear
exit 0;
}

if [[ ! $1 = "force" ]] && [[ ! $2 = "force" ]]; then
	if [[ $* = "restore" ]] || [ ! -z $2 ]; then
		for i in $1 $2; do
  	      		case $i in
        	        	restore)
                	        	imagedir="$imagebase/backup/partition$part/$machine"
                		;;
                		1|2|3|4)
                        		imagedir="$2"
                		;;
                		/*)
                   			imagedir="$2"
                		;;
                		"")
                   			imagedir="$1"
                		;;
        		esac
		done
	imageversion="$imagedir/imageversion"
	[[ -f $imageversion ]] && cp -rf $imageversion $imageversion_local
        [ ! -f $imagedir/$imagename ] && { printf '\n%s\n' "$imagedir/$imagename not found" && sleep 3 && clear && exit 1; }
        [ ! -f $imagedir/$kernelname ] && { printf '\n%s\n' "$imagedir/$kernelname not found" && sleep 3 && clear && exit 1; }
        space_available="$(df -Pk $imagedir | awk 'NR==2 {print $4}')"
        [ $space_available -lt 1048576 ] && { printf '\n\033[31m%s\n' "You need at least 1G of free space on your device" && sleep3 && clear && exit 1; }
        write_image
	fi
fi

if [[ -z $1 ]] || [[ $* = "force" ]] || [[ ! $* = "restore" ]] || [[ $1 =~ ^[0-9]+$ ]]; then
	case $1 in
		1|2|3|4|""|force)
			imagedir="$imagebase/$machine"
			imagefile="$imagebase/$image"
			imageversion="$imagedir/imageversion"
			[ ! -d $imagebase ] && mkdir -p $imagebase
			space_available="$(df -Pk $imagebase | awk 'NR==2 {print $4}')"
			[ $space_available -lt 1048576 ] && printf '\n\033[31m%s\n' "You need at least 1G of free space on $hdd_mount" && sleep 3 && clear && exit 3;
			if [[ -z $2 ]] && [[ ! $1 = "force" ]]; then
        			[ ! -f $imageversion_local ] && touch $imageversion_local
				md5_imageversion_online=$(curl -s $imageversion_online | md5sum | awk '{print $1}')
				md5_imageversion_local=$(md5sum "$imageversion_local" | awk '{print $1}')
				[ $md5_imageversion_online = $md5_imageversion_local ] && { printf '\n\033[32m%s\n\033[0m' "No update available" && \
				echo "No update available" > $dev_display; sleep 3; clear; exit 4; }
			elif [[ $1 = "force" ]] || [[ $2 = "force" ]]; then
                		printf '\033[31m'
			fi
			echo "Downloading $image" > $dev_display
			printf '\n\033[1m%s\n\033[0m' "Downloading $imagesource"
			unpack="unzip -x $imagefile -d $imagebase"
			curl $imagesource -o $imagefile || { printf '\n\033[31m%s\n' "Downloading the image failed"; sleep 3; clear; exit 5; }
			[ -d $imagedir ] && rm -r $imagedir
			echo "Decompressing $image" > $dev_display
        		printf '\n\033[1m%s\n\033[0m' "Decompressing  $image"
			$unpack || { printf '\n\033[31m%s\n' "Unpacking the image failed" ; sleep 3; clear; exit 6; }
			[[ -f $imageversion ]] && cp -rf $imageversion $imageversion_local
			[ -f $imagefile ] && rm $imagefile
			write_image
		;;
	esac
fi