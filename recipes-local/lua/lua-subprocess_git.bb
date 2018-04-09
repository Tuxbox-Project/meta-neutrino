DESCRIPTION = "lua-subprocess"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
HOMEPAGE = "https://github.com/xlq/lua-subprocess.git"
DEPENDS += "lua glib-2.0"
RDEPENDS_${PN} += "lua"


SRC_URI = "git://github.com/xlq/lua-subprocess.git;protocol=https \
	   file://0001-subprocess.c-include-signal.h.patch \
	   file://Makefile \
"		

SRCREV = "${AUTOREV}"
PV = "${SRCPV}"
PR = "1"

S = "${WORKDIR}/git"

inherit autotools-brokensep

TARGET_CC_ARCH += "${LDFLAGS}" 

CFLAGS_append += "-I${STAGING_INCDIR}"

do_configure_prepend() {
	cp -rf ${WORKDIR}/Makefile ${S}/Makefile
}

do_install () {
	install -d ${D}${libdir}/lua/5.2
	install -m 755 ${S}/subprocess.so ${D}${libdir}/lua/5.2
}

FILES_${PN} = "/usr/lib"


BBCLASSEXTEND = "native nativesdk"

