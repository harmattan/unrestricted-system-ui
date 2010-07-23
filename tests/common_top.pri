include(../check.pri)
include(../../mconfig.pri)
include(../../localconfig.pri)
SRCDIR = ../../src
STUBSDIR = ../stubs
INCLUDEPATH += $$M_INSTALL_HEADERS $$SRCDIR $$STUBSDIR ../../include
DEPENDPATH = $$INCLUDEPATH
CONFIG += meegotouch link_pkgconfig qmsystem
QT += testlib network gui dbus xml
TEMPLATE = app
DEFINES += UNIT_TEST
LIBS += -L../../lib

contains(DEFINES, HAVE_LIBNGF) {
	PKGCONFIG += libngf0
}

PKGCONFIG += dbus-1

DEFINES += STATUSINDICATORMENU_PLUGIN_DIR=\'$$quote(\"$$STATUSINDICATORMENU_PLUGIN_DIR\")\' \
    APPLICATION_EXTENSION_DIR=\'$$quote(\"$$M_APPLICATION_EXTENSION_DIR\")\'

# install tests
target.path = $$[QT_INSTALL_LIBS]/system-ui-tests
INSTALLS += target
