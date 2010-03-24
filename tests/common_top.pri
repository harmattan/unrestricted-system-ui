include(../check.pri)
include(../../duiconfig.pri)
SRCDIR = ../../src
STUBSDIR = ../stubs
INCLUDEPATH += $$DUI_INSTALL_HEADERS $$SRCDIR $$STUBSDIR ../../include
DEPENDPATH = $$INCLUDEPATH
CONFIG += debug dui silent link_pkgconfig
QT += testlib network gui dbus xml
exists($$[QT_INSTALL_LIBS]/libQtOpenGL.so):QT += opengl
TEMPLATE = app
DEFINES += UNIT_TEST
LIBS += -L../../lib

# For setting the coverage flag ON
contains(COV_OPTION, on) {
LIBS += -lgcov
QMAKE_CXXFLAGS += -ftest-coverage \
    -fprofile-arcs 
}

DEFINES += DUISTATUSINDICATORMENU_PLUGIN_DIR=\'$$quote(\"$$DUISTATUSINDICATORMENU_PLUGIN_DIR\")\'

# install tests
target.path = $$[QT_INSTALL_LIBS]/system-ui-tests
INSTALLS += target
