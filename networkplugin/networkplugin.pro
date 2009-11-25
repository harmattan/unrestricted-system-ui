MOC_DIR = .moc
OBJECTS_DIR = .objects
TEMPLATE = lib
CONFIG += plugin \
    gui \
    cellular-qt \
    dui \
    duistatusindicatormenu
LIBS += -lcellular-qt
INCLUDEPATH += /usr/include/duistatusindicatormenu \
    ../networkapplet
QT += dbus
HEADERS = networkplugin.h \
    network.h \    
    ../networkapplet/networkbusinesslogic.h
SOURCES = networkplugin.cpp \
    network.cpp \    
    ../networkapplet/networkbusinesslogic.cpp
TARGET = $$qtLibraryTarget(connectivity)
css.files = networkplugin.css
DESTDIR = lib
target.path += /usr/lib/duistatusindicatormenu/plugins
css.path += /usr/share/duistatusindicatormenu/themes/style/
INSTALLS += target \
    css
OTHER_FILES += networkplugin.css
