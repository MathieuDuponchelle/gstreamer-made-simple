DISTCHECK_CONFIGURE_FLAGS=--enable-gtk-doc

SUBDIRS = gms tests

DIST_SUBDIRS = $(SUBDIRS)

# include before EXTRA_DIST for win32 assignment
#include $(top_srcdir)/common/win32.mak

EXTRA_DIST = \
	gstreamer-made-simple.spec depcomp \
	AUTHORS COPYING NEWS README RELEASE \
	ChangeLog autogen.sh

DISTCLEANFILES = _stdint.h

ACLOCAL_AMFLAGS = -I m4 -I common/m4

include $(top_srcdir)/common/release.mak
include $(top_srcdir)/common/po.mak

include $(top_srcdir)/common/coverage/lcov.mak

# cruft: plugins that have been merged or moved or renamed
CRUFT_FILES = \
 $(top_builddir)/common/shave \
 $(top_builddir)/common/shave-libtool

include $(top_srcdir)/common/cruft.mak

all-local: check-cruft
