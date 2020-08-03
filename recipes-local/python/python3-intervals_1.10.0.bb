DESCRIPTION = "Interval arithmetic for Python"
HOMEPAGE = "https://github.com/AlexandreDecan/python-intervals"
SECTION = "devel/python"

LICENSE = "LGPLv3"
LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=05f1e16a8e59ce3e9a979e881816c2ab"

PYPI_PACKAGE := "python-intervals"

SRC_URI += " \
	file://run-ptest \
"

inherit pypi setuptools3 ptest

RDEPENDS_${PN}-ptest += " \
	${PYTHON_PN}-pytest \
"

do_install_ptest() {
	cp -f ${S}/test_intervals.py ${D}${PTEST_PATH}
	cp -f ${S}/README.md ${D}${PTEST_PATH}
}

SRC_URI[md5sum] = "8955317ff4e42590c90ba6247b1caaed"
SRC_URI[sha256sum] = "0d26746eaed0be78a61dd289bb7a10721b08770bb3e807614835f490d514f2a5"

BBCLASSEXTEND = "native"
