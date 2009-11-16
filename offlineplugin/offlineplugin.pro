MOC_DIR = .moc
OBJECTS_DIR = .objects
TEMPLATE = lib
CONFIG += plugin \
    gui \
    dui \
    duistatusindicatormenu
LIBS += -lcellular-qt
INCLUDEPATH += /usr/include/duistatusindicatormenu
QT += dbus
HEADERS = offlineplugin.h \
    offline.h
SOURCES = offlineplugin.cpp \
    offline.cpp
TARGET = $$qtLibraryTarget(connectivity)
css.files = offlineplugin.css
DESTDIR = lib
target.path += /usr/lib/duistatusindicatormenu/plugins
css.path += /usr/share/duistatusindicatormenu/themes/style/
INSTALLS += target \
    css
OTHER_FILES += offlineplugin.css
