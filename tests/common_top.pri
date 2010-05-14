include(../check.pri)
include(../../mconfig.pri)
SRCDIR = ../../src
STUBSDIR = ../stubs
INCLUDEPATH += $$M_INSTALL_HEADERS $$SRCDIR $$STUBSDIR ../../include
DEPENDPATH = $$INCLUDEPATH
CONFIG += meegotouch silent link_pkgconfig
QT += testlib network gui dbus xml
exists($$[QT_INSTALL_LIBS]/libQtOpenGL.so):QT += opengl
TEMPLATE = app
DEFINES += UNIT_TEST
LIBS += -L../../lib

PKGCONFIG += \
          qmsystem \
          libngf0 \
          dbus-1

# For setting the coverage flag ON
contains(COV_OPTION, on) {
LIBS += -lgcov
QMAKE_CXXFLAGS += -ftest-coverage \
    -fprofile-arcs 
}

DEFINES += STATUSINDICATORMENU_PLUGIN_DIR=\'$$quote(\"$$STATUSINDICATORMENU_PLUGIN_DIR\")\'

# install tests
target.path = $$[QT_INSTALL_LIBS]/system-ui-tests
INSTALLS += target
