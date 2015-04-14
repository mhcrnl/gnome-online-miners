pkglib_LTLIBRARIES = libgom-1.0.la

libgom_1_0_la_built_sources = \
    gom-dbus.c \
    gom-dbus.h \
    $(NULL)

nodist_libgom_1_0_la_SOURCES = \
    $(libgom_1_0_la_built_sources)
    $(NULL)

libgom_1_0_la_SOURCES = \
    gom-application.c \
    gom-application.h \
    gom-miner.c \
    gom-miner.h \
    gom-tracker.c \
    gom-tracker.h \
    gom-utils.c \
    gom-utils.h \
    tracker-ioprio.c \
    tracker-ioprio.h \
    tracker-sched.c \
    tracker-sched.h \
    $(NULL)

libgom_1_0_la_CPPFLAGS = \
    -DG_LOG_DOMAIN=\"Gom\" \
    -DG_DISABLE_DEPRECATED \
    -I$(top_srcdir)/src/ \
    $(GIO_CFLAGS) \
    $(GLIB_CFLAGS) \
    $(GOA_CFLAGS) \
    $(TRACKER_CFLAGS) \
    $(NULL)

libgom_1_0_la_LIBADD = \
    $(GIO_LIBS) \
    $(GLIB_LIBS) \
    $(GOA_LIBS) \
    $(TRACKER_LIBS) \
    $(NULL)

libgom_1_0_la_LDFLAGS = \
    -avoid-version \
    $(NULL)

libexec_PROGRAMS = \
    $(NULL)

if BUILD_FACEBOOK

libexec_PROGRAMS += \
    gom-facebook-miner \
    $(NULL)

gom_facebook_miner_SOURCES = \
    gom-facebook-miner-main.c \
    gom-facebook-miner.c \
    gom-facebook-miner.h \
    $(NULL)

gom_facebook_miner_CPPFLAGS = \
    -DG_LOG_DOMAIN=\"Gom\" \
    -DG_DISABLE_DEPRECATED \
    -I$(top_srcdir)/src \
    $(GFBGRAPH_CFLAGS) \
    $(GIO_CFLAGS) \
    $(GLIB_CFLAGS) \
    $(GOA_CFLAGS) \
    $(TRACKER_CFLAGS) \
    $(NULL)

gom_facebook_miner_LDADD = \
    libgom-1.0.la  \
    $(GFBGRAPH_LIBS) \
    $(GIO_LIBS) \
    $(GLIB_LIBS) \
    $(GOA_LIBS) \
    $(TRACKER_LIBS) \
    $(NULL)

endif # BUILD_FACEBOOK

if BUILD_FLICKR

libexec_PROGRAMS += \
    gom-flickr-miner \
    $(NULL)

gom_flickr_miner_SOURCES = \
    gom-flickr-miner-main.c \
    gom-flickr-miner.c \
    gom-flickr-miner.h \
    $(NULL)

gom_flickr_miner_CPPFLAGS = \
    -DG_LOG_DOMAIN=\"Gom\" \
    -DG_DISABLE_DEPRECATED \
    -I$(top_srcdir)/src \
    $(GIO_CFLAGS) \
    $(GLIB_CFLAGS) \
    $(GOA_CFLAGS) \
    $(GRILO_CFLAGS) \
    $(TRACKER_CFLAGS) \
    $(NULL)

gom_flickr_miner_LDADD = \
    libgom-1.0.la  \
    $(GIO_LIBS) \
    $(GLIB_LIBS) \
    $(GOA_LIBS) \
    $(GRILO_LIBS) \
    $(TRACKER_LIBS) \
    $(NULL)

endif # BUILD_FLICKR

if BUILD_GOOGLE

libexec_PROGRAMS += \
    gom-gdata-miner \
    $(NULL)

gom_gdata_miner_SOURCES = \
    gom-gdata-miner-main.c \
    gom-gdata-miner.c \
    gom-gdata-miner.h \
    $(NULL)

gom_gdata_miner_CPPFLAGS = \
    -DG_LOG_DOMAIN=\"Gom\" \
    -DG_DISABLE_DEPRECATED \
    -I$(top_srcdir)/src \
    $(GDATA_CFLAGS) \
    $(GIO_CFLAGS) \
    $(GLIB_CFLAGS) \
    $(GOA_CFLAGS) \
    $(TRACKER_CFLAGS) \
    $(NULL)

gom_gdata_miner_LDADD = \
    libgom-1.0.la  \
    $(GDATA_LIBS) \
    $(GIO_LIBS) \
    $(GLIB_LIBS) \
    $(GOA_LIBS) \
    $(TRACKER_LIBS) \
    $(NULL)

endif # BUILD_GOOGLE

if BUILD_MEDIA_SERVER

libexec_PROGRAMS += \
    gom-media-server-miner \
    $(NULL)

gom_media_server_miner_built_sources = \
    gom-dleyna-server-manager.c \
    gom-dleyna-server-manager.h \
    gom-dleyna-server-media-device.c \
    gom-dleyna-server-media-device.h \
    gom-upnp-media-container2.c \
    gom-upnp-media-container2.h \
    $(NULL)

nodist_gom_media_server_miner_SOURCES = \
    $(gom_media_server_miner_built_sources)
    $(NULL)

gom_media_server_miner_SOURCES = \
    gom-media-server-miner-main.c \
    gom-media-server-miner.c \
    gom-media-server-miner.h \
    gom-dlna-server.c \
    gom-dlna-server.h \
    gom-dlna-servers-manager.c \
    gom-dlna-servers-manager.h \
    $(NULL)

gom_media_server_miner_CPPFLAGS = \
    -DG_LOG_DOMAIN=\"Gom\" \
    -DG_DISABLE_DEPRECATED \
    -I$(top_srcdir)/src \
    $(GIO_CFLAGS) \
    $(GLIB_CFLAGS) \
    $(GOA_CFLAGS) \
    $(TRACKER_CFLAGS) \
    $(NULL)

gom_media_server_miner_LDADD = \
    libgom-1.0.la  \
    $(GIO_LIBS) \
    $(GLIB_LIBS) \
    $(GOA_LIBS) \
    $(TRACKER_LIBS) \
    $(NULL)

endif # BUILD_MEDIA_SERVER

if BUILD_OWNCLOUD

libexec_PROGRAMS += \
    gom-owncloud-miner \
    $(NULL)

gom_owncloud_miner_SOURCES = \
    gom-owncloud-miner-main.c \
    gom-owncloud-miner.c \
    gom-owncloud-miner.h \
    $(NULL)

gom_owncloud_miner_CPPFLAGS = \
    -DG_LOG_DOMAIN=\"Gom\" \
    -DG_DISABLE_DEPRECATED \
    -I$(top_srcdir)/src \
    $(GIO_CFLAGS) \
    $(GLIB_CFLAGS) \
    $(GOA_CFLAGS) \
    $(TRACKER_CFLAGS) \
    $(NULL)

gom_owncloud_miner_LDADD = \
    libgom-1.0.la  \
    $(GIO_LIBS) \
    $(GLIB_LIBS) \
    $(GOA_LIBS) \
    $(TRACKER_LIBS) \
    $(NULL)

endif # BUILD_OWNCLOUD

if BUILD_WINDOWS_LIVE

libexec_PROGRAMS += \
    gom-zpj-miner \
    $(NULL)

gom_zpj_miner_SOURCES = \
    gom-zpj-miner-main.c \
    gom-zpj-miner.c \
    gom-zpj-miner.h \
    $(NULL)

gom_zpj_miner_CPPFLAGS = \
    -DG_LOG_DOMAIN=\"Gom\" \
    -DG_DISABLE_DEPRECATED \
    -I$(top_srcdir)/src \
    $(GIO_CFLAGS) \
    $(GLIB_CFLAGS) \
    $(GOA_CFLAGS) \
    $(TRACKER_CFLAGS) \
    $(ZAPOJIT_CFLAGS) \
    $(NULL)

gom_zpj_miner_LDADD = \
    libgom-1.0.la  \
    $(GIO_LIBS) \
    $(GLIB_LIBS) \
    $(GOA_LIBS) \
    $(TRACKER_LIBS) \
    $(ZAPOJIT_LIBS) \
    $(NULL)

endif # BUILD_WINDOWS_LIVE

BUILT_SOURCES = \
    $(libgom_1_0_la_built_sources) \
    $(gom_media_server_miner_built_sources)
    $(NULL)

CLEANFILES = \
    $(BUILT_SOURCES) \
    $(NULL)

EXTRA_DIST = \
    gom-dbus.xml \
    gom-dleyna-server-manager.xml \
    gom-dleyna-server-media-device.xml \
    gom-upnp-media-container2.xml \
    gom-miner-main.c \
    $(NULL)

gom-dleyna-server-media-device.h gom-dleyna-server-media-device.c: gom-dleyna-server-media-device.xml
	$(AM_V_GEN)gdbus-codegen \
		--c-namespace DleynaServer \
		--generate-c-code gom-dleyna-server-media-device \
		--interface-prefix com.intel.dLeynaServer. \
		$<

gom-dleyna-server-manager.h gom-dleyna-server-manager.c: gom-dleyna-server-manager.xml
	$(AM_V_GEN)gdbus-codegen \
		--c-namespace DleynaServer \
		--generate-c-code gom-dleyna-server-manager \
		--interface-prefix com.intel.dLeynaServer. \
		$<

gom-upnp-media-container2.h gom-upnp-media-container2.c: gom-upnp-media-container2.xml
	$(AM_V_GEN)gdbus-codegen \
		--c-namespace Upnp \
		--generate-c-code gom-upnp-media-container2 \
		--interface-prefix org.gnome.UPnP. \
		$<

gom-dbus.h gom-dbus.c: gom-dbus.xml
	$(AM_V_GEN)gdbus-codegen \
		--c-namespace Gom \
		--generate-c-code gom-dbus \
		--interface-prefix org.gnome.OnlineMiners. \
		--annotate "org.gnome.OnlineMiners.Miner" org.gtk.GDBus.C.Name DBus \
		$<

-include $(top_srcdir)/git.mk
