servicedir = $(datadir)/dbus-1/services
service_DATA = $(service_in_files:.service.in=.service)

org.gnome.OnlineMiners.Facebook.service: org.gnome.OnlineMiners.Facebook.service.in Makefile
	$(AM_V_GEN)	\
		[ -d $(@D) ] || $(mkdir_p) $(@D) ; \
		sed -e "s|\@libexecdir\@|$(libexecdir)|" $< > $@.tmp && mv $@.tmp $@

org.gnome.OnlineMiners.Flickr.service: org.gnome.OnlineMiners.Flickr.service.in Makefile
	$(AM_V_GEN)	\
		[ -d $(@D) ] || $(mkdir_p) $(@D) ; \
		sed -e "s|\@libexecdir\@|$(libexecdir)|" $< > $@.tmp && mv $@.tmp $@

org.gnome.OnlineMiners.GData.service: org.gnome.OnlineMiners.GData.service.in Makefile
	$(AM_V_GEN)	\
		[ -d $(@D) ] || $(mkdir_p) $(@D) ; \
		sed -e "s|\@libexecdir\@|$(libexecdir)|" $< > $@.tmp && mv $@.tmp $@

org.gnome.OnlineMiners.MediaServer.service: org.gnome.OnlineMiners.MediaServer.service.in Makefile
	$(AM_V_GEN)	\
		[ -d $(@D) ] || $(mkdir_p) $(@D) ; \
		sed -e "s|\@libexecdir\@|$(libexecdir)|" $< > $@.tmp && mv $@.tmp $@

org.gnome.OnlineMiners.Owncloud.service: org.gnome.OnlineMiners.Owncloud.service.in Makefile
	$(AM_V_GEN)	\
		[ -d $(@D) ] || $(mkdir_p) $(@D) ; \
		sed -e "s|\@libexecdir\@|$(libexecdir)|" $< > $@.tmp && mv $@.tmp $@

org.gnome.OnlineMiners.Zpj.service: org.gnome.OnlineMiners.Zpj.service.in Makefile
	$(AM_V_GEN)	\
		[ -d $(@D) ] || $(mkdir_p) $(@D) ; \
		sed -e "s|\@libexecdir\@|$(libexecdir)|" $< > $@.tmp && mv $@.tmp $@

service_in_files = \
    org.gnome.OnlineMiners.Facebook.service.in \
    org.gnome.OnlineMiners.Flickr.service.in \
    org.gnome.OnlineMiners.GData.service.in \
    org.gnome.OnlineMiners.MediaServer.service.in \
    org.gnome.OnlineMiners.Owncloud.service.in \
    org.gnome.OnlineMiners.Zpj.service.in \
    $(NULL)

EXTRA_DIST= \
    $(service_in_files) \
    $(NULL)

CLEANFILES = \
    $(service_DATA) \
    $(NULL)

-include $(top_srcdir)/git.mk
