include(../check.pri)
include(../../mconfig.pri)
SRCDIR = ../../src
STUBSDIR = ../stubs
INCLUDEPATH += $$M_INSTALL_HEADERS $$SRCDIR $$STUBSDIR ../../include
DEPENDPATH = $$INCLUDEPATH
CONFIG += meegotouch link_pkgconfig
QT += testlib network gui dbus xml
exists($$[QT_INSTALL_LIBS]/libQtOpenGL.so):QT += opengl
TEMPLATE = app
DEFINES += UNIT_TEST
LIBS += -L../../lib

PKGCONFIG += \
          qmsystem \
          libngf0 \
          dbus-1

DEFINES += STATUSINDICATORMENU_PLUGIN_DIR=\'$$quote(\"$$STATUSINDICATORMENU_PLUGIN_DIR\")\' \
    APPLICATION_EXTENSION_DIR=\'$$quote(\"$$M_APPLICATION_EXTENSION_DIR\")\'

# install tests
target.path = $$[QT_INSTALL_LIBS]/system-ui-tests
INSTALLS += target
