built_header_make =
built_source_make =

lib_LTLIBRARIES = libgms-@GST_API_VERSION@.la

EXTRA_libgms_@GST_API_VERSION@_la_SOURCES = gmsmarshal.list

CLEANFILES = $(BUILT_SOURCES) $(built_header_make) $(built_source_make) *.gcno *.gcda *.gcov *.gcov.out

libgms_@GST_API_VERSION@_la_SOURCES = 		\
	$(built_source_make)			\
	gms.c                                     \
	gms-video-stream-comparator.c            \
	gms-waveformer.c                         \
	gms-cairo-waveform-renderer.c              \
	gms-utils.c

libgms_@GST_API_VERSION@includedir = $(includedir)/gstreamer-@GST_API_VERSION@/gms/
libgms_@GST_API_VERSION@include_HEADERS = 	\
	$(built_header_make)			\
	gms.h                                   \
	gms-types.h                             \
	gms-enums.h                             \
	gms-video-stream-comparator.h           \
	gms-cairo-waveform-renderer.h           \
	gms-waveformer.h                        \
	gms-utils.h

libgms_@GST_API_VERSION@_la_CFLAGS = -I$(top_srcdir) $(GST_PBUTILS_CFLAGS) \
		$(GST_VIDEO_CFLAGS) $(GST_CONTROLLER_CFLAGS) $(GST_PLUGINS_BASE_CFLAGS) \
		$(GST_CFLAGS) $(XML_CFLAGS) $(GIO_CFLAGS) $(cairo_CFLAGS)
libgms_@GST_API_VERSION@_la_LIBADD = $(GST_PBUTILS_LIBS) \
		$(GST_VIDEO_LIBS) $(GST_CONTROLLER_LIBS) $(GST_PLUGINS_BASE_LIBS) \
		$(GST_BASE_LIBS) $(GST_LIBS) $(XML_LIBS) $(GIO_LIBS) $(cairo_LIBS)
libgms_@GST_API_VERSION@_la_LDFLAGS = $(GST_LIB_LDFLAGS) $(GST_ALL_LDFLAGS) \
		$(GST_LT_LDFLAGS) $(GIO_CFLAGS) $(cairo_LIBS)

DISTCLEANFILE = $(CLEANFILES)

#files built on make all/check/instal
BUILT_SOURCES =			\
	$(built_header_make)	\
	$(built_source_make)

include $(top_srcdir)/common/gst-glib-gen.mak

if HAVE_INTROSPECTION
BUILT_GIRSOURCES = GMS-@GST_API_VERSION@.gir

gir_headers=$(patsubst %,$(srcdir)/%, $(libgms_@GST_API_VERSION@include_HEADERS))
gir_headers+=$(patsubst %,$(builddir)/%, $(nodist_libgms_@GST_API_VERSION@include_HEADERS))
gir_sources=$(patsubst %,$(srcdir)/%, $(libgms_@GST_API_VERSION@_la_SOURCES))
gir_sources+=$(patsubst %,$(builddir)/%, $(nodist_libgms_@GST_API_VERSION@_la_SOURCES))
gir_cincludes=$(patsubst %,--c-include='gms/%',$(libgms_@GST_API_VERSION@include_HEADERS))
gir_cincludes+=$(patsubst %,--c-include='gms/%',$(nodist_libgms_@GST_API_VERSION@include_HEADERS))

GMS-@GST_API_VERSION@.gir: $(INTROSPECTION_SCANNER) libgms-@GST_API_VERSION@.la
	$(AM_V_GEN)PKG_CONFIG_PATH="$(GST_PKG_CONFIG_PATH)" \
		$(INTROSPECTION_SCANNER) -v --namespace GMS \
		--nsversion=@GST_API_VERSION@ \
		--identifier-prefix=GMS \
		--symbol-prefix=gms \
		--warn-all \
		-v \
		$(gir_cincludes) \
		-I$(top_srcdir) \
		-I$(top_builddir) \
		--add-include-path=`$(PKG_CONFIG) --variable=girdir gstreamer-@GST_API_VERSION@` \
		--add-include-path=`$(PKG_CONFIG) --variable=girdir gstreamer-pbutils-@GST_API_VERSION@` \
		--add-include-path=`$(PKG_CONFIG) --variable=girdir gstreamer-audio-@GST_API_VERSION@` \
		--add-include-path=`$(PKG_CONFIG) --variable=girdir gstreamer-video-@GST_API_VERSION@` \
		--add-include-path=`$(PKG_CONFIG) --variable=girdir gstreamer-tag-@GST_API_VERSION@` \
		--add-include-path=`$(PKG_CONFIG) --variable=girdir gstreamer-base-@GST_API_VERSION@` \
		--add-include-path=`$(PKG_CONFIG) --variable=girdir gio-2.0` \
		--add-include-path=`$(PKG_CONFIG) cairo --cflags --libs` \
		--library=libgms-@GST_API_VERSION@.la \
		--include=Gst-@GST_API_VERSION@ \
		--include=GstPbutils-@GST_API_VERSION@ \
		--include=Gio-2.0 \
		--include=cairo-1.0 \
		--libtool="$(top_builddir)/libtool" \
		--pkg gstreamer-@GST_API_VERSION@ \
		--pkg gstreamer-pbutils-@GST_API_VERSION@ \
		--pkg gstreamer-controller-@GST_API_VERSION@ \
		--pkg gio-2.0 \
		--pkg cairo \
		--pkg-export gms-@GST_API_VERSION@ \
		--add-init-section="gst_init(NULL, NULL);" \
		--add-init-section="gms_init(NULL, NULL);" \
		--output $@ \
		$(gir_headers) \
		$(gir_sources)

# INTROSPECTION_GIRDIR/INTROSPECTION_TYPELIBDIR aren't the right place to
# install anything - we need to install inside our prefix.
girdir = $(datadir)/gir-1.0
gir_DATA = $(BUILT_GIRSOURCES)

typelibsdir = $(libdir)/girepository-1.0/

typelibs_DATA = $(BUILT_GIRSOURCES:.gir=.typelib)

%.typelib: %.gir $(INTROSPECTION_COMPILER)
	$(AM_V_GEN)PKG_CONFIG_PATH="$(GST_PKG_CONFIG_PATH)" \
		$(INTROSPECTION_COMPILER) \
		--includedir=$(srcdir) \
		--includedir=$(srcdir)/../video \
		--includedir=$(builddir) \
		--includedir=`$(PKG_CONFIG) --variable=girdir gstreamer-@GST_API_VERSION@` \
		--includedir=`$(PKG_CONFIG) --variable=girdir gstreamer-pbutils-@GST_API_VERSION@` \
		--includedir=`$(PKG_CONFIG) --variable=girdir gstreamer-audio-@GST_API_VERSION@` \
		--includedir=`$(PKG_CONFIG) --variable=girdir gstreamer-video-@GST_API_VERSION@` \
		--includedir=`$(PKG_CONFIG) --variable=girdir gstreamer-tag-@GST_API_VERSION@` \
		--includedir=`$(PKG_CONFIG) --variable=girdir gstreamer-base-@GST_API_VERSION@` \
		--includedir=`$(PKG_CONFIG) --variable=girdir gstreamer-controller-@GST_API_VERSION@` \
		--includedir=`$(PKG_CONFIG) --variable=girdir gio-2.0` \
		$(INTROSPECTION_COMPILER_OPTS) $< -o $(@F)

CLEANFILES += $(BUILT_GIRSOURCES) $(typelibs_DATA)
endif

%.c.gcov: .libs/libgms_@GST_API_VERSION@_la-%.gcda %.c
	$(GCOV) -b -f -o $^ > $@.out

gcov: $(libgms_@GST_API_VERSION@_la_SOURCES:=.gcov)

Android.mk: Makefile.am $(BUILT_SOURCES)
	androgenizer -:PROJECT libgms -:SHARED libgms-@GST_API_VERSION@ \
	 -:TAGS eng debug \
         -:REL_TOP $(top_srcdir) -:ABS_TOP $(abs_top_srcdir) \
	 -:SOURCES $(libgms_@GST_API_VERSION@_la_SOURCES) \
	           $(nodist_libgms_@GST_API_VERSION@_la_SOURCES) \
	 -:CFLAGS $(DEFS) $(libgms_@GST_API_VERSION@_la_CFLAGS) \
	 -:LDFLAGS $(libgms_@GST_API_VERSION@_la_LDFLAGS) \
	           $(libgms_@GST_API_VERSION@_la_LIBADD) \
	           -ldl \
	 -:HEADER_TARGET gstreamer-@GST_API_VERSION@/gst/interfaces \
	 -:HEADERS $(libgms_@GST_API_VERSION@include_HEADERS) \
	           $(nodist_libgms_@GST_API_VERSION@include_HEADERS) \
	 -:PASSTHROUGH LOCAL_ARM_MODE:=arm \
	> $@
