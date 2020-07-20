# Recipe created by recipetool

inherit allarch

DESCRIPTION = "NI Theme Set"
HOMEPAGE = "https://github.com/neutrino-images/ni-neutrino"
MAINTAINER = "NI-Team"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263 \
"

SRC_URI = "git://github.com/neutrino-images/ni-neutrino.git;protocol=https"

PR = "r1"
SRCREV = "${AUTOREV}"
PV = "3.60-git-${SRCPV}"

S = "${WORKDIR}/git"

do_install () {
	install -d ${D}/var/tuxbox/themes  
        install -m 644 ${S}/data/themes/Material*.theme ${D}/var/tuxbox/themes
        install -m 644 ${S}/data/themes/Adult*.theme ${D}/var/tuxbox/themes
        install -m 644 ${S}/data/themes/Dark*.theme ${D}/var/tuxbox/themes
}


FILES_${PN} = "\
    /var/tuxbox/themes \
"
