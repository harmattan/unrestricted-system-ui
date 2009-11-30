MOC_DIR = .moc
DUIGEN_OUTDIR = .gen
OBJECTS_DIR = .obj
LIBS += -lprofile-qt \
    -lduicontrolpanel
TEMPLATE      = lib
CONFIG       += plugin gui dui duistatusindicatormenu
INCLUDEPATH  += \
    ../../src \
    /usr/include/profile-qt \
    /usr/include/duicontrolpanel
HEADERS = profiledatainterface.h \
    profileplugin.h \
    profilewidget.h
SOURCES = profiledatainterface.cpp \
    profileplugin.cpp \
    profilewidget.cpp
TARGET        = $$qtLibraryTarget(profile)
DESTDIR       = lib
target.path += /usr/lib/duistatusindicatormenu/plugins/ 
INSTALLS += \
    target
