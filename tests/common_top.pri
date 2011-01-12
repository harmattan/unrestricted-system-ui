include(../check.pri)
include(../../mconfig.pri)
include(../../localconfig.pri)

ROOTSRCDIR = ../../src
LIBNOTIFICATIONSRCDIR = $$ROOTSRCDIR/libnotificationsystem
SRCDIR = $$ROOTSRCDIR/systemui
NOTIFICATIONSRCDIR = $$SRCDIR/notifications
STUBSDIR = ../stubs
COMMONDIR = ../common
INCLUDEPATH += $$M_INSTALL_HEADERS $$SRCDIR $$STUBSDIR
DEPENDPATH = $$INCLUDEPATH
CONFIG += meegotouch link_pkgconfig qmsystem2
QT += testlib network gui dbus xml
TEMPLATE = app
DEFINES += UNIT_TEST
LIBS += -L../../lib
QMAKE_LFLAGS_RPATH = -Wl

contains(DEFINES, HAVE_CONTEXTSUBSCRIBER) {
    PKGCONFIG += contextsubscriber-1.0
}

contains(DEFINES, HAVE_LIBNGF) {
    PKGCONFIG += libngf0
}

PKGCONFIG += dbus-1

DEFINES += APPLICATION_EXTENSION_DIR=\'$$quote(\"$$M_APPLICATION_EXTENSION_DIR\")\'

# install tests
target.path = $$[QT_INSTALL_LIBS]/system-ui-tests
INSTALLS += target
