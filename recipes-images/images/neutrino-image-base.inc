# Base this image on core-image-minimal
include recipes-core/images/core-image-minimal.bb

# add default password for root

ROOTFS_POSTPROCESS_COMMAND += "set_root_passwd;"
set_root_passwd() {
   ROOTPW_ENCRYPTED=""
   if [ -n "${ROOTPW}" ]; then
   ROOTPW_ENCRYPTED=`openssl passwd -crypt -quiet ${ROOTPW}`
   else
   ROOTPW_ENCRYPTED="wYNffsf6sozwE"
   fi
   sed "s%^root:[^:]*:%root:${ROOTPW_ENCRYPTED}:%" \
       < ${IMAGE_ROOTFS}/etc/shadow \
       > ${IMAGE_ROOTFS}/etc/shadow.new;
   mv ${IMAGE_ROOTFS}/etc/shadow.new ${IMAGE_ROOTFS}/etc/shadow ;
   rm -rf ${IMAGE_ROOTFS}/tmp/*
}

ROOTFS_POSTPROCESS_COMMAND += "set_image_version;"
# We create file /etc/image-version for common use (required by some plugins)
# and for compatibility we make a symlink from /etc/image-version to /.version
# required by neutrino itself.
set_image_version () {
	META_NAME="meta-neutrino"
	# In case of changed repository name this keeps compatibilty.
	# Why: meta-neutrino contains mostly recipes to create a tuxbox image and
	# neutrino is only a part of image like all the other recipes.
	# The flavour settings are only relevant for neutrino and for some
	# neutrino package related environment parts
	# so only neutrino should be marked with current flavour tags.
	if [ -e ${COREBASE}/meta-tuxbox ]; then
		META_NAME="meta-tuxbox"
	fi

	# We extract the image version from git describe content
	# general meta version comes from the yocto poky repository and git tags are synchronized
	# with our meta layers. Only the describe content will be added for our image version.
	META_TAG=`git -C ${COREBASE}/$META_NAME describe --abbrev=0`
	META_VERSION=$META_TAG-`git -C ${COREBASE}/$META_NAME rev-list $META_TAG..HEAD --count`

	if [ -z $META_VERSION ]; then
		META_VERSION=${DISTRO_VERSION_NUMBER}
	fi

	RELEASE_NAME="${IMAGE_BASENAME} ${META_VERSION}"
	RELEASE_NAME_DISCRIBE=`git -C ${COREBASE}/$META_NAME  describe --always --tags --dirty`

	# Set the release type within version files and readable text content for gui
	RELASE_TYPE="Release"
	if [ ${RELEASE_STATE} == 1 ]; then
		RELASE_TYPE="Beta"
	elif [ ${RELEASE_STATE} == 2 ]; then
		RELASE_TYPE="Nightly"
	fi

	IMAGE_DESCRIPTION="${MACHINE_BRAND}-${MACHINE_NAME} [${ARCH}-${MACHINE}] ${RELASE_TYPE}"

	# get content of ../imageversion file
	# required to synchronize version entry with content of ../imageversion file, which is used for image updates
	echo ${IMAGE_NAME} `cat ${DEPLOY_DIR_IMAGE}/version` > ${DEPLOY_DIR_IMAGE}/imageversion
	
	IMAGE_STAMP_STR=`cat ${DEPLOY_DIR_IMAGE}/imageversion`
	IMAGE_STAMP_LEN=`expr length ${IMAGE_STAMP_STR}`
	IMAGE_STAMP_START=`expr ${IMAGE_STAMP_LEN} - 14`
	IMAGE_STAMP=${IMAGE_STAMP_STR:$IMAGE_STAMP_START:$IMAGE_STAMP_LEN}
	IMAGE_BUILD_TIME_STAMP=${IMAGE_STAMP:0:12}

	#IMAGE_ROOTFS_ETC_DIR=${D}${sysconfdir}
	IMAGE_ROOTFS_ETC_DIR=${IMAGE_ROOTFS}/etc

	# create content for /etc/image-version file
	echo "builddate=${IMAGE_STAMP:6:2}.${IMAGE_STAMP:4:2}.${IMAGE_STAMP:0:4} ${IMAGE_STAMP:8:2}:${IMAGE_STAMP:10:2}" > ${IMAGE_ROOTFS_ETC_DIR}/image-version
	echo "box_model=${MACHINE}"                                                     				>> ${IMAGE_ROOTFS_ETC_DIR}/image-version
	echo "creator=${CREATOR}"                                                       				>> ${IMAGE_ROOTFS_ETC_DIR}/image-version
	echo "distro=${DISTRO}"                                                             				>> ${IMAGE_ROOTFS_ETC_DIR}/image-version
	echo "distro_version=${DISTRO_VERSION}"                                                             		>> ${IMAGE_ROOTFS_ETC_DIR}/image-version
	echo "homepage=${HOME_URL}"                                                     				>> ${IMAGE_ROOTFS_ETC_DIR}/image-version
	echo "imagename=${IMAGE_BASENAME}"  	                                         				>> ${IMAGE_ROOTFS_ETC_DIR}/image-version
	echo "machine=${MACHINE}"                                                       				>> ${IMAGE_ROOTFS_ETC_DIR}/image-version
	echo "version=${RELEASE_STATE}${DISTRO_VERSION_NUMBER_CYCLE}${IMAGE_BUILD_TIME_STAMP}"   			>> ${IMAGE_ROOTFS_ETC_DIR}/image-version
	echo "imagedescription=${IMAGE_DESCRIPTION}"                                    				>> ${IMAGE_ROOTFS_ETC_DIR}/image-version
	echo "imageversion=${META_VERSION}"                                             				>> ${IMAGE_ROOTFS_ETC_DIR}/image-version
	echo "describe=${RELEASE_NAME_DISCRIBE}"                                        				>> ${IMAGE_ROOTFS_ETC_DIR}/image-version
	echo "neutrino_src=${FLAVOUR}"                                                  				>> ${IMAGE_ROOTFS_ETC_DIR}/image-version
	echo "imagedir=${MACHINE}"                                                      				>> ${IMAGE_ROOTFS_ETC_DIR}/image-version
	echo "image_update_url=${IMAGE_LOCATION}"                                       				>> ${IMAGE_ROOTFS_ETC_DIR}/image-version

	# We create a symlink from /etc/image-version to /.version to have all required content
	# and we have a compatible data source.
	ln -sf /etc/image-version ${IMAGE_ROOTFS}.version

	# Finally we create content for update.urls file.
	if [ ! -z ${RELEASE_TEXT_LOCATION_HOST} ]; then
		echo "${IMAGE_LOCATION} ${RELEASE_STATE}${DISTRO_VERSION_NUMBER_CYCLE}${IMAGE_BUILD_TIME_STAMP} MD5 ${RELEASE_NAME} ${IMAGE_DESCRIPTION} ${UPDATE_SERVER_URL}" > ${RELEASE_TEXT_LOCATION_HOST}
	fi
}


KERNEL_MODULES_INSTALLED += ""


BIG_IMAGE_FILES = " \
	bash \
	bluez5 \
	bzip2 \
	cifs-utils \
	dvb-femon \
	etckeeper \
	exfat-utils \
	fbset \
	fuse-exfat \
	glibc-binary-localedata-en-us \
	glibc-binary-localedata-cs-cz \
	glibc-binary-localedata-de-de \
	glibc-binary-localedata-es-es \
	glibc-binary-localedata-fr-fr \
	glibc-binary-localedata-pl-pl \
	glibc-binary-localedata-ru-ru \
	glibc-charmap-utf-8 \
	grep \
	gzip \
	htop \
	image-config \
	imagemagick \
	links \
	localedef \
	ntfs-3g \
	ntfsprogs \
	oh-my-zsh \
	parted \
	procps \
	proftpd \
	samba \
	sed \
	usbutils \
	util-linux \
	util-linux-fstrim \
	webmin \
	wpa-supplicant \
	zsh \
"

KERNEL_WIFI_DRIVERS = " \
	firmware-carl9170 \
	firmware-htc7010 \
	firmware-htc9271 \
	firmware-rt2870 \
	firmware-rt73 \
	firmware-rtl8712u \
	firmware-zd1211 \
	\
	kernel-module-ath9k-htc \
	kernel-module-carl9170 \
	kernel-module-r8712u \
	kernel-module-rt2500usb \
	kernel-module-rt2800usb \
	kernel-module-rt73usb \
	kernel-module-zd1211rw \
	"

EXTRA_KERNEL_WIFI_DRIVERS = " \
	firmware-rtl8192cu \
	\
	kernel-module-r8188eu \
	kernel-module-rtl8192cu \
	"

EXTERNAL_WIFI_DRIVERS = " \
	rtl8192cu \
	rt8188eu \
	rtl8192eu \
	"

IMAGE_INSTALL += " \
	autofs \
	ca-certificates \
	curl \
	dosfstools \
	e2fsprogs \
	e2fsprogs-resize2fs \
	findutils \
	gptfdisk \
	hdparm \
	ifupdown \
	less \
	libswscale \
	libusb1 \
	minidlna \
	nano \
	nfs-utils \
	nfs-utils-client \
	ntpdate \
	ofgwrite \
	openssh \
	openssl \
	opkg \
	oscam \
	pv \
	rpcbind \
	rsync \
	samsunglcd4linux \
	tar \
	tzdata \
	tzdata-europe \
	udev-extraconf \
	udpxy \
	util-linux-blkid \
	util-linux-mount \
	util-linux-swaponoff \
	util-linux-umount \
	virtual/screengrabber \
	virtual/neutrino \
	neutrino-plugins \
	neutrino-lua-plugins \
	neutrino-plugin-mediathek \
	xupnpd \
	zip \
"

EXTRAOPKGCONFIG = "neutrino-feed-config"

