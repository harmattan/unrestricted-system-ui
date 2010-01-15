MOC_DIR = .moc
DUIGEN_OUTDIR = .gen
OBJECTS_DIR = .obj
QT += dbus
LIBS += -lprofile-qt \
    -lduicontrolpanel
TEMPLATE      = lib
CONFIG       += plugin gui dui duistatusindicatormenu
INCLUDEPATH  += \
    ../profileapplet \
    /usr/include/profile-qt \
    /usr/include/duicontrolpanel
HEADERS = ../profileapplet/profiledatainterface.h \
    ../profileapplet/profilebuttons.h \
    profileplugin.h \
    profilewidget.h
SOURCES = ../profileapplet/profiledatainterface.cpp \
    ../profileapplet/profilebuttons.cpp \
    profileplugin.cpp \
    profilewidget.cpp
TARGET        = $$qtLibraryTarget(profile)
DESTDIR       = lib
target.path += /usr/lib/duistatusindicatormenu/plugins/ 
INSTALLS += \
    target
