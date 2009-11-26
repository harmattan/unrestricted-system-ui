TEMPLATE = app
TARGET = page_win_appear
CONFIG += dui
QMAKE_CXXFLAGS += -g
QT += dbus
# Input
SOURCES += main.cpp
INCLUDEPATH += ../../src/include\
    /usr/include/dbus-1.0 \
    /usr/lib/dbus-1.0/include
QMAKE_LIBDIR += ../../lib/
HEADERS += main.h
