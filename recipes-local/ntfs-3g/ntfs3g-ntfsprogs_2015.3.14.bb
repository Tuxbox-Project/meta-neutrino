# Thanks to Hongxu Jia for pushing this receipe on openembedded.core mailing list
DESCRIPTION = "The NTFS-3G driver is an open source, freely available NTFS driver for Linux with read and write support."
HOMEPAGE = "http://www.ntfs-3g.org/"
DEPENDS = "fuse libgcrypt"
PROVIDES = "ntfsprogs ntfs-3g"
LICENSE = "GPLv2 & LGPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=59530bdf33659b29e73d4adb9f9f6552 \
                    file://COPYING.LIB;md5=f30a9716ef3762e3467a2f62bf790f0a"

SRC_URI = "http://tuxera.com/opensource/ntfs-3g_ntfsprogs-${PV}.tgz"
S = "${WORKDIR}/ntfs-3g_ntfsprogs-${PV}"

SRC_URI[md5sum] = "8cd57768310e3b2be39b3191d808e241"
SRC_URI[sha256sum] = "97f996015d8316d4a272bd2629978e5e97072dd3cc148ce39802f8037c6538f2"

inherit autotools pkgconfig

# required or it calls ldconfig at install step
EXTRA_OEMAKE = "LDCONFIG=echo"

PACKAGES =+ "ntfs-3g ntfsprogs libntfs-3g"

FILES_ntfs-3g = "${base_sbindir}/*.ntfs-3g ${bindir}/ntfs-3g* ${base_sbindir}/mount.ntfs"
RDEPENDS_ntfs-3g += "fuse util-linux-mount"

FILES_ntfsprogs = "${base_sbindir}/* ${bindir}/* ${sbindir}/*"
FILES_libntfs-3g = "${libdir}/*${SOLIBS}"

do_install_append() {
    # Standard mount will execute the program /sbin/mount.TYPE
    # when called. Add the symbolic to let mount could find ntfs.
    ln -sf mount.ntfs-3g ${D}/${base_sbindir}/mount.ntfs
}
