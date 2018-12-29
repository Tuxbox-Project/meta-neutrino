DESCRIPTION = "Settings for Neutrino made by Matze - Astra + Hotbird"
LICENSE = "BSD-2-Clause"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/BSD-2-Clause;md5=8bef8e6712b1be5aa76af1ebde9d6378"
HOMEPAGE = "http://matzesetting.brinkster.net"

S = "${WORKDIR}"

RREPLACES_${PN} = "settings-annie-19.2e-13.0e-23.5e-28.2e settings-annie-19.2e-13.0e-23.5e settings-annie-19.2e-13.0e settings-annie-19.2e settings-matze-astra settings-pathauf settings-annie-19.2e-13.0e-23.5e-28.2e-26.0e-0.8w-5.0w"
RCONFLICTS_${PN} = "settings-annie-19.2e-13.0e-23.5e-28.2e settings-annie-19.2e-13.0e-23.5e settings-annie-19.2e-13.0e settings-annie-19.2e settings-matze-astra settings-pathauf settings-annie-19.2e-13.0e-23.5e-28.2e-26.0e-0.8w-5.0w"

SRC_URI = "http://matzesetting.brinkster.net/db/wizard/neutrino-hd/astra_hotbird/cs-astra+hb.tar"

do_install () {
	install -d ${D}/etc/neutrino/config/zapit  
        install -m 644 ${WORKDIR}/*.xml ${D}/etc/neutrino/config/zapit
	rm ${D}/etc/neutrino/config/zapit/satellites.xml
}

FILES_${PN} = "\
    /etc/neutrino/config/zapit \
"

SRC_URI[md5sum] = "1a7f9595a808c54272b19bfff0e179f9"
SRC_URI[sha256sum] = "a7ed9934c436524f88c5903b4ac33a5e72a260fac4cae5787b0e3178d07e6e2d"

