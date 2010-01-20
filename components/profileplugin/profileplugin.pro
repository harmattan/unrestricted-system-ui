MOC_DIR = .moc
DUIGEN_OUTDIR = .gen
OBJECTS_DIR = .obj
QT += dbus
LIBS += -lprofile-qt \
        -lduicontrolpanel
TEMPLATE      = lib
CONFIG       += plugin gui dui duistatusindicatormenu silent
INCLUDEPATH  += ../profileapplet \
                /usr/include/profile-qt \
                /usr/include/duicontrolpanel

HEADERS = profileplugin.h \
          profilewidget.h \
          ../profileapplet/profiledatainterface.h \
          ../profileapplet/profilebuttons.h \
          ../debug.h
          
SOURCES = profileplugin.cpp \
          profilewidget.cpp \
          ../profileapplet/profiledatainterface.cpp \
          ../profileapplet/profilebuttons.cpp \
          ../debug.cpp

TARGET        = $$qtLibraryTarget(profile)
DESTDIR       = lib
target.path += /usr/lib/duistatusindicatormenu/plugins/ 
INSTALLS +=  target
