# Recipe created by recipetool

inherit allarch

IMAGE_FEATURES += " ${PN} "

DESCRIPTION = "Tangos Theme Set"
HOMEPAGE = "https://github.com/TangoCash/neutrino-tangos"
MAINTAINER = "Tango"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263 \
"

SRC_URI = "git://github.com/TangoCash/neutrino-tangos.git;protocol=https"


PR = "r1"
SRCREV = "${AUTOREV}"
PV = "git-${SRCPV}"

S = "${WORKDIR}/git"

do_install () {
	install -d ${D}/var/tuxbox/themes
        install -m 644 ${S}/data/themes/GlassHD.theme ${D}/var/tuxbox/themes
        install -m 644 ${S}/data/themes/Lechuk.theme ${D}/var/tuxbox/themes
        install -m 644 ${S}/data/themes/Sportster.theme ${D}/var/tuxbox/themes
        install -m 644 ${S}/data/themes/TangoCash.theme ${D}/var/tuxbox/themes
        install -m 644 ${S}/data/themes/adtheme.theme ${D}/var/tuxbox/themes
        install -m 644 ${S}/data/themes/bgfade.theme ${D}/var/tuxbox/themes
        install -m 644 ${S}/data/themes/colors.theme ${D}/var/tuxbox/themes
        install -m 644 ${S}/data/themes/dbox-all.theme ${D}/var/tuxbox/themes
        install -m 644 ${S}/data/themes/megasat4000.theme ${D}/var/tuxbox/themes
        install -m 644 ${S}/data/themes/nesh.theme ${D}/var/tuxbox/themes
}


FILES_${PN} = "\
    /var/tuxbox/themes \
"

