MOC_DIR = .moc
OBJECTS_DIR = .objects
TEMPLATE = lib
CONFIG += plugin \
    gui \
    dui \
    duistatusindicatormenu
INCLUDEPATH += /usr/include/duistatusindicatormenu
QT += dbus
HEADERS = networkplugin.h \
    network.h
SOURCES = networkplugin.cpp \
    network.cpp
TARGET = $$qtLibraryTarget(network)
DESTDIR = lib
target.path += /usr/lib/duistatusindicatormenu/plugins
INSTALLS += target
