MOC_DIR = .moc
OBJECTS_DIR = .objects
TEMPLATE = lib

CONFIG += plugin \
          gui \
          dui \
          duistatusindicatormenu \
          silent

QT += dbus

HEADERS = volumebarplugin.h \
          volumebarlogic.h \
          volumebar.h \
          ../debug.h

SOURCES = volumebarplugin.cpp \
          volumebarlogic.cpp \
          volumebar.cpp \
          ../debug.cpp

TARGET = $$qtLibraryTarget(volumebar)
DESTDIR = lib
target.path += /usr/lib/duistatusindicatormenu/plugins

css.files = volumebarplugin.css
css.path += /usr/share/duistatusindicatormenu/themes/style/

INSTALLS += target \
            css
