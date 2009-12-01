MOC_DIR = .moc
OBJECTS_DIR = .objects
TEMPLATE = lib
CONFIG += plugin \
    gui \
    dui \
    duistatusindicatormenu
INCLUDEPATH += /usr/include/duistatusindicatormenu \
    ../batteryapplet \
    /usr/include/duicontrolpanel
QT += dbus
HEADERS = batteryplugin.h \
    battery.h \
    ../batteryapplet/batterydbusinterface.h \
    ../batteryapplet/batteryimage.h
SOURCES = batteryplugin.cpp \
    battery.cpp \
    ../batteryapplet/batterydbusinterface.cpp \
    ../batteryapplet/batteryimage.cpp
TARGET = $$qtLibraryTarget(battery)
css.files = batteryplugin.css
DESTDIR = lib
target.path += /usr/lib/duistatusindicatormenu/plugins
css.path += /usr/share/duistatusindicatormenu/themes/style/
INSTALLS += target \
    css
OTHER_FILES += batteryplugin.css
