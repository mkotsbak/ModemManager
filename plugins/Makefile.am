
##########################
# Icera-specific support #
##########################

noinst_LTLIBRARIES = libicera-utils.la

libicera_utils_la_SOURCES = \
	mm-modem-icera.c \
	mm-modem-icera.h

libicera_utils_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src

libicera_utils_la_LIBADD = \
	$(GUDEV_LDFLAGS)

########################################

pkglib_LTLIBRARIES = \
	libmm-plugin-generic.la \
	libmm-plugin-moto-c.la \
	libmm-plugin-gobi.la \
	libmm-plugin-huawei.la \
	libmm-plugin-hso.la \
	libmm-plugin-option.la \
	libmm-plugin-sierra.la \
	libmm-plugin-novatel.la \
	libmm-plugin-nokia.la \
	libmm-plugin-zte.la \
	libmm-plugin-mbm.la \
	libmm-plugin-longcheer.la \
	libmm-plugin-anydata.la \
	libmm-plugin-simtech.la \
	libmm-plugin-x22x.la \
	libmm-plugin-linktop.la \
	libmm-plugin-samsung_lte.la
# Generic

libmm_plugin_generic_la_SOURCES = \
	mm-plugin-generic.c \
	mm-plugin-generic.h

libmm_plugin_generic_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src

libmm_plugin_generic_la_LDFLAGS = \
	$(GUDEV_LDFLAGS) \
	-module \
	-avoid-version

# Motorola C-series phones

libmm_plugin_moto_c_la_SOURCES = \
	mm-plugin-moto-c.c \
	mm-plugin-moto-c.h \
	mm-modem-moto-c-gsm.c \
	mm-modem-moto-c-gsm.h

libmm_plugin_moto_c_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src

libmm_plugin_moto_c_la_LDFLAGS = \
	$(GUDEV_LDFLAGS) \
	-module \
	-avoid-version

# Qualcomm Gobi

libmm_plugin_gobi_la_SOURCES = \
	mm-plugin-gobi.c \
	mm-plugin-gobi.h \
	mm-modem-gobi-gsm.c \
	mm-modem-gobi-gsm.h

libmm_plugin_gobi_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src

libmm_plugin_gobi_la_LDFLAGS = \
	$(GUDEV_LDFLAGS) \
	-module \
	-avoid-version

# Huawei

libmm_plugin_huawei_la_SOURCES = \
	mm-plugin-huawei.c \
	mm-plugin-huawei.h \
	mm-modem-huawei-gsm.c \
	mm-modem-huawei-gsm.h \
	mm-modem-huawei-cdma.c \
	mm-modem-huawei-cdma.h

libmm_plugin_huawei_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src

libmm_plugin_huawei_la_LDFLAGS = \
	$(GUDEV_LDFLAGS) \
	-module \
	-avoid-version

# HSO

libmm_plugin_hso_la_SOURCES = \
	mm-plugin-hso.c \
	mm-plugin-hso.h \
	mm-modem-gsm-hso-glue.h \
	mm-modem-hso.c \
	mm-modem-hso.h

mm-modem-gsm-hso-glue.h: $(top_srcdir)/introspection/org.freedesktop.ModemManager.Modem.Gsm.Hso.xml
	$(AM_V_GEN) dbus-binding-tool --prefix=mm_modem_gsm_hso --mode=glib-server --output=$@ $<

libmm_plugin_hso_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src

libmm_plugin_hso_la_LDFLAGS = \
	$(GUDEV_LDFLAGS) \
	-module \
	-avoid-version

# MBM

libmm_plugin_mbm_la_SOURCES = \
	mm-plugin-mbm.c \
	mm-plugin-mbm.h \
	mm-modem-mbm.c \
	mm-modem-mbm.h

libmm_plugin_mbm_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src

libmm_plugin_mbm_la_LDFLAGS = \
	$(GUDEV_LDFLAGS) \
	-module \
	-avoid-version

# Option

libmm_plugin_option_la_SOURCES = \
	mm-plugin-option.c \
	mm-plugin-option.h \
	mm-modem-option.c \
	mm-modem-option.h

libmm_plugin_option_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src

libmm_plugin_option_la_LDFLAGS = \
	$(GUDEV_LDFLAGS) \
	-module \
	-avoid-version

# Sierra

libmm_plugin_sierra_la_SOURCES = \
	mm-plugin-sierra.c \
	mm-plugin-sierra.h \
	mm-modem-sierra-gsm.c \
	mm-modem-sierra-gsm.h \
	mm-modem-sierra-cdma.c \
	mm-modem-sierra-cdma.h

libmm_plugin_sierra_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src

libmm_plugin_sierra_la_LDFLAGS = \
	$(GUDEV_LDFLAGS) \
	-module \
	-avoid-version

# Novatel

libmm_plugin_novatel_la_SOURCES = \
	mm-plugin-novatel.c \
	mm-plugin-novatel.h \
	mm-modem-novatel-gsm.c \
	mm-modem-novatel-gsm.h \
	mm-modem-novatel-cdma.c \
	mm-modem-novatel-cdma.h

libmm_plugin_novatel_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)

libmm_plugin_novatel_la_LDFLAGS = \
	$(GUDEV_LDFLAGS) \
	-module \
	-avoid-version

# Nokia

libmm_plugin_nokia_la_SOURCES = \
	mm-plugin-nokia.c \
	mm-plugin-nokia.h \
	mm-modem-nokia.c \
	mm-modem-nokia.h

libmm_plugin_nokia_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src

libmm_plugin_nokia_la_LDFLAGS = \
	$(GUDEV_LDFLAGS) \
	-module \
	-avoid-version

# Zte

libmm_plugin_zte_la_SOURCES = \
	mm-plugin-zte.c \
	mm-plugin-zte.h \
	mm-modem-zte.c \
	mm-modem-zte.h

libmm_plugin_zte_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src

libmm_plugin_zte_la_LDFLAGS = \
	$(GUDEV_LDFLAGS) \
	$(builddir)/libicera-utils.la \
	-module \
	-avoid-version

# Longcheer (and rebranded dongles)

libmm_plugin_longcheer_la_SOURCES = \
	mm-plugin-longcheer.c \
	mm-plugin-longcheer.h \
	mm-modem-longcheer-gsm.c \
	mm-modem-longcheer-gsm.h

libmm_plugin_longcheer_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src

libmm_plugin_longcheer_la_LDFLAGS = \
	$(GUDEV_LDFLAGS) \
	-module \
	-avoid-version

# AnyData CDMA

libmm_plugin_anydata_la_SOURCES = \
	mm-plugin-anydata.c \
	mm-plugin-anydata.h \
	mm-modem-anydata-cdma.c \
	mm-modem-anydata-cdma.h

libmm_plugin_anydata_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src

libmm_plugin_anydata_la_LDFLAGS = \
	$(GUDEV_LDFLAGS) \
	-module \
	-avoid-version

# SimTech

libmm_plugin_simtech_la_SOURCES = \
	mm-plugin-simtech.c \
	mm-plugin-simtech.h \
	mm-modem-simtech-gsm.c \
	mm-modem-simtech-gsm.h

libmm_plugin_simtech_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src

libmm_plugin_simtech_la_LDFLAGS = \
	$(GUDEV_LDFLAGS) \
	-module \
	-avoid-version

# Alcatel/TCT/JRD x220D and possibly others

libmm_plugin_x22x_la_SOURCES = \
	mm-plugin-x22x.c \
	mm-plugin-x22x.h \
	mm-modem-x22x-gsm.c \
	mm-modem-x22x-gsm.h

libmm_plugin_x22x_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src

libmm_plugin_x22x_la_LDFLAGS = \
	$(GUDEV_LDFLAGS) \
	-module \
	-avoid-version

# Linktop

libmm_plugin_linktop_la_SOURCES = \
	mm-plugin-linktop.c \
	mm-plugin-linktop.h \
	mm-modem-linktop.c \
	mm-modem-linktop.h

libmm_plugin_linktop_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src

libmm_plugin_linktop_la_LDFLAGS = \
	$(GUDEV_LDFLAGS) \
	-module \
	-avoid-version

# Samsung LTE

libmm_plugin_samsung_lte_la_SOURCES = \
	mm-plugin-samsung.c \
	mm-plugin-samsung-lte.h \
	mm-modem-samsung-lte.c \
	mm-modem-samsung-lte.h

libmm_plugin_samsung_lte_la_CPPFLAGS = \
	$(MM_CFLAGS) \
	$(GUDEV_CFLAGS) \
	-I$(top_srcdir)/src

libmm_plugin_samsung_lte_la_LDFLAGS = \
	$(GUDEV_LDFLAGS) \
	-module \
	-avoid-version

udevrulesdir = $(UDEV_BASE_DIR)/rules.d
udevrules_DATA = \
	77-mm-ericsson-mbm.rules \
	77-mm-zte-port-types.rules \
	77-mm-longcheer-port-types.rules \
	77-mm-simtech-port-types.rules \
	77-mm-x22x-port-types.rules \
	77-mm-samsung-lte.rules

BUILT_SOURCES = \
	mm-modem-gsm-hso-glue.h

CLEANFILES = $(BUILT_SOURCES)

EXTRA_DIST = \
	$(udevrules_DATA) \
	mm-modem-option-utils.c

