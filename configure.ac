AC_INIT([GNOME Online Miners],
        [3.14.0],
        [https://bugzilla.gnome.org/enter_bug.cgi?product=gnome-online-miners],
        [gnome-online-miners],
        [https://wiki.gnome.org/Projects/GnomeOnlineMiners])
AC_PREREQ(2.63)

AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src])

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AM_INIT_AUTOMAKE([dist-xz no-dist-gzip tar-ustar])
AM_MAINTAINER_MODE([enable])

AC_PROG_CC
AC_PROG_CC_C_O
AC_HEADER_STDC

GDATA_MIN_VERSION=0.15.2
GFBGRAPH_MIN_VERSION=0.2.2
GLIB_MIN_VERSION=2.35.1
GOA_MIN_VERSION=3.13.3
GRILO_MIN_VERSION=0.2.6
ZAPOJIT_MIN_VERSION=0.0.2

GNOME_COMPILE_WARNINGS([maximum])
GNOME_DEBUG_CHECK
GNOME_MAINTAINER_MODE_DEFINES

LT_INIT

PKG_CHECK_MODULES(GDATA, [libgdata >= $GDATA_MIN_VERSION])
PKG_CHECK_MODULES(GFBGRAPH, [libgfbgraph-0.2 >= $GFBGRAPH_MIN_VERSION])
PKG_CHECK_MODULES(GLIB, [glib-2.0 >= $GLIB_MIN_VERSION])
PKG_CHECK_MODULES(GIO, [gio-2.0 gio-unix-2.0])

PKG_CHECK_MODULES(GOA, [goa-1.0 >= $GOA_MIN_VERSION])
AC_DEFINE([GOA_API_IS_SUBJECT_TO_CHANGE], [], [We are aware that GOA's API can change])

PKG_CHECK_MODULES(GRILO, [grilo-0.2 >= $GRILO_MIN_VERSION])
PKG_CHECK_MODULES(TRACKER, [tracker-miner-1.0 tracker-sparql-1.0])
PKG_CHECK_MODULES(ZAPOJIT, [zapojit-0.0 >= $ZAPOJIT_MIN_VERSION])

AC_CONFIG_FILES([
Makefile
data/Makefile
src/Makefile
])
AC_OUTPUT
