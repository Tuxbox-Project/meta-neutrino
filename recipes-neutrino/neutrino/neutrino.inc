
PROVIDES = "virtual/neutrino"
RPROVIDES_${PN} = "virtual/neutrino"

inherit autotools pkgconfig systemd ccache

DEPENDS += " \
	curl \
	ffmpeg \
	flac \
	freetype \
	freetype-native \
	fribidi \
	gettext \
	giflib \
	glib-2.0 \
	libass \
	libbluray \
	libdvbsi++ \
	libid3tag \
	libjpeg-turbo \
	pugixml \
	libpng \
	libsigc++-2.0\
	lua \
	luaposix \
	openssl \
	openthreads \
	virtual/libstb-hal \
	virtual/libiconv \
	fontconfig \
	libpng \
	tremor \
	zlib \
"

RDEPENDS_${PN} = "streamripper virtual/libstb-hal lua luaposix lua-expat ffmpeg "

S = "${WORKDIR}/git"

SYSTEMD_SERVICE_${PN} = "neutrino.service"

YT_DEV_KEY ?= "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

TMDB_DEV_KEY ?= "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

SHOUTCAST_DEV_KEY ?= "XXXXXXXXXXXXXXXX"

OMDB_API_KEY ?= "XXXXXXXX"

N_PREFIX = "/usr"

N_EXEC_PREFIX = "${N_PREFIX}/bin"

EXTRA_OECONF = " \
	--enable-maintainer-mode \
	--enable-silent-rules \
	--enable-upnp \
	--enable-mdev \
	--enable-fribidi \
	--enable-pugixml \
	--enable-giflib \
	--with-tremor \
	--enable-flac \
	--enable-ffmpegdec \
	--enable-lua \
	--with-target=native \
	--prefix=${N_PREFIX} \
	--exec_prefix=${N_EXEC_PREFIX} \
	--includedir=${N_PREFIX}/include \
	--with-configdir=/etc/neutrino/config \
	--with-controldir_var=/etc/neutrino/config \
	--with-plugindir=/usr/share/tuxbox/neutrino/plugins \
        --with-plugindir_var=/etc/neutrino/plugins \
	--with-webtvdir=/usr/share/tuxbox/neutrino/plugins/webtv \
	--with-webtvdir_var=/etc/neutrino/plugins/webtv \
        --with-webradiodir=/usr/share/tuxbox/neutrino/plugins/webradio \
        --with-webradiodir_var=/etc/neutrino/plugins/webradio \
	--with-luaplugindir=/usr/share/tuxbox/neutrino/plugins \
	--with-gamesdir=/etc/neutrino/plugins \
	--with-tmdb-dev-key="${TMDB_DEV_KEY}" \
	--with-omdb-api-key="${OMDB_API_KEY}" \
	--with-youtube-dev-key="${YT_DEV_KEY}" \
	--with-shoutcast-dev-key="${SHOUTCAST_DEV_KEY}" \
	--with-weather-dev-key="${WEATHER_DEV_KEY}" \
        --disable-tmdb-key-manage \
        --disable-omdb-key-manage \
        --disable-youtube-key-manage \
        --disable-shoutcast-key-manage \
	--disable-weather-key-manage \
"

EXTRA_OEMAKE += "'ARFLAGS=rc'"

# VERSION_FILE = "${N_PREFIX}/.version"

N_CFLAGS = "-Wall -Wextra -Wshadow -Wno-psabi -g -O2 -fno-strict-aliasing -rdynamic -DENABLE_FREESATEPG -DNEW_LIBCURL \
-I${S}/lib/libcoolstream2 -I${STAGING_INCDIR}/freetype2 -I${STAGING_INCDIR}/sigc++-2.0 -I${STAGING_INCDIR}/glib-2.0 -I${STAGING_LIBDIR}/glib-2.0/include"

N_CXXFLAGS = "${N_CFLAGS}"

N_LDFLAGS += "-Wl,--hash-style=gnu -Wl,-rpath-link, ${STAGING_LIBDIR} -lcurl -ldl -lass"
N_CPPFLAGS = "-DDYNAMIC_LUAPOSIX -D__KERNEL_STRICT_NAMES -D__STDC_CONSTANT_MACROS"
CPPFLAGS = "${N_CPPFLAGS}"

do_configure_prepend() {
        ln -sf ${B}/src/gui/version.h ${S}/src/gui/
}


do_compile () {
	# unset CFLAGS CXXFLAGS LDFLAGS
	oe_runmake CFLAGS="${N_CFLAGS}" CXXFLAGS="${N_CXXFLAGS}" LDFLAGS="${N_LDFLAGS}"
}


do_install_prepend () {
	install -d ${D}${sysconfdir}/network
	install -d ${D}${systemd_unitdir}/system
	install -d ${D}${bindir}
	install -d ${D}${sysconfdir}/neutrino/config
	install -d ${D}${sysconfdir}/neutrino/plugins	
	install -m 0644 ${WORKDIR}/neutrino.service ${D}${systemd_unitdir}/system/
	install -m 0644 ${WORKDIR}/neutrino-log.service  ${D}${systemd_unitdir}/system
	install -m 0755 ${WORKDIR}/neutrino.sh_${FLAVOUR} ${D}${bindir}/neutrino.sh   
	install -m 755 ${WORKDIR}/pre-wlan0.sh ${D}${sysconfdir}/network/
	install -m 755 ${WORKDIR}/post-wlan0.sh ${D}${sysconfdir}/network/
	install -m 644 ${WORKDIR}/timezone.xml ${D}${sysconfdir}/timezone.xml
	install -m 755 ${WORKDIR}/tobackup.conf ${D}${sysconfdir}/neutrino/config
	install -d ${D}${localstatedir}/cache
	install -d ${D}/lib/mdev/fs
	install -m 755 ${WORKDIR}/mount.mdev ${D}/lib/mdev/fs/mount

	# why this ???
	touch ${D}/usr/bin/enigma2
}

do_install_append() {
	install -d ${D}${sysconfdir}/neutrino/bin ${D}${sysconfdir}/neutrino/plugins/luaplugins
	install -d ${D}${sysconfdir}/neutrino/plugins/webradio ${D}${sysconfdir}/neutrino/plugins/webtv
	# We install update.urls file, but with suffix '.disable'. 
	# That causes disabling the formal online flash update menu entry within Neutrino which is using update.urls.
	# Online flash updates will working only with the stb-flash.lua script.
	# Nevertheless, you can remove the suffix '.disable', but flashing will never works.
	if [ ! -z ${RELEASE_TEXT_LOCATION} ]; then
		echo "${RELEASE_TEXT_LOCATION}" > ${D}/etc/update.urls.disable
	fi
	cp -rf ${WORKDIR}/etc ${D}
	install -m 644 ${WORKDIR}/update_download.jpg ${D}${datadir}/tuxbox/neutrino/icons
	install -m 644 ${WORKDIR}/update_decompress.jpg ${D}${datadir}/tuxbox/neutrino/icons
	install -m 644 ${WORKDIR}/update_kernel.jpg ${D}${datadir}/tuxbox/neutrino/icons
	install -m 644 ${WORKDIR}/update_rootfs.jpg ${D}${datadir}/tuxbox/neutrino/icons
	install -m 644 ${WORKDIR}/update_done.jpg ${D}${datadir}/tuxbox/neutrino/icons
}

FILES_${PN} += "\
	/etc \
	/usr/share \
	/lib \
"

systemd_prerm() {
}

systemd_postinst() {
OPTS=""

if [ -n "$D" ]; then
    OPTS="--root=$D"
fi

if type systemctl >/dev/null 2>/dev/null; then
	if [ -z "$D" ]; then
		systemctl daemon-reload
	fi

	systemctl $OPTS ${SYSTEMD_AUTO_ENABLE} ${SYSTEMD_SERVICE}
fi
}


pkg_postinst_ontarget_${PN} () {
	update-alternatives --install /bin/backup.sh backup.sh /usr/bin/backup.sh 100
	update-alternatives --install /bin/install.sh install.sh /usr/bin/install.sh 100
	update-alternatives --install /bin/restore.sh restore.sh /usr/bin/restore.sh 100
	# pic2m2v is only available on platforms that use "real" libstb-hal
	if which pic2m2v >/dev/null 2>&1; then
		# neutrino icon path
		I=/usr/share/tuxbox/neutrino/icons
		pic2m2v $I/mp3.jpg $I/radiomode.jpg $I/scan.jpg $I/shutdown.jpg $I/start.jpg
	fi
}
